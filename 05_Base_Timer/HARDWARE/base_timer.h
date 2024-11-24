#ifndef __BASE_TIMER_H
#define __BASE_TIMER_H


#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f4xx.h"

void BASE_Timer_Init(uint16_t Pre,uint16_t ARR);


#ifdef __cplusplus
}
#endif

#endif
