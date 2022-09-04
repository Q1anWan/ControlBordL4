#ifndef MOTOR_H
#define MOTOR_H
#include "main.h"
#include "can.h"
#ifdef __cplusplus
#define MOTOR1_6020 0x208
extern "C"
{
	void CAN_Task(void const * argument);
}
void CAN_Filter_Init(void);
void CAN_CMD_Motor(CAN_HandleTypeDef *hcan, uint32_t identifier, int16_t data1, int16_t data2, int16_t data3, int16_t data4);
#endif
#endif
