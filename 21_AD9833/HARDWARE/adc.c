#include "adc.h"
#include "delay.h"

//extern MyADC_Typedef ADC_Temp;

void ADC_GPIO_Init(void)
{
	RCC_AHB1PeriphClockCmd(ADC10_GPIO_RCC_PORT,ENABLE);
	
	GPIO_InitTypeDef  GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = ADC10_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(ADC10_PORT, &GPIO_InitStructure);
}

void DMA_Control_Init(void)
{
	RCC_AHB1PeriphClockCmd(ADCX_DMA_RCC,ENABLE);
	
	DMA_InitTypeDef	DMA_InitStructure;
	DMA_InitStructure.DMA_BufferSize = ADC_NUM;
	DMA_InitStructure.DMA_Channel = ADCX_DMA_Channel;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
//	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
//	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
//	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)ADCX_DMA_Memory0BaseAddr;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADCX_DMA_PeripheralBaseAddr;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_Init(DMA2_Stream0,&DMA_InitStructure);
	DMA_Cmd(DMA2_Stream0,ENABLE);
}

void MYADC_Init(void)
{
	ADC_GPIO_Init();
	DMA_Control_Init();
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;		//84/4 = 21MHZ
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInit(&ADC_CommonInitStructure);
	
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_NbrOfConversion = ADC_NUM;
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_Init(ADCX,&ADC_InitStructure);
	
	ADC_RegularChannelConfig(ADCX,ADC_Channel_16,1,ADC_SampleTime_56Cycles);
	ADC_RegularChannelConfig(ADCX,ADC_Channel_3,2,ADC_SampleTime_56Cycles);
	ADC_TempSensorVrefintCmd(ENABLE);
	ADC_DMARequestAfterLastTransferCmd(ADCX,ENABLE);
	ADC_DMACmd(ADCX,ENABLE);
	ADC_Cmd(ADCX,ENABLE);
	ADC_SoftwareStartConv(ADCX);
}

void Get_Kernal_Tempature(MyADC_Typedef *temp,uint8_t FilterNum)
{
	temp->TempSensor = 0;
	for(uint8_t i= 0; i<FilterNum; i++)
	{
		temp->TempSensor += temp->AdcValue[0];
	}
	if (FilterNum > 1)
	{
		temp->FilterTempSensor = temp->TempSensor / FilterNum; // 计算平均值
	} 
	else 
	{
		temp->FilterTempSensor = temp->TempSensor; // 单次采样
	}
	temp->Tempture = (temp->FilterTempSensor/4096.0*3.3-0.76)*1000/2.5+25;

}



