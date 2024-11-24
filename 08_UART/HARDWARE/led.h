#ifndef __LED_H
#define __LED_H


#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f4xx.h"

typedef enum
{
	CORE_LED,
} Led_TypeDef;

#define LEDn                             1

#define CORE_LED_PIN                    GPIO_Pin_2
#define CORE_LED_PORT                   GPIOB
#define CORE_LED_GPIO_RCC_PORT			RCC_AHB1Periph_GPIOB
#define CORE_LED_GPIO_CLK_ENABLE()		RCC_AHB1PeriphClockCmd(CORE_LED_GPIO_RCC_PORT, ENABLE); 

#define LEDx_GPIO_CLK_ENABLE(__INDEX__) do{if((__INDEX__) == 0) CORE_LED_GPIO_CLK_ENABLE(); \
                                           }while(0)

void LED_Init(Led_TypeDef Led);
	 
	 
#ifdef __cplusplus
}
#endif

#endif
