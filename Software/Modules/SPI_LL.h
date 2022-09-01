/*
	SPI LowLayer库SPI支持
	  千万 2022.6.21
	  F1 F4 G0 G4 H7
	  Version:1.0			基础版本
	  Version:1.0.1			修复发送函数阻塞不完全问题
	启用DMA收发需要DMA_LL库支持
*/
/*
	支持功能

	SPI单字节首发
	连续字节阻塞发送
	连续字节DMA发送

	当DMA被使能时将启用DMA相关代码
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

/*启动SPI DMA时*/
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
