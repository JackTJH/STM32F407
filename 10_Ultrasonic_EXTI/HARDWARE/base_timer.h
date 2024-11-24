#ifndef __BASE_TIMER_H
#define __BASE_TIMER_H


#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f4xx.h"

typedef enum
{
    BASE_1MS = 0u,
    BASE_10MS,
    BASE_20MS,
    BASE_50MS,
    BASE_100MS,
    BASE_200MS,
    BASE_500MS,
    BASE_1000MS,
    BASE_NUM,
}time_base_e;
	 
#define BASE_TIMER														TIM6
#define BASE_TIMER_RCC												RCC_APB1Periph_TIM6
#define BASE_TIMER7														TIM7
#define BASE_TIMER7_RCC												RCC_APB1Periph_TIM7

#define NVIC_BASE_TIMER_PREEMPTION_PRIORITY		1
#define NVIC_BASE_TIMER_SUB_PRIORITY					0
#define NVIC_IRQ_BASE_TIMER										TIM6_DAC_IRQn

#define NVIC_BASE_TIMER7_PREEMPTION_PRIORITY	1
#define NVIC_BASE_TIMER7_SUB_PRIORITY					0
#define NVIC_IRQ_BASE_TIMER7									TIM7_IRQn


void BASE_Timer_Init(uint16_t Pre,uint16_t ARR);

uint8_t TimeBaseGetFlag(time_base_e base);
void TimeBaseSetFlag(time_base_e base);
void TimeBaseClearFlag(time_base_e base);
void TimeBaseUpdata(void);


void BASE_Timer7_Init(void);

#ifdef __cplusplus
}
#endif

#endif
