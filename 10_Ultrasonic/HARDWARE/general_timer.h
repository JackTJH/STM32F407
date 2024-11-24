#ifndef __GENERAL_TIMER_H
#define __GENERAL_TIMER_H


#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f4xx.h"

#define GENERAL_TIMER_Input										TIM9
#define GENERAL_TIMER_Input_RCC								RCC_APB2Periph_TIM9

void GeneralTimer_InputCaptureInit(void);



#ifdef __cplusplus
}
#endif

#endif
