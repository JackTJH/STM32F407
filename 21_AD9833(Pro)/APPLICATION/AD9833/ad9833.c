#include "ad9833.h"
#include "delay.h"

AD9833_Typedef AD9833_1;

/**********************************************************************************************
** 函数名称 ：unsigned char AD9833_SPI_Write(unsigned char* data,unsigned char bytesNumber)
** 函数功能 ：使用模拟SPI向AD9833写数据
** 入口参数 ：* data:写入数据缓冲区,第一个字节是寄存器地址；第二个字节开始要写入的数据。
						bytesNumber: 要写入的字节数
** 出口参数 ：无
** 函数说明 ：无
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
** 函数名称 ：void AD9833_SetRegisterValue(unsigned short regValue)
** 函数功能 ：将值写入寄存器
** 入口参数 ：regValue：要写入寄存器的值。
** 出口参数 ：无
** 函数说明 ：无
*******************************************************************************************/
void AD9833_SetRegisterValue(unsigned short regValue,SPI_TYPE spi_type,SPIX_Typedef spix)
{
	uint8_t data[5] = {0x03, 0x00, 0x00};	
	
	data[1] = (uint8_t)((regValue & 0xFF00) >> 8);
	data[2] = (uint8_t)((regValue & 0x00FF) >> 0);
	AD9833_SPI_Write(data,2,spi_type,spix);
}

/*****************************************************************************************
** 函数名称 ：void AD9833_Reset(void)  
** 函数功能 ：设置AD9833的复位位
** 入口参数 ：无
** 出口参数 ：无
** 函数说明 ：无
*******************************************************************************************/
void AD9833_Reset(SPI_TYPE spi_type,SPIX_Typedef spix)
{
	AD9833_SetRegisterValue(AD9833_REG_CMD | AD9833_RESET,spi_type,spix);
	Delay_ms(10);
}

/*****************************************************************************************
** 函数名称 ：void AD9833_ClearReset(void)  
** 函数功能 ：清除AD9833的复位位。
** 入口参数 ：无
** 出口参数 ：无
** 函数说明 ：无
*******************************************************************************************/
void AD9833_ClearReset(SPI_TYPE spi_type,SPIX_Typedef spix)
{
	AD9833_SetRegisterValue(AD9833_REG_CMD,spi_type,spix);
}

/*****************************************************************************************
** 函数名称 ：void AD9833_SetFrequency(unsigned short reg, float fout,unsigned short type)
** 函数功能 ：写入频率寄存器
** 入口参数 ：reg：要写入的频率寄存器。
**						val：要写入的值。
**						type：波形类型；AD9833_OUT_SINUS正弦波、AD9833_OUT_TRIANGLE三角波、AD9833_OUT_MSB方波
** 出口参数 ：无
** 函数说明 ：无
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
** 函数名称 ：void AD9833_SetFrequencyQuick(float fout,unsigned short type)
** 函数功能 ：写入频率寄存器
** 入口参数 ：val：要写入的频率值。
**						type：波形类型；AD9833_OUT_SINUS正弦波、AD9833_OUT_TRIANGLE三角波、AD9833_OUT_MSB方波
** 出口参数 ：无
** 函数说明 ：时钟速率为25 MHz时， 可以实现0.1 Hz的分辨率；而时钟速率为1 MHz时，则可以实现0.004 Hz的分辨率。
*******************************************************************************************/
void AD9833_SetFrequencyQuick(float fout,unsigned short type,SPI_TYPE spi_type,SPIX_Typedef spix)
{
	AD9833_SetFrequency(AD9833_REG_FREQ0, fout,type,spi_type,spix);
}
/*****************************************************************************************
** 函数名称 ：void AD9833_SetPhase(unsigned short reg, unsigned short val)
** 函数功能 ：写入相位寄存器。
** 入口参数 ：reg：要写入的相位寄存器。--》AD9833_REG_PHASE0；AD9833_REG_PHASE1
**						val：要写入的值。设置180°相位，范围：0-4095(0-360°)
** 出口参数 ：无
** 函数说明 ：无
*******************************************************************************************/
void AD9833_SetPhase(unsigned short reg, unsigned short val,SPI_TYPE spi_type,SPIX_Typedef spix)
{
	unsigned short phase = reg;
	phase |= val;
	AD9833_SetRegisterValue(phase,spi_type,spix);
}

/*****************************************************************************************
** 函数名称 ：void AD9833_Setup(unsigned short freq, unsigned short phase,unsigned short type)
** 函数功能 ：写入相位寄存器。
** 入口参数 ：freq：使用的频率寄存器。
							phase：使用的相位寄存器。
							type：要输出的波形类型。
** 出口参数 ：无
** 函数说明 ：无
*******************************************************************************************/
void AD9833_Setup(unsigned short freq, unsigned short phase,unsigned short type,SPI_TYPE spi_type,SPIX_Typedef spix)
{
	unsigned short val = 0;
	
	val = freq | phase | type;
	AD9833_SetRegisterValue(val,spi_type,spix);
}

/*****************************************************************************************
** 函数名称 ：void AD9833_SetWave(unsigned short type)
** 函数功能 ：设置要输出的波形类型。
** 入口参数 ：type：要输出的波形类型。
** 出口参数 ：无
** 函数说明 ：无
*******************************************************************************************/
void AD9833_SetWave(unsigned short type,SPI_TYPE spi_type,SPIX_Typedef spix)
{
	AD9833_SetRegisterValue(type,spi_type,spix);
}


