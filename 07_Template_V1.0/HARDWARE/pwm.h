#ifndef __PWM_H
#define __PWM_H


#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f4xx.h"


#define PWM_GPIO_LedRCC		RCC_AHB1Periph_GPIOA
#define PWM_GPIO_LedPort	GPIOA
#define PWM_GPIO_LedPin		GPIO_Pin_5
#define PWM_GPIO_LedAFPin	GPIO_PinSource5
#define PWM_GPIO_LedAFTime	GPIO_AF_TIM2

#define PWM_TIM_RCC			RCC_APB1Periph_TIM2
#define PWM_TIM				TIM2

void PWM_Config_Init(uint16_t psc,uint32_t arr);
	
	
	
#ifdef __cplusplus
}
#endif

#endif
