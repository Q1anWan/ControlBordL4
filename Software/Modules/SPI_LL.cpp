/*
	SPI LowLayer��SPI֧��
	  ǧ�� 2022.6.30
	  F1 F4 G0 G4 H7
	  Version:1.0.1
	����DMA�շ���ҪDMA_LL��֧��
*/
/*CS���Ž����ټ��ɿ��ƹ���*/
#include "SPI_LL.h"
#if defined(SPI1) || defined(SPI2) || defined(SPI3) || defined(SPI4)
/*
	��ʼ��SPI�ӿ�
	SPI�ṹ�塢Ƭѡ�źŶ˿ڡ�Ƭѡ����
*/
void SPI_C::Init(SPI_TypeDef *SPI, GPIO_TypeDef *CS_Port, uint32_t CS_Pin)
{
	this->SPI = SPI;
	this->CS_Port = CS_Port;
	this->CS_Pin = CS_Pin;
	LL_SPI_Enable(this->SPI);
#if defined(SPI_CR1_CSTART)
	LL_SPI_StartMasterTransfer(this->SPI);
#endif
}

/*
	Ƭѡ�ź���Ч
	һ�����õ͵�ƽ
*/
void SPI_C::CS_Enable(void)
{
	LL_GPIO_ResetOutputPin(this->CS_Port, this->CS_Pin);
}

/*
	Ƭѡ�ź���Ч
	һ�����øߵ�ƽ
*/
void SPI_C::CS_Disable(void)
{
	LL_GPIO_SetOutputPin(this->CS_Port, this->CS_Pin);
}

/*
	SPI���ݽ���
	��һ�ֽ���һ�ֽ�
*/
uint8_t SPI_C::Exchange_Byte(uint8_t byte)
{
	uint8_t wait_cnt = 0;
#if !defined(SPI_SR_TXP)
	while (!LL_SPI_IsActiveFlag_TXE(this->SPI)) //�ȴ����ݷ������
#else
	while (!LL_SPI_IsActiveFlag_TXP(this->SPI))
#endif
	{
		if (wait_cnt++ > this->OutTime)
			break;
	}
	LL_SPI_TransmitData8(this->SPI, byte); //����8λ����
	
	wait_cnt = 0;
#if !defined(SPI_SR_TXP)//�ȴ����һ�����ݷ������
	while (LL_SPI_IsActiveFlag_BSY(this->SPI)) 
#else
	while (!LL_SPI_IsActiveFlag_TXP(this->SPI))
#endif
	{
		if (wait_cnt++ > this->OutTime)
			break;
	}
	
	wait_cnt = 0;
#if !defined(SPI_SR_RXP)
	while (!LL_SPI_IsActiveFlag_RXNE(this->SPI)) //�ȴ����ݷ������
#else
	while (!LL_SPI_IsActiveFlag_RXP(this->SPI))
#endif
	{
		if (wait_cnt++ > this->OutTime)
			break;
	}
	return LL_SPI_ReceiveData8(this->SPI); //����SPI2���յ�������
}

/*
	SPI�Ĵ�����ȡ
	������Ϣ֡���֡
	���տ�֡����Ϣ֡
*/
uint8_t SPI_C::Read_Reg(uint8_t Reg)
{
	uint8_t Data = this->blank_reg;
	Reg |= 0x80;
	this->Exchange_Byte(Reg);
	Data = Exchange_Byte(Data);
	return Data;
}

/*
	SPI�Ĵ���������ȡ
	����ͷ��Ϣ֡��num����֡
	����ͷ��֡��num����Ϣ֡
*/
void SPI_C::Read_Muti_Reg(uint8_t reg, uint16_t num, uint8_t *pdata)
{
	reg |= 0x80;
	this->Exchange_Byte(reg);
	for (uint16_t i = 0; i < num; i++)
	{
		pdata[i] = this->Exchange_Byte(this->blank_reg);
	}
}

/*
	SPI�Ĵ���д��
	����������Ϣ֡
	���տ�֡������Ϣ֡
*/
uint8_t SPI_C::Write_Reg(uint8_t Reg, uint8_t data)
{
	Reg = this->Exchange_Byte(Reg);
	this->Exchange_Byte(data);
	return Reg;
}

/*
	SPI��Ϣ����
	����size����Ϣ֡
	�������һ����Ϣ֡��
	�����н��յ������ݽ������ж�ȡ
*/
uint8_t SPI_C::MesgTransmit(uint8_t *pdata, uint32_t size)
{
	/*У������*/
	if ((pdata == NULL) || (size == 0))
	{
		return this->blank_reg;
	}

	/*���ݷ���*/
	uint8_t wait_cnt = 0;
	for (uint32_t i = 0; i < size; i++)
	{
		wait_cnt = 0;
#if !defined(SPI_SR_TXP)
		while (!LL_SPI_IsActiveFlag_TXE(this->SPI)) //�ȴ����ݷ������
#else
		while (!LL_SPI_IsActiveFlag_TXP(this->SPI))
#endif
		{
			if (wait_cnt++ > this->OutTime)
				break;
		}
		LL_SPI_TransmitData8(this->SPI, pdata[i]); //����8λ����
	}

	wait_cnt = 0;
#if !defined(SPI_SR_TXP)
	while (LL_SPI_IsActiveFlag_BSY(this->SPI)) //�ȴ����һ�������շ����
#else
	while (!LL_SPI_IsActiveFlag_TXP(this->SPI))
#endif
	{
		if (wait_cnt++ > this->OutTime)
			break;
	}

	/*������͹����в��������ݵ��µ����*/
	LL_SPI_ClearFlag_OVR(this->SPI);
	LL_SPI_ReceiveData8(this->SPI);
	return LL_SPI_ReceiveData8(this->SPI);
}

/*����SPI DMAʱ*/
#if (ENABLE_SPI_DMA==1)
/*
	DMA�жϻص�����
	������ݴ�������жϲ��ñ�־λ
*/
void SPI_C::DMA_IQR_Handel(void)
{
	LL_DMA_DisableIT_TC(this->DMAx, this->DMA_Channelx);
	this->DMA_DisableCH();
	this->DMA_Clear_TC();
	
	uint16_t wait_cnt = 0;
	#if !defined(SPI_SR_TXP)
	while (LL_SPI_IsActiveFlag_BSY(this->SPI)) //�ȴ����һ�������շ����
#else
	while (!LL_SPI_IsActiveFlag_TXP(this->SPI))
#endif
	{
		if (wait_cnt++ > this->OutTime)
			break;
	}
	
	LL_SPI_ClearFlag_OVR(this->SPI);
	IsDMATransmitOK = 0;
}
/*
	DMA�������ݷ���
*/
void SPI_C::MesgTransmit_DMA(uint8_t *pdata, uint32_t size)
{
	/*У������*/
	if ((pdata == NULL) || (size == 0))
	{
		return;
	}
#if !defined(SPI_TXDR_TXDR)
	LL_DMA_SetPeriphAddress(this->DMAx, this->DMA_Channelx, (uint32_t) & this->SPI->DR);
#else
	LL_DMA_SetPeriphAddress(this->DMAx, this->DMA_Channelx, (uint32_t) & this->SPI->TXDR);
#endif

	LL_DMA_SetMemoryAddress(this->DMAx, this->DMA_Channelx, (uint32_t)pdata);
	LL_DMA_SetDataLength(this->DMAx, this->DMA_Channelx, size);
	LL_DMA_EnableIT_TC(this->DMAx, this->DMA_Channelx);
	LL_SPI_EnableDMAReq_TX(this->SPI);
	this->DMA_EnableCH();
	this->IsDMATransmitOK = 1;
}
#endif
#endif
