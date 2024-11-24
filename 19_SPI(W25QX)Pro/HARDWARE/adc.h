#ifndef __ADC_H
#define __ADC_H

#ifdef __cplusplus
	extern "C" {
#endif

#include "stm32f4xx.h"

#define ADC_NUM	2

#define ADCX													ADC1

#define ADC10_PIN                    	GPIO_Pin_3
#define ADC10_PORT                   	GPIOA
#define ADC10_GPIO_RCC_PORT						RCC_AHB1Periph_GPIOA

#define	ADCX_DMA_RCC									RCC_AHB1Periph_DMA2
#define ADCX_DMA_Channel							DMA_Channel_0
#define ADCX_DMA_PeripheralBaseAddr		((uint32_t)ADCX+0x4C)
#define ADCX_DMA_Memory0BaseAddr			ADC_Temp.AdcValue

typedef struct
{
	uint16_t TempSensor;
	uint16_t FilterTempSensor;
	float Tempture;
	uint16_t AdcValue[ADC_NUM];
} MyADC_Typedef;

void MYADC_Init(void);
void Get_Kernal_Tempature(MyADC_Typedef *temp,uint8_t samples);





#ifdef __cplusplus
	}
#endif

#endif
