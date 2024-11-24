#ifndef __BASE_TIMER_H
#define __BASE_TIMER_H


#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f4xx.h"
	 
#define BASE_TIMER								TIM6
#define BASE_TIMER_RCC							RCC_APB1Periph_TIM6

#define NVIC_BASE_TIMER_PREEMPTION_PRIORITY		1
#define NVIC_BASE_TIMER_SUB_PRIORITY			0
#define NVIC_IRQ_BASE_TIMER						TIM6_DAC_IRQn


void BASE_Timer_Init(uint16_t Pre,uint16_t ARR);


#ifdef __cplusplus
}
#endif

#endif
