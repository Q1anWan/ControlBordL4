#include "BSPTask.h"
#include "myTaskList.h"
#include "tim.h"
#include "string.h"
#include "usart.h"
uint8_t KEY_Value=0;
void RGB_Task(void const * argument)
{
	int8_t  RGB_ADD[3]={1,1,1};
	uint16_t RGB[3]={333,0,999};
	TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	for(;;)
	{
		vTaskDelayUntil(&xLastWakeTime,3);
		for(uint8_t i=0;i<3;i++){
		RGB[i] += RGB_ADD[i];
		if(RGB[i]==0)RGB_ADD[i]*=-1;
		else if(RGB[i]==1000)RGB_ADD[i]*=-1;
		}
		__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_4,RGB[0]);
		if(!KEY_Value){
		__HAL_TIM_SetCompare(&htim16,TIM_CHANNEL_1,RGB[1]);}
		else{
		__HAL_TIM_SetCompare(&htim16,TIM_CHANNEL_1,1000);}
		__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_3,RGB[2]);

	}
}
	
void KeyScan_Task(void const * argument)
{
	osDelay(10);
	/*解决上电触发问题*/
	LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_0);
	LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_0);
	for(;;)
	{
		/*获取二值量*/
		xSemaphoreTake(KeyScanSemHandle,portMAX_DELAY);
		/*消抖时间*/
		vTaskDelay(10);
		/*确定按键状态*/
		KEY_Value = !LL_GPIO_IsInputPinSet(KEY_GPIO_Port,KEY_Pin);//按下是1
		/*重新启用中断*/
		LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_0);
	}
}

uint32_t Time_Counter;
#if (SHOW_SYS_INFO==1)

void configureTimerForRunTimeStats(void)
{
	HAL_TIM_Base_Start_IT(&htim6);
	Time_Counter = 0;
}

unsigned long getRunTimeCounterValue(void)
{
	return Time_Counter;
}

void SysInf_Task(void const * argument)
{
	uint8_t CPU_RunInfo[400] = {0};
	char MSG1[50]="\r\n任务名  任务状态  优先级  剩余栈  任务序号\r\n";
	char MSG2[30] = "\r\n任务名  运行计数  使用率\r\n";
	TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	for(;;)
	{
		vTaskDelayUntil(&xLastWakeTime,100);
		if(KEY_Value==0)
		{
			memset(CPU_RunInfo, 0, 400);
			vTaskList((char *)&CPU_RunInfo);	
			HAL_UART_Transmit(&huart2,(uint8_t *)MSG1,50,1000);
			HAL_UART_Transmit(&huart2,CPU_RunInfo,400,1000);
		}
		else
		{
			memset(CPU_RunInfo,0,400); 
			vTaskGetRunTimeStats((char *)&CPU_RunInfo);
			HAL_UART_Transmit(&huart2,(uint8_t *)MSG2,30,1000);
			HAL_UART_Transmit(&huart2,CPU_RunInfo,400,1000);
		}
	}
}
#endif
