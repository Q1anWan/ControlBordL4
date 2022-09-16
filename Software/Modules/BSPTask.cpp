#include "BSPTask.h"

#include "string.h"
#include <stdio.h>
#include <stdarg.h>

#include "tim.h"
#include "usart.h"
#include "adc.h"

#include "myTaskList.h"
#include "BMI160.h"
#include "QCS_Lite.h"
#include "OLED.h"
#include "SPI_LL.h"

SPI_C SPI_2;
uint8_t KEY_Value=0;
uint8_t Printf_buf[256];

uint16_t ADC=1499;
float VDDA=0.0f;

void RGB_Task(void const * argument)
{
	int8_t  RGB_ADD[3]={4,4,4};
	uint16_t RGB[3]={180,500,816};
	TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_4);
	HAL_TIM_PWM_Start(&htim16,TIM_CHANNEL_1);

	for(;;)
	{
		vTaskDelayUntil(&xLastWakeTime,10);
		for(uint8_t i=0;i<3;i++){
		RGB[i] += RGB_ADD[i];
		if(RGB[i]<=180)RGB_ADD[i]*=-1;
		else if(RGB[i]>=820)RGB_ADD[i]*=-1;
		}
		__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_4,(uint32_t)(BMI160.Angel[0])+RGB[0]);
		__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_3,(uint32_t)(BMI160.Angel[1])+RGB[1]);

		if(!KEY_Value){
		__HAL_TIM_SetCompare(&htim16,TIM_CHANNEL_1,(uint32_t)(2*BMI160.Angel[2])+RGB[2]);}
		else{
		__HAL_TIM_SetCompare(&htim16,TIM_CHANNEL_1,1000);}

		QCS_Show_Degree(BMI160.AHRS_Q,BMI160.Angel);
		//usb_printf("|Line1=%f\r\n\t|%f\t|%f\t|%f\t|\r\n",BMI160.Temperature,BMI160.Angel[0],BMI160.Angel[1],BMI160.Angel[2]);
//		usb_printf("Roll=%f,Yaw=%f,Pitch=%f,",BMI160.Angel[0],BMI160.Angel[1],BMI160.Angel[2]);

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
		KEY_Value = !LL_GPIO_IsInputPinSet(KEY_GPIO_Port,KEY_Pin);//按是1
		/*重新启用中断*/
		LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_0);
	}
}

void usb_printf(const char *fmt,...)
{
    static va_list ap;
    uint16_t len = 0;

    va_start(ap, fmt);

    len = vsprintf((char *)Printf_buf, fmt, ap);

    va_end(ap);
	HAL_UART_Transmit(&huart2,Printf_buf,len,1000);
}

/*这个进程中，使用IIC驱动OLED刷新了一次图像，并测试了SPI2和UART1接口*/
void OLED_Task(void const * argument)
{
	vTaskDelay(100);
	OLED_Init();
	vTaskDelay(100);
	OLED_BMP(LOGO_DLUT);//播放LOGO
	SPI_2.Init(SPI2,SPI2_CS_GPIO_Port,SPI2_CS_Pin);
	for(;;)
	{
		uint8_t a[3]={1,2,3};
		HAL_UART_Transmit(&huart1,a,10,1000);
		SPI_2.CS_Enable();
		SPI_2.MesgTransmit(a,3);
		SPI_2.CS_Disable();
		vTaskDelay(10);
	}
}

/*这个进程中，使用了4位超采样读取了内部电压值，并校准了VDDA值*/
void ADC_Task(void const * argument)
{
	uint16_t ADC_A=0;
	uint32_t ADC_S=0;
	vTaskDelay(100);
	HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
	vTaskDelay(100);
	HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
	vTaskDelay(100);
		
	for(uint16_t i=0;i<1000;i++)			//预热
	{
		vTaskDelay(10);
		HAL_ADC_Start(&hadc1);	            //启动ADC单次转换
		vTaskDelay(10);
		ADC_A = HAL_ADC_GetValue(&hadc1); 	//读取ADC转换数据
		ADC = 0.9*ADC+0.1*ADC_A;			//一阶低通滤波
	}
	for(uint16_t i=0;i<1000;i++)			//校准VDDA
	{
		vTaskDelay(10);
		HAL_ADC_Start(&hadc1);	             //启动ADC单次转换
		vTaskDelay(10);
		ADC_A = HAL_ADC_GetValue(&hadc1); 	//读取ADC转换数据
		ADC = 0.9*ADC+0.1*ADC_A;	
		ADC_S += ADC;
	}
	ADC_S /= 1000;							//计算VDDA
	VDDA = (3.0f*(*(__IO uint16_t *)(0x1FFF75AA))) / ADC_S;	//读取校准参数
	for(;;)
	{
		vTaskDelay(10);
		HAL_ADC_Start(&hadc1);	            //启动ADC单次转换
		vTaskDelay(10);
		ADC_A = HAL_ADC_GetValue(&hadc1); 	//读取ADC转换数据
		ADC = 0.9*ADC+0.1*ADC_A;
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
	int8_t show_which = 1,show_flag=0;
	for(;;)
	{
		vTaskDelayUntil(&xLastWakeTime,100);
		/*按键切换*/
		if(KEY_Value==1&&show_flag){show_which*=-1;show_flag=0;}
		else if(KEY_Value==0){show_flag=1;}
		/*内容显示*/
		if(show_which==1)
		{
			memset(CPU_RunInfo, 0, 400);
			vTaskList((char *)&CPU_RunInfo);	
			HAL_UART_Transmit(&huart2,(uint8_t *)MSG1,50,1000);
			HAL_UART_Transmit(&huart2,CPU_RunInfo,400,1000);
			LL_GPIO_ResetOutputPin(SERVO_EN_GPIO_Port,SERVO_EN_Pin);
		}
		else
		{
			memset(CPU_RunInfo,0,400); 
			vTaskGetRunTimeStats((char *)&CPU_RunInfo);
			HAL_UART_Transmit(&huart2,(uint8_t *)MSG2,30,1000);
			HAL_UART_Transmit(&huart2,CPU_RunInfo,400,1000);
			LL_GPIO_SetOutputPin(SERVO_EN_GPIO_Port,SERVO_EN_Pin);
		}
	}
}
#endif
