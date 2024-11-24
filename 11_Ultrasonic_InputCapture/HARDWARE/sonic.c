#include "sonic.h"
#include "delay.h"
#include "uart.h"
#include "general_timer.h"


void SONIC_GPIO_Init(void)
{
	RCC_AHB1PeriphClockCmd(SONIC_TRIG_RCC_PORT, ENABLE); 
	
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = SONIC_TRIG_Pin;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(SONIC_TRIG_PORT, &GPIO_InitStructure);
}

void HC_SR04_Start(void)
{
	TRIG(1);
	Delay_us(20);
	TRIG(0);
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
	
	return (0xffffff - SysTick->VAL)/21/(1e6)*170*100; //µ¥Î»cm
}
	
