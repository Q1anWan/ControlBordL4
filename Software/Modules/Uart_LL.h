/*
	UART LowLayer��UART֧��
	  ǧ�� 2022.6.27
	  F1 F4 G0 G4 H7
	  Version:1.0
	����DMA�շ���ҪDMA_LL��֧��
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
	���ڻ���
	��ʹ��DMAʱʹ�ô���ʵ�廯
	�����������ա����͹���

	ʹ��ǰ��ִ��Init����
*/
class UART_C
{
public:
	USART_TypeDef *Uartx;
	void Init(USART_TypeDef *Uart);
	void UART_TX(uint8_t *data, uint16_t num);
	void UART_RX(uint8_t *data, uint16_t num);
};

/*����DMAʱ��ʹ�ܱ����ִ���*/
#if defined(DMA1) || defined(DMA2) || defined(DMA3)

/**@brief�Ƿ�ʹ��˫������*/
//#define ENABLE_DOUBLE_BUF

/*DMA���ͻ��� ��Ҫʵ�廯*/
class UART_DMA_TX_C : public DMA_C
{
public:
	void Init_DMA_TX(DMA_TypeDef *DMA, uint32_t DMA_Channelx);
};
/*DMA���ջ��� ��Ҫʵ�廯*/
class UART_DMA_RX_C : public DMA_C
{
public:
	void Init_DMA_RX(DMA_TypeDef *DMA, uint32_t DMA_Channelx);
};
/*
	����DMA����
	ʹ��DMAʱʹ�ô���ʵ�廯
	�̳����������ա����͹���
	����DMA�շ�����

	ʹ��ǰ��ִ��Init����
	��ִ����Ӧ��Init_DMA����
	�����Խ����շ�

	��Ӧ���жϴ���������õ�λ
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