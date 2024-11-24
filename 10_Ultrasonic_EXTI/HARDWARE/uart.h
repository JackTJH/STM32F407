#ifndef __UART_H
#define __UART_H


#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f4xx.h"
#include <stdio.h>

#define UART_CLK_ENABLE()	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE)

#define UART_GPIO_RCC		RCC_AHB1Periph_GPIOA
#define UART_GPIO_Port		GPIOA
#define UART_GPIO_TX_Pin	GPIO_Pin_9
#define UART_GPIO_RX_Pin	GPIO_Pin_10
#define UART_GPIO_AFTXPin	GPIO_PinSource9
#define UART_GPIO_AFRXPin	GPIO_PinSource10
#define UART_GPIO_AFUART	GPIO_AF_USART1
#define UART				USART1

#define NVIC_UART_PREEMPTION_PRIORITY			1
#define NVIC_UART_SUB_PRIORITY					1
#define NVIC_IRQ_UART							USART1_IRQn

void UART_Init(uint32_t baud);
void USART_SendString(USART_TypeDef* USARTx, char *DataString);


#ifdef __cplusplus
}
#endif

#endif
