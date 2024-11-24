#include "ad9833.h"
#include "delay.h"


/**********************************************************************************************
** �������� ��unsigned char AD9833_SPI_Write(unsigned char* data,unsigned char bytesNumber)
** �������� ��ʹ��ģ��SPI��AD9833д����
** ��ڲ��� ��* data:д�����ݻ�����,��һ���ֽ��ǼĴ�����ַ���ڶ����ֽڿ�ʼҪд������ݡ�
						bytesNumber: Ҫд����ֽ���
** ���ڲ��� ����
** ����˵�� ����
************************************************************************************************/
uint8_t AD9833_SPI_Write(uint8_t* data,uint8_t bytesNumber)
{
  uint8_t i,j; 
	uint8_t writeData[5]	= {0,0, 0, 0, 0};
	
	SPI1_SCK(1);
	SPI1_CS(0);

	for(i = 0; i < bytesNumber; i++)
	{
		writeData[i] = data[i + 1];
	}
	
	for(i=0; i<bytesNumber; i++) 
	{
    for(j=0; j<8; j++)      
		{ 
			if(writeData[i]&0x80) 
			{
				SPI1_MOSI(1); 
			}
			else 
			{
				SPI1_MOSI(0); 
			}
			Delay_us(5);
			SPI1_SCK(0);
			Delay_us(5);
      writeData[i] <<= 1; 
			SPI1_SCK(1);
			Delay_us(5);
    } 
		
	}
	SPI1_MOSI(1); 
	SPI1_CS(1);
	return i;
}

/*****************************************************************************************
** �������� ��void AD9833_SetRegisterValue(unsigned short regValue)
** �������� ����ֵд��Ĵ���
** ��ڲ��� ��regValue��Ҫд��Ĵ�����ֵ��
** ���ڲ��� ����
** ����˵�� ����
*******************************************************************************************/
void AD9833_SetRegisterValue(unsigned short regValue)
{
	uint8_t data[5] = {0x03, 0x00, 0x00};	
	
	data[1] = (uint8_t)((regValue & 0xFF00) >> 8);
	data[2] = (uint8_t)((regValue & 0x00FF) >> 0);
	AD9833_SPI_Write(data,2);
}

/*****************************************************************************************
** �������� ��void AD9833_SetFrequency(unsigned short reg, float fout,unsigned short type)
** �������� ��д��Ƶ�ʼĴ���
** ��ڲ��� ��reg��Ҫд���Ƶ�ʼĴ�����
**						val��Ҫд���ֵ��
**						type���������ͣ�AD9833_OUT_SINUS���Ҳ���AD9833_OUT_TRIANGLE���ǲ���AD9833_OUT_MSB����
** ���ڲ��� ����
** ����˵�� ����
*******************************************************************************************/
void AD9833_SetFrequency(unsigned short reg, float fout,unsigned short type)
{
	unsigned short freqHi = reg;
	unsigned short freqLo = reg;
	unsigned long val=RealFreDat*fout;
	freqHi |= (val & 0xFFFC000) >> 14 ;
	freqLo |= (val & 0x3FFF);
	AD9833_SetRegisterValue(AD9833_B28|type);
	AD9833_SetRegisterValue(freqLo);
	AD9833_SetRegisterValue(freqHi);
}

/*****************************************************************************************
** �������� ��void AD9833_SetFrequencyQuick(float fout,unsigned short type)
** �������� ��д��Ƶ�ʼĴ���
** ��ڲ��� ��val��Ҫд���Ƶ��ֵ��
**						type���������ͣ�AD9833_OUT_SINUS���Ҳ���AD9833_OUT_TRIANGLE���ǲ���AD9833_OUT_MSB����
** ���ڲ��� ����
** ����˵�� ��ʱ������Ϊ25 MHzʱ�� ����ʵ��0.1 Hz�ķֱ��ʣ���ʱ������Ϊ1 MHzʱ�������ʵ��0.004 Hz�ķֱ��ʡ�
*******************************************************************************************/
void AD9833_SetFrequencyQuick(float fout,unsigned short type)
{
	AD9833_SetFrequency(AD9833_REG_FREQ0, fout,type);
}

