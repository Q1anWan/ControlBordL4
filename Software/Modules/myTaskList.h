#ifndef MYTASKLIST_H
#define MYTASKLIST_H
#include "main.h"
#include "cmsis_os.h"
#define SHOW_SYS_INFO 1
void Task_Init(void);
extern osThreadId RGBTaskHandle;
extern osThreadId KeyScanTaskHandle;
extern osThreadId IMURecTaskHandle;
extern osThreadId IMUTemTaskHandle;
extern osThreadId IMUShowAngelHandle;
extern osThreadId CANTaskHandle;
extern osThreadId OLEDTaskHandle;
extern osSemaphoreId KeyScanSemHandle;
extern osSemaphoreId IMURecSemHandle;

#endif