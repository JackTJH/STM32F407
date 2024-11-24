#include "base_timer.h"

void BASE_Timer_Init(uint16_t Pre,uint16_t ARR)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	
	RCC_APB1PeriphClockCmd(BASE_TIMER_RCC,ENABLE);//�򿪻�����ʱ��ʱ��
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;//�����ֲ����������ʱ��TIM6��TIM7ֻ�����ϼ���
	TIM_TimeBaseInitStructure.TIM_Period = ARR-1;
	TIM_TimeBaseInitStructure.TIM_Prescaler = Pre-1;
	TIM_TimeBaseInit(BASE_TIMER,&TIM_TimeBaseInitStructure);
	TIM_ITConfig(BASE_TIMER,TIM_IT_Update,ENABLE);//���ж�Դ
	
	//����NVIC
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = NVIC_IRQ_BASE_TIMER;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = NVIC_BASE_TIMER_PREEMPTION_PRIORITY;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = NVIC_BASE_TIMER_SUB_PRIORITY;
	NVIC_Init(&NVIC_InitStructure);
	TIM_Cmd(BASE_TIMER,ENABLE);//�򿪶�ʱ��
}	
