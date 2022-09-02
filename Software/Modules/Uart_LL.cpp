/*
	UART LowLayer库UART支持
	  千万 2022.6.27
	  F1 F4 G0 G4 H7
	  Version:1.0
	启用DMA收发需要DMA_LL库支持
*/
#include "Uart_LL.h"
#include "string.h"
/*串口初始化函数*/
void UART_C::Init(USART_TypeDef *Uart)
{
	this->Uartx = Uart;
	LL_USART_Enable(this->Uartx);
}
/*阻塞型发送*/
void UART_C::UART_TX(uint8_t *data, uint16_t num)
{
	uint16_t wait_cnt = 0;
	for (uint16_t i = 0; i < num; i++)
	{
		wait_cnt = 0;
		while (!LL_USART_IsActiveFlag_TXE(this->Uartx)) //等待数据发送完成
		{
			if (wait_cnt++ > TIME_OUT)
				break;
		}
		LL_USART_TransmitData8(this->Uartx, data[i]); //发送8位数据
	}
	wait_cnt = 0;
	while (!LL_USART_IsActiveFlag_TXE(this->Uartx)) //等待数据发送完成
	{
		if (wait_cnt++ > TIME_OUT)
			break;
	}
}
/*阻塞型接收*/
void UART_C::UART_RX(uint8_t *data, uint16_t num)
{
	uint16_t wait_cnt = 0;
	for (uint16_t i = 0; i < num; i++)
	{
		wait_cnt = 0;
		while (!LL_USART_IsActiveFlag_RXNE(this->Uartx)) //等待数据发送完成
		{
			if (wait_cnt++ > TIME_OUT)
				break;
		}
		data[i] = LL_USART_ReceiveData8(this->Uartx); //发送8位数据
	}
	wait_cnt = 0;
}
/*串口DMA发送初始化*/
void UART_DMA_TX_C::Init_DMA_TX(DMA_TypeDef *DMA, uint32_t DMA_Channelx)
{
	this->DMAx = DMA;
	this->DMA_Channelx = DMA_Channelx;
}
/*串口DMA接收初始化*/
void UART_DMA_RX_C::Init_DMA_RX(DMA_TypeDef *DMA, uint32_t DMA_Channelx)
{
	this->DMAx = DMA;
	this->DMA_Channelx = DMA_Channelx;
}
/*串口DMA接收*/
void UART_DMA_C::UART_DMA_RX(uint8_t *pData, uint16_t length)
{
	this->Target_Length = length;
	this->pRX_Data = pData;
#if defined(USART_RDR_RDR)
	LL_DMA_SetPeriphAddress(this->UART_DMA_RX_C::DMAx, this->UART_DMA_RX_C::DMA_Channelx, (uint32_t) & this->Uartx->RDR);
#else
	LL_DMA_SetPeriphAddress(this->UART_DMA_RX_C::DMAx, this->UART_DMA_RX_C::DMA_Channelx, (uint32_t) & this->Uartx->DR);
#endif
	LL_DMA_SetMemoryAddress(this->UART_DMA_RX_C::DMAx, this->UART_DMA_RX_C::DMA_Channelx, (uint32_t)this->pRX_BUF);
	LL_DMA_SetDataLength(this->UART_DMA_RX_C::DMAx, this->UART_DMA_RX_C::DMA_Channelx, RX_BUF_LEN); //必须在启用传输前设置
	LL_USART_ClearFlag_IDLE(this->Uartx);
	LL_USART_EnableIT_IDLE(this->Uartx);
	LL_USART_EnableDMAReq_RX(this->Uartx);
	this->UART_DMA_RX_C::DMA_EnableCH();
}
/*串口DMA发送*/
void UART_DMA_C::UART_DMA_TX(uint8_t *pData, uint16_t length)
{
	// if((!LL_USART_IsActiveFlag_TXE(this->Uartx))||(TX_BSY==1)){return;}
	if (TX_BSY == 1)
	{
		return;
	}
#if defined(USART_TDR_TDR)
	LL_DMA_SetPeriphAddress(this->UART_DMA_TX_C::DMAx, this->UART_DMA_TX_C::DMA_Channelx, (uint32_t) & this->Uartx->TDR);
#else
	LL_DMA_SetPeriphAddress(this->UART_DMA_TX_C::DMAx, this->UART_DMA_TX_C::DMA_Channelx, (uint32_t) & this->Uartx->DR);
#endif
	LL_DMA_SetMemoryAddress(this->UART_DMA_TX_C::DMAx, this->UART_DMA_TX_C::DMA_Channelx, (uint32_t)pData);
	LL_DMA_SetDataLength(this->UART_DMA_TX_C::DMAx, this->UART_DMA_TX_C::DMA_Channelx, length); //必须在启用传输前设置
	LL_USART_EnableDMAReq_TX(this->Uartx);
	LL_DMA_EnableIT_TC(this->UART_DMA_TX_C::DMAx, this->UART_DMA_TX_C::DMA_Channelx);
	this->TX_BSY = 1;
	this->UART_DMA_TX_C::DMA_EnableCH();
}

/*
	DMA接收中断处理函数
	可选是否使能双缓冲区
	USARTx中断下
*/
void UART_DMA_C::RX_Handel(void)
{
	if (LL_USART_IsActiveFlag_ORE(this->Uartx))
	{
		LL_USART_ClearFlag_ORE(this->Uartx);
	}
	if (LL_USART_IsActiveFlag_IDLE(this->Uartx))
	{
		/*关闭中断*/
		LL_USART_DisableIT_IDLE(this->Uartx);
		/*暂停DMA传输*/
		this->UART_DMA_RX_C::DMA_DisableCH();

		this->Recieve_length = RX_BUF_LEN - LL_DMA_GetDataLength(this->UART_DMA_RX_C::DMAx, this->UART_DMA_RX_C::DMA_Channelx); //计算已传输的个数
#ifdef ENABLE_DOUBLE_BUF
		/*交换缓冲区*/
		switch (this->BUF_ID)
		{
		case 0:
			this->BUF_ID = 1;
			this->pRX_BUF = this->RX_BUF1;
			break;
		case 1:
			this->BUF_ID = 0;
			this->pRX_BUF = this->RX_BUF0;
			break;
		}
		/*数据迁移*/
		switch (this->BUF_ID)
		{
		case 0:
			/*此时BUF1有效*/
			memcpy(this->pRX_Data, this->RX_BUF1, (this->Recieve_length > this->Target_Length) ? this->Target_Length : this->Recieve_length);
			break;
		case 1:
			/*此时BUF0有效*/
			memcpy(this->pRX_Data, this->RX_BUF0, (this->Recieve_length > this->Target_Length) ? this->Target_Length : this->Recieve_length);
			break;
		}
#else
		memcpy(this->pRX_Data, this->RX_BUF0, (this->Recieve_length > this->Target_Length) ? this->Target_Length : this->Recieve_length);
#endif
		/*退出中断*/
		this->UART_DMA_RX_C::DMA_Clear_TC();
		LL_USART_ClearFlag_IDLE(this->Uartx);
	}
}

/*
	DMA发送中断处理函数
	DMA_CHANNELx中断下
*/
void UART_DMA_C::TX_Handel(void)
{
	LL_DMA_DisableIT_TC(this->UART_DMA_TX_C::DMAx, this->UART_DMA_TX_C::DMA_Channelx);
	this->UART_DMA_TX_C::DMA_DisableCH();
	this->UART_DMA_TX_C::DMA_Clear_TC();
	this->TX_BSY = 0;
}
