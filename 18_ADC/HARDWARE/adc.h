#ifndef __ADC_H
#define __ADC_H

#ifdef __cplusplus
	extern "C" {
#endif

#include "stm32f4xx.h"

typedef struct
{
	uint16_t TempSensor;
	uint16_t FilterTempSensor;
	float Tempture;
} MyADC_Typedef;

void Kernal_TempSensor(void);
void Get_Kernal_Tempature(MyADC_Typedef *temp,uint8_t samples);

uint16_t Get_Adc(ADC_TypeDef* ADCx,uint8_t channelx);
uint16_t Get_Adc_Average(ADC_TypeDef* ADCx,uint8_t channelx,uint8_t FilterNum);



#ifdef __cplusplus
	}
#endif

#endif
