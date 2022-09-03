#include "myTaskList.h"
#include "cmsis_os.h"
#include <string.h>
#include "usart.h"

#ifdef SHOW_SYS_INFO
osThreadId SysInfTaskHandle;
extern void SysInf_Task(void const * argument);
#endif

extern osThreadId defaultTaskHandle;
osThreadId RGBTaskHandle;
osThreadId KeyScanTaskHandle;
osThreadId IMURecTaskHandle;

osSemaphoreId KeyScanSemHandle;
osSemaphoreId IMURecSemHandle;

extern void RGB_Task(void const * argument);
extern void KeyScan_Task(void const * argument);
extern void BMI160_Rec_Task(void const * argument);


void Task_Init(void)
{
	#ifdef SHOW_SYS_INFO
	osThreadDef(SysInfTask, SysInf_Task, osPriorityLow, 0, 256);
	SysInfTaskHandle = osThreadCreate(osThread(SysInfTask), NULL);
	#endif
	vTaskDelete(defaultTaskHandle);
	
	osThreadDef(RGBTask, RGB_Task, osPriorityHigh, 0, 128);
	RGBTaskHandle = osThreadCreate(osThread(RGBTask), NULL);
	
	osSemaphoreDef(KeyScanSem);
	KeyScanSemHandle = osSemaphoreCreate(osSemaphore(KeyScanSem),1);

	osThreadDef(KeyScanTask, KeyScan_Task, osPriorityRealtime, 0, 128);
	KeyScanTaskHandle = osThreadCreate(osThread(KeyScanTask), NULL);
	
	osSemaphoreDef(IMURecSem);
	IMURecSemHandle = osSemaphoreCreate(osSemaphore(IMURecSem),1);

	osThreadDef(IMURecTask, BMI160_Rec_Task, osPriorityRealtime, 0, 128);
	IMURecTaskHandle = osThreadCreate(osThread(IMURecTask), NULL);

}




