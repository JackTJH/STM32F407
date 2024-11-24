#ifndef __DELAY_H
#define __DELAY_H


#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f4xx.h"

void System_Tick_Init(void);

void Delay_us(uint32_t nus);
void Delay_ms(uint32_t nms);
	 
	 
#ifdef __cplusplus
}
#endif

#endif

