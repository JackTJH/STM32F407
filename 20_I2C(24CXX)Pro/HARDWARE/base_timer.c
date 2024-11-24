#include "base_timer.h"
#include "button.h"

static volatile uint16_t base_time_flag = 0;
static volatile uint32_t base_counter = 0;
static volatile uint8_t base_index = 0;
static const uint32_t base_time_div[(uint8_t)BASE_NUM] = {1,10,20,40,50,100,200,500,1000};

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

uint16_t TimeBaseGetFlag(time_base_e base){
    return (base_time_flag & (1<<(uint8_t)base));
}

void TimeBaseSetFlag(time_base_e base){
    base_time_flag |= (1u<<(uint8_t)base);
}

void TimeBaseClearFlag(time_base_e base){
    base_time_flag &= ~(1u<<(uint8_t)base);
}

void TimeBaseUpdata(void){
    base_counter++;
    for(base_index = 0; base_index < (uint8_t)BASE_NUM; base_index++)
		{
        if(base_counter % base_time_div[base_index] == 0)
				{
            TimeBaseSetFlag((time_base_e)base_index);
						if(base_index == (uint8_t)BASE_10MS)
						{
							button_process();
						}
        }
    }
}
