#include "uart.h"
  
int fputc(int ch, FILE *f)
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(UART, (uint8_t) ch);

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(UART, USART_FLAG_TC) == RESET)
  {}

  return ch;
}

void UART_Init(uint32_t baud)
{
	RCC_AHB1PeriphClockCmd(UART_GPIO_RCC, ENABLE);
	UART_CLK_ENABLE();
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = UART_GPIO_TX_Pin|UART_GPIO_RX_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(UART_GPIO_Port, &GPIO_InitStructure); 

	GPIO_PinAFConfig(UART_GPIO_Port, UART_GPIO_AFTXPin, UART_GPIO_AFUART);
	GPIO_PinAFConfig(UART_GPIO_Port, UART_GPIO_AFRXPin, UART_GPIO_AFUART);
	
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = baud;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(UART,&USART_InitStructure);
	
	USART_ITConfig(UART,USART_IT_RXNE,ENABLE);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = NVIC_IRQ_UART;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = NVIC_UART_PREEMPTION_PRIORITY;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = NVIC_UART_SUB_PRIORITY;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_Cmd(UART,ENABLE);
}

void USART_SendString(USART_TypeDef* USARTx, char *DataString)
{
	int i = 0;
	USART_ClearFlag(USARTx,USART_FLAG_TXE);										//发送字符前清空标志位（否则缺失字符串的第一个字符）
	while(DataString[i] != '\0')												//字符串结束符
	{
		USART_SendData(USARTx,DataString[i]);									//每次发送字符串的一个字符
		while(USART_GetFlagStatus(USARTx,USART_FLAG_TXE) == 0);					//等待数据发送成功
		USART_ClearFlag(USARTx,USART_FLAG_TXE);									//发送字符后清空标志位
		i++;
	}
}

