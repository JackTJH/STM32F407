#ifndef __WWDG_H
#define __WWDG_H

#ifdef	__cplusplus
	extern "C"{
#endif

#include "stm32f4xx.h"
		
typedef struct
{
	volatile uint8_t Cnt;
} WWDG_Cnt_Typedef;

//#define WWDG_ON	

void WWDG_Init(uint8_t IsOnInterrupt);


#ifdef __cplusplus
{
#endif

#endif
