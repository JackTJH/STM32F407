#ifndef __SONIC_H
#define __SONIC_H


#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f4xx.h"

#define SONIC_TRIG_RCC_PORT					RCC_AHB1Periph_GPIOA
#define SONIC_ECHO_RCC_PORT					RCC_AHB1Periph_GPIOB

#define SONIC_TRIG_Pin							GPIO_Pin_1
#define SONIC_ECHO_Pin							GPIO_Pin_3

#define SONIC_TRIG_PORT							GPIOA
#define SONIC_ECHO_PORT							GPIOA


#define TRIG(bit)										GPIO_WriteBit(SONIC_TRIG_PORT,SONIC_TRIG_Pin,(bit)?Bit_SET:Bit_RESET)
#define ECHO_Read() 								GPIO_ReadInputDataBit(SONIC_ECHO_PORT, SONIC_ECHO_Pin)



void SONIC_GPIO_Init(void);
void HC_SR04_Start(void);
float HC_SR04_GetDistance1(void);
	 
	 
	 
#ifdef __cplusplus
}
#endif

#endif	
