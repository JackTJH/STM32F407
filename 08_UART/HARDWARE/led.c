#include "led.h"


GPIO_TypeDef* GPIO_PORT[LEDn] = {CORE_LED_PORT};
const uint16_t GPIO_PIN[LEDn] = {CORE_LED_PIN};

void LED_Init(Led_TypeDef Led)
{
	
	
	GPIO_InitTypeDef  GPIO_InitStructure;
	LEDx_GPIO_CLK_ENABLE(Led);

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN[Led];
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIO_PORT[Led], &GPIO_InitStructure);
}
