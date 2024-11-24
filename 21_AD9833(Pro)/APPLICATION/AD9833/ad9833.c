#include "ad9833.h"
#include "delay.h"

AD9833_Typedef AD9833_1;

/**********************************************************************************************
** �������� ��unsigned char AD9833_SPI_Write(unsigned char* data,unsigned char bytesNumber)
** �������� ��ʹ��ģ��SPI��AD9833д����
** ��ڲ��� ��* data:д�����ݻ�����,��һ���ֽ��ǼĴ�����ַ���ڶ����ֽڿ�ʼҪд������ݡ�
						bytesNumber: Ҫд����ֽ���
** ���ڲ��� ����
** ����˵�� ����
************************************************************************************************/
uint8_t AD9833_SPI_Write(uint8_t* data,uint8_t bytesNumber,SPI_TYPE spi_type,SPIX_Typedef spix)
{
  uint8_t i,j; 
	uint8_t writeData[5]	= {0,0, 0, 0, 0};
	
	if(spix == JSPI1)
	{
		SPI1_SCK(1);
	}
	else if(spix == JSPI2)
	{
		SPI2_SCK(1);
	}
	
	SPIX_CS(0,spix);

	for(i = 0; i < bytesNumber; i++)
	{
		writeData[i] = data[i + 1];
	}
	
	for(i=0; i<bytesNumber; i++) 
	{
		SPI_SendByte(writeData[i],spi_type,spix); 	
	}
	if(spix == JSPI1)
	{
		SPI1_MOSI(1); 
	}
	else if(spix == JSPI2)
	{
		SPI2_MOSI(1); 
	}
	SPIX_CS(1,spix);
	return i;
}

/*****************************************************************************************
** �������� ��void AD9833_SetRegisterValue(unsigned short regValue)
** �������� ����ֵд��Ĵ���
** ��ڲ��� ��regValue��Ҫд��Ĵ�����ֵ��
** ���ڲ��� ����
** ����˵�� ����
*******************************************************************************************/
void AD9833_SetRegisterValue(unsigned short regValue,SPI_TYPE spi_type,SPIX_Typedef spix)
{
	uint8_t data[5] = {0x03, 0x00, 0x00};	
	
	data[1] = (uint8_t)((regValue & 0xFF00) >> 8);
	data[2] = (uint8_t)((regValue & 0x00FF) >> 0);
	AD9833_SPI_Write(data,2,spi_type,spix);
}

/*****************************************************************************************
** �������� ��void AD9833_Reset(void)  
** �������� ������AD9833�ĸ�λλ
** ��ڲ��� ����
** ���ڲ��� ����
** ����˵�� ����
*******************************************************************************************/
void AD9833_Reset(SPI_TYPE spi_type,SPIX_Typedef spix)
{
	AD9833_SetRegisterValue(AD9833_REG_CMD | AD9833_RESET,spi_type,spix);
	Delay_ms(10);
}

/*****************************************************************************************
** �������� ��void AD9833_ClearReset(void)  
** �������� �����AD9833�ĸ�λλ��
** ��ڲ��� ����
** ���ڲ��� ����
** ����˵�� ����
*******************************************************************************************/
void AD9833_ClearReset(SPI_TYPE spi_type,SPIX_Typedef spix)
{
	AD9833_SetRegisterValue(AD9833_REG_CMD,spi_type,spix);
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
void AD9833_SetFrequency(unsigned short reg, float fout,unsigned short type,SPI_TYPE spi_type,SPIX_Typedef spix)
{
	unsigned short freqHi = reg;
	unsigned short freqLo = reg;
	unsigned long val=RealFreDat*fout;
	freqHi |= (val & 0xFFFC000) >> 14 ;
	freqLo |= (val & 0x3FFF);
	AD9833_SetRegisterValue(AD9833_B28|type,spi_type,spix);
	AD9833_SetRegisterValue(freqLo,spi_type,spix);
	AD9833_SetRegisterValue(freqHi,spi_type,spix);
}

/*****************************************************************************************
** �������� ��void AD9833_SetFrequencyQuick(float fout,unsigned short type)
** �������� ��д��Ƶ�ʼĴ���
** ��ڲ��� ��val��Ҫд���Ƶ��ֵ��
**						type���������ͣ�AD9833_OUT_SINUS���Ҳ���AD9833_OUT_TRIANGLE���ǲ���AD9833_OUT_MSB����
** ���ڲ��� ����
** ����˵�� ��ʱ������Ϊ25 MHzʱ�� ����ʵ��0.1 Hz�ķֱ��ʣ���ʱ������Ϊ1 MHzʱ�������ʵ��0.004 Hz�ķֱ��ʡ�
*******************************************************************************************/
void AD9833_SetFrequencyQuick(float fout,unsigned short type,SPI_TYPE spi_type,SPIX_Typedef spix)
{
	AD9833_SetFrequency(AD9833_REG_FREQ0, fout,type,spi_type,spix);
}
/*****************************************************************************************
** �������� ��void AD9833_SetPhase(unsigned short reg, unsigned short val)
** �������� ��д����λ�Ĵ�����
** ��ڲ��� ��reg��Ҫд�����λ�Ĵ�����--��AD9833_REG_PHASE0��AD9833_REG_PHASE1
**						val��Ҫд���ֵ������180����λ����Χ��0-4095(0-360��)
** ���ڲ��� ����
** ����˵�� ����
*******************************************************************************************/
void AD9833_SetPhase(unsigned short reg, unsigned short val,SPI_TYPE spi_type,SPIX_Typedef spix)
{
	unsigned short phase = reg;
	phase |= val;
	AD9833_SetRegisterValue(phase,spi_type,spix);
}

/*****************************************************************************************
** �������� ��void AD9833_Setup(unsigned short freq, unsigned short phase,unsigned short type)
** �������� ��д����λ�Ĵ�����
** ��ڲ��� ��freq��ʹ�õ�Ƶ�ʼĴ�����
							phase��ʹ�õ���λ�Ĵ�����
							type��Ҫ����Ĳ������͡�
** ���ڲ��� ����
** ����˵�� ����
*******************************************************************************************/
void AD9833_Setup(unsigned short freq, unsigned short phase,unsigned short type,SPI_TYPE spi_type,SPIX_Typedef spix)
{
	unsigned short val = 0;
	
	val = freq | phase | type;
	AD9833_SetRegisterValue(val,spi_type,spix);
}

/*****************************************************************************************
** �������� ��void AD9833_SetWave(unsigned short type)
** �������� ������Ҫ����Ĳ������͡�
** ��ڲ��� ��type��Ҫ����Ĳ������͡�
** ���ڲ��� ����
** ����˵�� ����
*******************************************************************************************/
void AD9833_SetWave(unsigned short type,SPI_TYPE spi_type,SPIX_Typedef spix)
{
	AD9833_SetRegisterValue(type,spi_type,spix);
}


