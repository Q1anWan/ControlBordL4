/*
	UART LowLayer库UART支持
	  千万 2022.6.27
	  F1 F4 G0 G4 H7
	  Version:1.0
	启用DMA收发需要DMA_LL库支持
*/
#ifndef Uart_LL_H
#define Uart_LL_H
#include "main.h"
#if defined(USART1) || defined(USART2) || defined(USART3) || defined(USART6) || defined(UART4) || defined(UART5) || defined(UART7) || defined(UART8) || defined(UART9) || defined(USART10)
#if defined(DMA1) || defined(DMA2) || defined(DMA3)
#include "DMA_LL.h"
#endif
#define RX_BUF_LEN 64
#define TIME_OUT 12000
#ifdef __cplusplus
/*
	串口基类
	不使用DMA时使用此类实体化
	具有阻塞接收、发送功能

	使用前先执行Init函数
*/
class UART_C
{
public:
	USART_TypeDef *Uartx;
	void Init(USART_TypeDef *Uart);
	void UART_TX(uint8_t *data, uint16_t num);
	void UART_RX(uint8_t *data, uint16_t num);
};

/*启动DMA时将使能本部分代码*/
#if defined(DMA1) || defined(DMA2) || defined(DMA3)

/**@brief是否使能双缓冲区*/
//#define ENABLE_DOUBLE_BUF

/*DMA发送基类 不要实体化*/
class UART_DMA_TX_C : public DMA_C
{
public:
	void Init_DMA_TX(DMA_TypeDef *DMA, uint32_t DMA_Channelx);
};
/*DMA接收基类 不要实体化*/
class UART_DMA_RX_C : public DMA_C
{
public:
	void Init_DMA_RX(DMA_TypeDef *DMA, uint32_t DMA_Channelx);
};
/*
	串口DMA基类
	使用DMA时使用此类实体化
	继承了阻塞接收、发送功能
	具有DMA收发功能

	使用前先执行Init函数
	再执行响应的Init_DMA函数
	随后可以进行收发

	相应的中断处理函数需放置到位
*/
class UART_DMA_C : public UART_C, public UART_DMA_TX_C, public UART_DMA_RX_C
{

private:
	uint16_t Target_Length = 0;

	uint8_t RX_BUF0[RX_BUF_LEN];
#ifdef ENABLE_DOUBLE_BUF
	uint8_t RX_BUF1[RX_BUF_LEN];
	uint8_t BUF_ID = 0;
#endif
	uint8_t *pRX_Data;
	uint8_t *pRX_BUF = RX_BUF0;
	uint8_t TX_BSY = 0;

public:
	uint32_t Recieve_length = 0;
	void RX_Handel(void); // Should be under USARTx IT
	void TX_Handel(void); // Should be under DMACHx IT

	void UART_DMA_RX(uint8_t *pData, uint16_t length);
	void UART_DMA_TX(uint8_t *pData, uint16_t length);
};
#endif
#endif
extern "C"
{
	/*C Code Begin*/
	/*C Code End*/
}
#endif
#endif