#ifndef MYTASKLIST_H
#define MYTASKLIST_H
#include "main.h"
#include "cmsis_os.h"
#define SHOW_SYS_INFO
void Task_Init(void);
extern osThreadId RGBTaskHandle;
extern osThreadId KeyScanTaskHandle;
extern osThreadId IMURecTaskHandle;

extern osSemaphoreId KeyScanSemHandle;
extern osSemaphoreId IMURecSemHandle;

#endif