#include "general_timer.h"

void TIM_IC_MspInit(void)
{
	RCC_AHB1PeriphClockCmd(GTIMER_INPUT_PORT_RCC, ENABLE); 
	RCC_APB1PeriphClockCmd(GTIMER_INPUT_RCC,ENABLE);
	
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = INPUT_CAPTURE_PIN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(INPUT_CAPTURE_PORT, &GPIO_InitStructure);
	GPIO_PinAFConfig(INPUT_CAPTURE_PORT, INPUT_AF_PIN, INPUT_AF);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = NVIC_INPUT_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = NVIC_INPUT_PRE;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = NVIC_INPUT_SUB;
	NVIC_Init(&NVIC_InitStructure);
}

void GTIM_TIMx_Cap_Chy_Init(uint16_t psc,uint32_t arr)
{	
	TIM_IC_MspInit();
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;//根据手册这里基本定时器TIM6和TIM7只能向上计数
	TIM_TimeBaseInitStructure.TIM_Period = arr-1;
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc-1;
	TIM_TimeBaseInit(GTIMER_INPUT,&TIM_TimeBaseInitStructure);	
	
	TIM_ICInitTypeDef TIM_ICInitStructure;
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICFilter = 0X00;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInit(GTIMER_INPUT,&TIM_ICInitStructure);
		
	TIM_ITConfig(GTIMER_INPUT,TIM_IT_Update|TIM_IT_CC1,ENABLE);//打开中断源
	TIM_Cmd(GTIMER_INPUT,ENABLE);	
}
