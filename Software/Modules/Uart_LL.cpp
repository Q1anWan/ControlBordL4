/*
	UART LowLayer��UART֧��
	  ǧ�� 2022.6.27
	  F1 F4 G0 G4 H7
	  Version:1.0
	����DMA�շ���ҪDMA_LL��֧��
*/
#include "Uart_LL.h"
#include "string.h"
/*���ڳ�ʼ������*/
void UART_C::Init(USART_TypeDef *Uart)
{
	this->Uartx = Uart;
	LL_USART_Enable(this->Uartx);
}
/*�����ͷ���*/
void UART_C::UART_TX(uint8_t *data, uint16_t num)
{
	uint16_t wait_cnt = 0;
	for (uint16_t i = 0; i < num; i++)
	{
		wait_cnt = 0;
		while (!LL_USART_IsActiveFlag_TXE(this->Uartx)) //�ȴ����ݷ������
		{
			if (wait_cnt++ > TIME_OUT)
				break;
		}
		LL_USART_TransmitData8(this->Uartx, data[i]); //����8λ����
	}
	wait_cnt = 0;
	while (!LL_USART_IsActiveFlag_TXE(this->Uartx)) //�ȴ����ݷ������
	{
		if (wait_cnt++ > TIME_OUT)
			break;
	}
}
/*�����ͽ���*/
void UART_C::UART_RX(uint8_t *data, uint16_t num)
{
	uint16_t wait_cnt = 0;
	for (uint16_t i = 0; i < num; i++)
	{
		wait_cnt = 0;
		while (!LL_USART_IsActiveFlag_RXNE(this->Uartx)) //�ȴ����ݷ������
		{
			if (wait_cnt++ > TIME_OUT)
				break;
		}
		data[i] = LL_USART_ReceiveData8(this->Uartx); //����8λ����
	}
	wait_cnt = 0;
}
/*����DMA���ͳ�ʼ��*/
void UART_DMA_TX_C::Init_DMA_TX(DMA_TypeDef *DMA, uint32_t DMA_Channelx)
{
	this->DMAx = DMA;
	this->DMA_Channelx = DMA_Channelx;
}
/*����DMA���ճ�ʼ��*/
void UART_DMA_RX_C::Init_DMA_RX(DMA_TypeDef *DMA, uint32_t DMA_Channelx)
{
	this->DMAx = DMA;
	this->DMA_Channelx = DMA_Channelx;
}
/*����DMA����*/
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
	LL_DMA_SetDataLength(this->UART_DMA_RX_C::DMAx, this->UART_DMA_RX_C::DMA_Channelx, RX_BUF_LEN); //���������ô���ǰ����
	LL_USART_ClearFlag_IDLE(this->Uartx);
	LL_USART_EnableIT_IDLE(this->Uartx);
	LL_USART_EnableDMAReq_RX(this->Uartx);
	this->UART_DMA_RX_C::DMA_EnableCH();
}
/*����DMA����*/
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
	LL_DMA_SetDataLength(this->UART_DMA_TX_C::DMAx, this->UART_DMA_TX_C::DMA_Channelx, length); //���������ô���ǰ����
	LL_USART_EnableDMAReq_TX(this->Uartx);
	LL_DMA_EnableIT_TC(this->UART_DMA_TX_C::DMAx, this->UART_DMA_TX_C::DMA_Channelx);
	this->TX_BSY = 1;
	this->UART_DMA_TX_C::DMA_EnableCH();
}

/*
	DMA�����жϴ�����
	��ѡ�Ƿ�ʹ��˫������
	USARTx�ж���
*/
void UART_DMA_C::RX_Handel(void)
{
	if (LL_USART_IsActiveFlag_ORE(this->Uartx))
	{
		LL_USART_ClearFlag_ORE(this->Uartx);
	}
	if (LL_USART_IsActiveFlag_IDLE(this->Uartx))
	{
		/*�ر��ж�*/
		LL_USART_DisableIT_IDLE(this->Uartx);
		/*��ͣDMA����*/
		this->UART_DMA_RX_C::DMA_DisableCH();

		this->Recieve_length = RX_BUF_LEN - LL_DMA_GetDataLength(this->UART_DMA_RX_C::DMAx, this->UART_DMA_RX_C::DMA_Channelx); //�����Ѵ���ĸ���
#ifdef ENABLE_DOUBLE_BUF
		/*����������*/
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
		/*����Ǩ��*/
		switch (this->BUF_ID)
		{
		case 0:
			/*��ʱBUF1��Ч*/
			memcpy(this->pRX_Data, this->RX_BUF1, (this->Recieve_length > this->Target_Length) ? this->Target_Length : this->Recieve_length);
			break;
		case 1:
			/*��ʱBUF0��Ч*/
			memcpy(this->pRX_Data, this->RX_BUF0, (this->Recieve_length > this->Target_Length) ? this->Target_Length : this->Recieve_length);
			break;
		}
#else
		memcpy(this->pRX_Data, this->RX_BUF0, (this->Recieve_length > this->Target_Length) ? this->Target_Length : this->Recieve_length);
#endif
		/*�˳��ж�*/
		this->UART_DMA_RX_C::DMA_Clear_TC();
		LL_USART_ClearFlag_IDLE(this->Uartx);
	}
}

/*
	DMA�����жϴ�����
	DMA_CHANNELx�ж���
*/
void UART_DMA_C::TX_Handel(void)
{
	LL_DMA_DisableIT_TC(this->UART_DMA_TX_C::DMAx, this->UART_DMA_TX_C::DMA_Channelx);
	this->UART_DMA_TX_C::DMA_DisableCH();
	this->UART_DMA_TX_C::DMA_Clear_TC();
	this->TX_BSY = 0;
}
