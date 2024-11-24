#include "stm32f4xx.h"
#include "base_timer.h"
#include "pwm.h"
#include "delay.h"
#include "led.h"
#include "uart.h"
#include <stdbool.h>

void Key_Init(void)
{
//	GPIO_InitTypeDef  GPIO_InitStructure;
//	
//	/* GPIOG Peripheral clock enable */
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

//	/* Configure PG6 and PG8 in output pushpull mode */
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
//	RCC_AHB1ENR |= (1<<0);
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
//	GPIOA->MODER &= ~(0<<0);
//	GPIOA_MODER &= ~(0<<0);
	
	
	//配置按键中断
	EXTI_InitTypeDef EXTI_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_Init(&EXTI_InitStructure);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,EXTI_PinSource0);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);

}

void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0) == SET)
	{
		GPIO_ToggleBits(GPIOB,GPIO_Pin_2);
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
}


__IO uint16_t Timer_Cnt1s;
bool PWM_Dir = 0;

void TIM6_DAC_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM6,TIM_IT_Update) ==SET)
	{
		if(++Timer_Cnt1s >= 1000)
		{
//			printf("1s is arrive\r\n");
			Timer_Cnt1s = 0;
		}
		TIM_ClearITPendingBit(TIM6,TIM_IT_Update);
	}

}

void USART1_IRQHandler(void)
{
	uint16_t ReceiveData = 0;
	
	if(USART_GetITStatus(UART,USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(UART,USART_IT_RXNE);
		ReceiveData = USART_ReceiveData(UART);
		USART_SendData(UART,ReceiveData);
		while(USART_GetFlagStatus(UART,USART_FLAG_TXE) == RESET);
	}
}

int main(int argc,char *argv[])
{

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);	//中断优先级分组
	LED_Init(CORE_LED);								//初始化核心板LED
	Key_Init();										//按键初始化
	System_Tick_Init();								//滴答定时器初始化
	BASE_Timer_Init(84,1000);						//基本定时器初始化,这里参数配置的1ms中断
	PWM_Config_Init(84,100);						//方波的频率为10k
	UART_Init(115200);								//串口初始化UART,PA9-PA10
	printf("All Initial is OK\r\n");
	while(1)
	{
		
	}
}




