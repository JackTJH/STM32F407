#ifndef __UART_H
#define __UART_H


#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f4xx.h"
#include <stdio.h>

typedef enum
{
	UART_1,
	UART_2
} Uart_TypeDef;

#define UARTn																2

#define UART_CLK_ENABLE()	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE)

#define UART1_GPIO_RCC											RCC_AHB1Periph_GPIOA
#define UART1_RCC														RCC_APB2Periph_USART1
#define UART1_GPIO_Port											GPIOA
#define UART1_GPIO_TX_Pin										GPIO_Pin_9
#define UART1_GPIO_RX_Pin										GPIO_Pin_10
#define UART1_GPIO_AFTXPin									GPIO_PinSource9
#define UART1_GPIO_AFRXPin									GPIO_PinSource10
#define UART1_GPIO_AFUART										GPIO_AF_USART1
#define UART1																USART1

#define NVIC_UART1_PRE											1
#define NVIC_UART1_SUB											2
#define NVIC_IRQ_UART1											USART1_IRQn

#define UART2_GPIO_RCC											RCC_AHB1Periph_GPIOD
#define UART2_RCC														RCC_APB1Periph_USART2
#define UART2_GPIO_Port											GPIOD
#define UART2_GPIO_TX_Pin										GPIO_Pin_5
#define UART2_GPIO_RX_Pin										GPIO_Pin_6
#define UART2_GPIO_AFTXPin									GPIO_PinSource5
#define UART2_GPIO_AFRXPin									GPIO_PinSource6
#define UART2_GPIO_AFUART										GPIO_AF_USART2
#define UART2																USART2

#define NVIC_UART2_PRE											1
#define NVIC_UART2_SUB											3
#define NVIC_IRQ_UART2											USART2_IRQn


#define UART1_GPIO_CLK_ENABLE()							RCC_AHB1PeriphClockCmd(UART1_GPIO_RCC, ENABLE)
#define UART2_GPIO_CLK_ENABLE()							RCC_AHB1PeriphClockCmd(UART2_GPIO_RCC, ENABLE)
#define UARTx_GPIO_CLK_ENABLE(__INDEX__)		do{if((__INDEX__) == 0)	UART1_GPIO_CLK_ENABLE(); else \
																							 if((__INDEX__) == 1) UART2_GPIO_CLK_ENABLE(); \
																							}while(0)

#define UART1_CLK_ENABLE()									RCC_APB2PeriphClockCmd(UART1_RCC, ENABLE)
#define UART2_CLK_ENABLE()									RCC_APB1PeriphClockCmd(UART2_RCC, ENABLE)
#define UARTx_CLK_ENABLE(__INDEX__)					do{if((__INDEX__) == 0)	UART1_CLK_ENABLE(); else \
																							 if((__INDEX__) == 1) UART2_CLK_ENABLE(); \
																							}while(0)



void UARTx_Init(Uart_TypeDef Uart,uint32_t baud);
void USART_SendString(USART_TypeDef* USARTx, uint8_t *DataString);
void USART_PRINT(USART_TypeDef* USARTx,const char *fmt,...);


#ifdef __cplusplus
}
#endif

#endif
