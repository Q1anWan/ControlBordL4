#include "myTaskList.h"
#include "cmsis_os.h"

osThreadId RGBTaskHandle;
extern void RGB_Task(void const * argument);

void Task_Init(void)
{
	osThreadDef(RGBTask, RGB_Task, osPriorityNormal, 0, 128);
	RGBTaskHandle = osThreadCreate(osThread(RGBTask), NULL);

}



