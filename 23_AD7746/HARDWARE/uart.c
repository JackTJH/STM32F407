#include "uart.h"
#include <stdarg.h>
  
int fputc(int ch, FILE *f)
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(UART1, (uint8_t) ch);

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(UART1, USART_FLAG_TXE) == RESET)
  {}

  return ch;
}

GPIO_TypeDef* UARTx_GPIO_PORT[UARTn] = {UART1_GPIO_Port, 
																				UART2_GPIO_Port};

USART_TypeDef* USARTx[UARTn] = {UART1,
																UART2};

const uint8_t GPIOx_AF[UARTn]	= {UART1_GPIO_AFUART,
																 UART2_GPIO_AFUART};	

const uint8_t NVIC_UARTx_IRQ[UARTn] = {NVIC_IRQ_UART1,
																			 NVIC_IRQ_UART2}; 

const uint8_t NVIC_UARTx_Pre[UARTn] = {NVIC_UART1_PRE,
																	     NVIC_UART2_PRE}; 
const uint8_t NVIC_UARTx_Sub[UARTn] = {NVIC_UART1_SUB,
																	     NVIC_UART2_SUB};

void UARTx_Init(Uart_TypeDef Uart,uint32_t baud)
{
	UARTx_GPIO_CLK_ENABLE(Uart);
	UARTx_CLK_ENABLE(Uart);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	if(Uart == UART_1)
	{
		GPIO_InitStructure.GPIO_Pin = UART1_GPIO_TX_Pin|UART1_GPIO_RX_Pin;
	}
	else if(Uart == UART_2)
	{
		GPIO_InitStructure.GPIO_Pin = UART2_GPIO_TX_Pin|UART2_GPIO_RX_Pin;
	}
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(UARTx_GPIO_PORT[Uart], &GPIO_InitStructure); 

	if(Uart == UART_1)
	{
		GPIO_PinAFConfig(UARTx_GPIO_PORT[Uart], UART1_GPIO_AFTXPin, GPIOx_AF[Uart]);
		GPIO_PinAFConfig(UARTx_GPIO_PORT[Uart], UART1_GPIO_AFRXPin, GPIOx_AF[Uart]);
	}
	else if(Uart == UART_2)
	{
		GPIO_PinAFConfig(UARTx_GPIO_PORT[Uart], UART2_GPIO_AFTXPin, GPIOx_AF[Uart]);
		GPIO_PinAFConfig(UARTx_GPIO_PORT[Uart], UART2_GPIO_AFRXPin, GPIOx_AF[Uart]);
	}
	
	USART_InitTypeDef USART_InitStructure;
	if(Uart == UART_1)
	{
		USART_InitStructure.USART_BaudRate = 115200;
	}
	else if(Uart == UART_2)
	{
		USART_InitStructure.USART_BaudRate = 9600;
	}
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USARTx[Uart],&USART_InitStructure);
	
	if(Uart == UART_2)
	{
		USART_ITConfig(USARTx[Uart],USART_IT_RXNE,ENABLE);
	
		NVIC_InitTypeDef NVIC_InitStructure;
		NVIC_InitStructure.NVIC_IRQChannel = NVIC_UARTx_IRQ[Uart];
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = NVIC_UARTx_Pre[Uart];
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = NVIC_UARTx_Sub[Uart];
		NVIC_Init(&NVIC_InitStructure);
	}
	
	USART_Cmd(USARTx[Uart],ENABLE);
}

void USART_SendString(USART_TypeDef* USARTx, uint8_t *DataString)
{
//	int i = 0;
//	USART_ClearFlag(USARTx,USART_FLAG_TXE);										//发送字符前清空标志位（否则缺失字符串的第一个字符）
//	while(DataString[i] != '\0')												//字符串结束符
//	{
//		USART_SendData(USARTx,DataString[i]);									//每次发送字符串的一个字符
//		while(USART_GetFlagStatus(USARTx,USART_FLAG_TXE) == 0);					//等待数据发送成功
//		USART_ClearFlag(USARTx,USART_FLAG_TXE);									//发送字符后清空标志位
//		i++;
//	}
	
	uint8_t *p = DataString;
	USART_ClearFlag(USARTx,USART_FLAG_TXE);
	while(p && *p)
	{
		USART_SendData(USARTx,*p++);
		while(USART_GetFlagStatus(USARTx,USART_FLAG_TXE) == 0);	
		USART_ClearFlag(USARTx,USART_FLAG_TXE);		
	}
}

void USART_PRINT(USART_TypeDef* USARTx,const char *fmt,...)
{
	static uint8_t USART_Send_Buf[255] = {0};
	static va_list ap;
	
	va_start(ap,fmt);
	vsprintf((char*)USART_Send_Buf,fmt,ap);
	va_end(ap);
	
	USART_SendString(USARTx,USART_Send_Buf);
}


