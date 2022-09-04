#include "Motor.h"
#include "can.h"
#include "arm_math.h"
#include "myTaskList.h"
void CAN_Task(void const * argument)
{
	CAN_Filter_Init();
	TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	for(;;)
	{
		vTaskDelayUntil(&xLastWakeTime,1);
		CAN_CMD_Motor(&hcan1,0x1FF,0x1234,0x5678,0X9ABC,0X0000);
	}
}

/*CAN滤波器配置*/
void CAN_Filter_Init(void)
{
	CAN_FilterTypeDef can_filter_st;
	can_filter_st.FilterActivation = ENABLE;
	can_filter_st.FilterMode = CAN_FILTERMODE_IDMASK;
	can_filter_st.FilterScale = CAN_FILTERSCALE_32BIT;
	can_filter_st.FilterIdHigh = 0x0000;
	can_filter_st.FilterIdLow = 0x0000;
	can_filter_st.FilterMaskIdHigh = 0x0000;
	can_filter_st.FilterMaskIdLow = 0x0000;
	can_filter_st.FilterBank = 0;
	can_filter_st.FilterFIFOAssignment = CAN_RX_FIFO0;
	HAL_CAN_ConfigFilter(&hcan1, &can_filter_st);
	HAL_CAN_Start(&hcan1);
	HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);
}

/*CAN接收中断*/
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	CAN_RxHeaderTypeDef rx_header;
	uint8_t rx_data[8];
	uint8_t i;

	HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rx_header, rx_data);
	
	switch (rx_header.StdId)
	{
		case MOTOR1_6020:
		//GM6020.UpdataMotorStatus(rx_data);
		break;	
		default:
			break;
	}
}

/*CAN数据包发送*/
void CAN_CMD_Motor(CAN_HandleTypeDef *hcan, uint32_t identifier, int16_t data1, int16_t data2, int16_t data3, int16_t data4)
{
	CAN_TxHeaderTypeDef tx_header;
	uint8_t tx_data[8];
	uint32_t tx_mail_box;

	tx_header.StdId = identifier;
	tx_header.IDE = CAN_ID_STD;
	tx_header.RTR = CAN_RTR_DATA;
	tx_header.DLC = 0x08;

	tx_data[0] = (data1 >> 8);
	tx_data[1] = data1;
	tx_data[2] = (data2 >> 8);
	tx_data[3] = data2;
	tx_data[4] = (data3 >> 8);
	tx_data[5] = data3;
	tx_data[6] = (data4 >> 8);
	tx_data[7] = data4;

	HAL_CAN_AddTxMessage(hcan, &tx_header, tx_data, &tx_mail_box);
}