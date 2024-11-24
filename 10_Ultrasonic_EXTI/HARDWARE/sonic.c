#include "sonic.h"
#include "delay.h"
#include "uart.h"
#include "general_timer.h"


void SONIC_GPIO_Init(void)
{
	RCC_AHB1PeriphClockCmd(SONIC_TRIG_RCC_PORT, ENABLE); 
	RCC_AHB1PeriphClockCmd(SONIC_ECHO_RCC_PORT, ENABLE); 
	
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = SONIC_TRIG_Pin|SONIC_ECHO_Pin;

	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(SONIC_TRIG_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init(SONIC_ECHO_PORT, &GPIO_InitStructure);
	
		//配置按键中断
	EXTI_InitTypeDef EXTI_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line2;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_Init(&EXTI_InitStructure);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,EXTI_PinSource2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
}

float HC_SR04_GetDistance1(void)
{
	TRIG(1);
	Delay_us(20);
	TRIG(0);
	
	while(ECHO_Read() == RESET); 
	SysTick->CTRL = 0;
	SysTick->VAL = 0xffffff; 
	SysTick->LOAD = 0xffffff; 
	SysTick->CTRL = SysTick_CTRL_ENABLE_Msk; 
	while(ECHO_Read() == SET); 
	SysTick->CTRL = 0;
	
	return (0xffffff - SysTick->VAL)/21/(1e6)*170*100; //单位cm
}
	
