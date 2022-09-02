#include "myTaskList.h"
#include "cmsis_os.h"
#include <string.h>
#include "usart.h"

osThreadId RGBTaskHandle;
extern void RGB_Task(void const * argument);

osThreadId KeyScanTaskHandle;
extern void KeyScan_Task(void const * argument);
osSemaphoreId KeyScanSemHandle;

#ifdef SHOW_SYS_INFO
osThreadId SysInfTaskHandle;
extern void SysInf_Task(void const * argument);
#endif
void Task_Init(void)
{
	osThreadDef(RGBTask, RGB_Task, osPriorityLow, 0, 128);
	RGBTaskHandle = osThreadCreate(osThread(RGBTask), NULL);
	
	osSemaphoreDef(KeyScanSemHandle);
	KeyScanSemHandle = osSemaphoreCreate(osSemaphore(KeyScanSemHandle),1);
	osThreadDef(KeyScanTask, KeyScan_Task, osPriorityHigh, 0, 128);
	KeyScanTaskHandle = osThreadCreate(osThread(KeyScanTask), NULL);
	#ifdef SHOW_SYS_INFO
	osThreadDef(SysInfTask, SysInf_Task, osPriorityNormal, 0, 256);
	SysInfTaskHandle = osThreadCreate(osThread(SysInfTask), NULL);
	#endif
	}




