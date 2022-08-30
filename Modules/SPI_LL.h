/*
	SPI LowLayer��SPI֧��
	  ǧ�� 2022.6.21
	  F1 F4 G0 G4 H7
	  Version:1.0			�����汾
	  Version:1.0.1			�޸����ͺ�����������ȫ����
	����DMA�շ���ҪDMA_LL��֧��
*/
/*
	֧�ֹ���

	SPI���ֽ��׷�
	�����ֽ���������
	�����ֽ�DMA����

	��DMA��ʹ��ʱ������DMA��ش���
*/
#ifndef SPI_LL_H
#define SPI_LL_H

#include "main.h"
#if defined(SPI1) || defined(SPI2) || defined(SPI3) || defined(SPI4)
#include "spi.h"
#if defined(DMA1) || defined(DMA2) || defined(DMA3)
#include "DMA_LL.h"
#endif
#ifdef __cplusplus
#if defined(DMA1) || defined(DMA2) || defined(DMA3)
class SPI_C : public DMA_C
#endif
#if !defined(DMA1) && !defined(DMA2) && !defined(DMA3)
			  class SPI_C
#endif
{
public:
	void Init(SPI_TypeDef *SPI, GPIO_TypeDef *CS_Port, uint32_t CS_Pin);
	void CS_Enable(void);
	void CS_Disable(void);
	uint8_t Read_Reg(uint8_t reg);
	void Read_Muti_Reg(uint8_t reg, uint16_t num, uint8_t *pdata);
	uint8_t Write_Reg(uint8_t reg, uint8_t data);
	uint8_t MesgTransmit(uint8_t *pdata, uint32_t size);

private:
	uint8_t Exchange_Byte(uint8_t byte);
	SPI_TypeDef *SPI;
	GPIO_TypeDef *CS_Port;
	uint32_t CS_Pin;
	uint16_t OutTime = 100;
	uint8_t blank_reg = 0xff;

/*����SPI DMAʱ*/
#if defined(DMA1) || defined(DMA2) || defined(DMA3)
public:
	uint8_t IsDMATransmitOK = 0;
	void DMA_IQR_Handel(void);
	void MesgTransmit_DMA(uint8_t *pdata, uint32_t size);
#endif
};

extern SPI_C SPI_1;
extern "C"
{
}
#endif
#endif
#endif
