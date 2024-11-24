#include "button.h"

GPIO_TypeDef* BUTTON_GPIO_PORT[Button_n] = {CORE_BUTTON_PORT};
const uint16_t BUTTON_GPIO_PIN[Button_n] = {CORE_BUTTON_PIN};

void Button_Init(Button_TypeDef Button)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	BUTTONx_GPIO_CLK_ENABLE(Button);

	GPIO_InitStructure.GPIO_Pin = BUTTON_GPIO_PIN[Button];
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init(BUTTON_GPIO_PORT[Button], &GPIO_InitStructure);
	
}

uint8_t Get_Button_Status(Button_TypeDef Button)
{
	return GPIO_ReadInputDataBit(BUTTON_GPIO_PORT[Button],BUTTON_GPIO_PIN[Button]);
}

