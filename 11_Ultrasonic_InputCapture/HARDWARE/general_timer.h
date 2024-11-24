#ifndef __GENERAL_TIMER_H
#define __GENERAL_TIMER_H


#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f4xx.h"

//#define GENERAL_TIMER_Input										TIM9
//#define GENERAL_TIMER_Input_RCC								RCC_APB2Periph_TIM9

#define GTIMER_INPUT										TIM3
#define GTIMER_INPUT_RCC								RCC_APB1Periph_TIM3
#define GTIMER_INPUT_PORT_RCC						RCC_AHB1Periph_GPIOA

#define INPUT_CAPTURE_PORT							GPIOA
#define INPUT_CAPTURE_PIN								GPIO_Pin_6

#define INPUT_AF_PIN										GPIO_PinSource6
#define INPUT_AF												GPIO_AF_TIM3

#define NVIC_INPUT_IRQ									TIM3_IRQn
#define NVIC_INPUT_PRE									1
#define NVIC_INPUT_SUB									3

void GeneralTimer_InputCaptureInit(void);
void GTIM_TIMx_Cap_Chy_Init(uint16_t psc,uint32_t arr);



#ifdef __cplusplus
}
#endif

#endif
