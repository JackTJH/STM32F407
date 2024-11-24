#ifndef __KEY_H
#define __KEY_H


#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f4xx.h"
#include "button.h"

#define Button_n                          	1

#define CORE_BUTTON_PIN                   	GPIO_Pin_0
#define CORE_BUTTON_PORT                  	GPIOA
#define CORE_BUTTON_GPIO_RCC_PORT						RCC_AHB1Periph_GPIOA
#define CORE_BUTTON_GPIO_CLK_ENABLE()				RCC_AHB1PeriphClockCmd(CORE_BUTTON_GPIO_RCC_PORT, ENABLE); 

#define BUTTONx_GPIO_CLK_ENABLE(__INDEX__) 	do{if((__INDEX__) == 0) CORE_BUTTON_GPIO_CLK_ENABLE(); \
																						}while(0)

void key_setup(void);

typedef enum
{
	CORE_BUTTON,
} Button_TypeDef;

void Button_Init(Button_TypeDef Button);			


#ifdef __cplusplus
}
#endif

#endif
