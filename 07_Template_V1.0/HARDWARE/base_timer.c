#include "base_timer.h"

void BASE_Timer_Init(uint16_t Pre,uint16_t ARR)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	
	RCC_APB1PeriphClockCmd(BASE_TIMER_RCC,ENABLE);//打开基本定时器时钟
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;//根据手册这里基本定时器TIM6和TIM7只能向上计数
	TIM_TimeBaseInitStructure.TIM_Period = ARR-1;
	TIM_TimeBaseInitStructure.TIM_Prescaler = Pre-1;
	TIM_TimeBaseInit(BASE_TIMER,&TIM_TimeBaseInitStructure);
	TIM_ITConfig(BASE_TIMER,TIM_IT_Update,ENABLE);//打开中断源
	
	//配置NVIC
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = NVIC_IRQ_BASE_TIMER;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = NVIC_BASE_TIMER_PREEMPTION_PRIORITY;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = NVIC_BASE_TIMER_SUB_PRIORITY;
	NVIC_Init(&NVIC_InitStructure);
	TIM_Cmd(BASE_TIMER,ENABLE);//打开定时器
}	
