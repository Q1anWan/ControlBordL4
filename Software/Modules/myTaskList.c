#include "myTaskList.h"
#include "cmsis_os.h"
#include <string.h>
#include "usart.h"

#if (SHOW_SYS_INFO==1)
osThreadId SysInfTaskHandle;
extern void SysInf_Task(void const * argument);
#endif

extern osThreadId defaultTaskHandle;
osThreadId RGBTaskHandle;
osThreadId KeyScanTaskHandle;
osThreadId IMURecTaskHandle;
osThreadId IMUTemTaskHandle;
osThreadId IMUShowAngelHandle;
osThreadId CANTaskHandle;
osThreadId OLEDTaskHandle;
osThreadId ADCTaskHandle;

osSemaphoreId KeyScanSemHandle;
osSemaphoreId IMURecSemHandle;

extern void RGB_Task(void const * argument);
extern void KeyScan_Task(void const * argument);
extern void BMI160_Rec_Task(void const * argument);
extern void BMI160_Tem_Task(void const * argument);
extern void BMI160_InsUpdate_Task(void const * argument);
extern void CAN_Task(void const * argument);
extern void OLED_Task(void const * argument);
extern void ADC_Task(void const * argument);

void Task_Init(void)
{
	#if (SHOW_SYS_INFO==1)
	osThreadDef(SysInfTask, SysInf_Task, osPriorityLow, 0, 256);
	SysInfTaskHandle = osThreadCreate(osThread(SysInfTask), NULL);
	#endif
	vTaskDelete(defaultTaskHandle);
	
	osThreadDef(RGBTask, RGB_Task, osPriorityNormal, 0, 128);
	RGBTaskHandle = osThreadCreate(osThread(RGBTask), NULL);
	
	osThreadDef(OLEDTask, OLED_Task, osPriorityNormal, 0, 128);
	OLEDTaskHandle = osThreadCreate(osThread(OLEDTask), NULL);
	
	osSemaphoreDef(KeyScanSem);
	KeyScanSemHandle = osSemaphoreCreate(osSemaphore(KeyScanSem),1);

	osThreadDef(KeyScanTask, KeyScan_Task, osPriorityRealtime, 0, 128);
	KeyScanTaskHandle = osThreadCreate(osThread(KeyScanTask), NULL);
	
	osSemaphoreDef(IMURecSem);
	IMURecSemHandle = osSemaphoreCreate(osSemaphore(IMURecSem),1);

	osThreadDef(IMURecTask, BMI160_Rec_Task, osPriorityRealtime, 0, 256);
	IMURecTaskHandle = osThreadCreate(osThread(IMURecTask), NULL);
	
	osThreadDef(IMUTemTask, BMI160_Tem_Task, osPriorityNormal, 0, 128);
	IMUTemTaskHandle = osThreadCreate(osThread(IMUTemTask), NULL);
	
	osThreadDef(IMUShowAngelTask, BMI160_InsUpdate_Task, osPriorityAboveNormal, 0, 256);
	IMUShowAngelHandle = osThreadCreate(osThread(IMUShowAngelTask), NULL);
	
	osThreadDef(CANTask, CAN_Task, osPriorityNormal, 0, 256);
	CANTaskHandle = osThreadCreate(osThread(CANTask), NULL);
	
	osThreadDef(ADCTask, ADC_Task, osPriorityNormal, 0, 128);
	ADCTaskHandle = osThreadCreate(osThread(ADCTask), NULL);

}





