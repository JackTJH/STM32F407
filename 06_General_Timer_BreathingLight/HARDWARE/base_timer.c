#include "base_timer.h"

void BASE_Timer_Init(uint16_t Pre,uint16_t ARR)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);//�򿪻�����ʱ��ʱ��
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;//�����ֲ����������ʱ��TIM6��TIM7ֻ�����ϼ���
	TIM_TimeBaseInitStructure.TIM_Period = ARR-1;
	TIM_TimeBaseInitStructure.TIM_Prescaler = Pre-1;
	TIM_TimeBaseInit(TIM6,&TIM_TimeBaseInitStructure);
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);//���ж�Դ
	
	//����NVIC
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM6_DAC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStructure);
	TIM_Cmd(TIM6,ENABLE);//�򿪶�ʱ��
}	


	