#include "BSPTask.h"
#include "myTaskList.h"
#include "tim.h"
#include "BMI160.h"
#include "QCS_Lite.h"
#include "string.h"
#include "usart.h"
#include <stdio.h>
#include <stdarg.h>
#include "OLED.h"
#include "SPI_LL.h"
SPI_C SPI_2;
uint8_t KEY_Value=0;
uint8_t Printf_buf[256];

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
	/*����ϵ紥������*/
	LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_0);
	LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_0);
	for(;;)
	{
		/*��ȡ��ֵ��*/
		xSemaphoreTake(KeyScanSemHandle,portMAX_DELAY);
		/*����ʱ��*/
		vTaskDelay(10);
		/*ȷ������״̬*/
		KEY_Value = !LL_GPIO_IsInputPinSet(KEY_GPIO_Port,KEY_Pin);//����1
		/*���������ж�*/
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
void OLED_Task(void const * argument)
{
	vTaskDelay(100);
	OLED_Init();
	vTaskDelay(100);
	OLED_BMP(LOGO_DLUT);//����LOGO
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
	char MSG1[50]="\r\n������  ����״̬  ���ȼ�  ʣ��ջ  �������\r\n";
	char MSG2[30] = "\r\n������  ���м���  ʹ����\r\n";
	TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	int8_t show_which = 1,show_flag=0;
	for(;;)
	{
		vTaskDelayUntil(&xLastWakeTime,100);
		/*�����л�*/
		if(KEY_Value==1&&show_flag){show_which*=-1;show_flag=0;}
		else if(KEY_Value==0){show_flag=1;}
		/*������ʾ*/
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
