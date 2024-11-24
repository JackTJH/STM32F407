#include "adc.h"
#include "delay.h"

void Kernal_TempSensor(void)
{
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
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_Init(ADC1,&ADC_InitStructure);
	
	ADC_RegularChannelConfig(ADC1,ADC_Channel_16,1,ADC_SampleTime_56Cycles);
	ADC_TempSensorVrefintCmd(ENABLE);
	ADC_Cmd(ADC1,ENABLE);
	ADC_SoftwareStartConv(ADC1);
}

void Get_Kernal_Tempature(MyADC_Typedef *temp,uint8_t FilterNum)
{
	temp->TempSensor = 0;
	for(uint8_t i= 0; i<FilterNum; i++)
	{
    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)); // �ȴ�ת�����
		temp->TempSensor += ADC_GetConversionValue(ADC1);
	}
	if (FilterNum > 1)
	{
		temp->FilterTempSensor = temp->TempSensor / FilterNum; // ����ƽ��ֵ
	} 
	else 
	{
		temp->FilterTempSensor = temp->TempSensor; // ���β���
	}
	temp->Tempture = (temp->FilterTempSensor/4096.0*3.3-0.76)*1000/2.5+25;

}

uint16_t Get_Adc(ADC_TypeDef* ADCx,uint8_t channelx)
{
	ADC_SoftwareStartConv(ADCx);//ʹ��ָ����ADC1�����ת����������
	while(!ADC_GetFlagStatus(ADCx,ADC_FLAG_EOC));//��ȡ״̬�Ĵ�����״̬λEOC���ȴ�ת������
	return ADC_GetConversionValue(ADCx);//�������һ�ε�ADC1�������ת�����
}

uint16_t Get_Adc_Average(ADC_TypeDef* ADCx,uint8_t channelx,uint8_t FilterNum)
{
	uint32_t temp_val = 0;
	uint8_t t;
	for(t = 0; t<FilterNum; t++)
	{
		temp_val += Get_Adc(ADCx,channelx);
//		Delay_ms(5);
	}
	return temp_val/FilterNum;
} 



