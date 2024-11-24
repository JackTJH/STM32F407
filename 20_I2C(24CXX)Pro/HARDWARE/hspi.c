#include "hspi.h"
#include "delay.h"

void JSPI_GPIO_Init(SPI_TYPE spi_type)
{
	RCC_AHB1PeriphClockCmd(HSPI_CS_GPIO_RCC, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = HSPI_CS_GPIO_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(HSPI_CS_GPIO_Port, &GPIO_InitStructure); 
	
	if(spi_type == S_SPI)
	{
		GPIO_InitStructure.GPIO_Pin = HSPI_SCK_GPIO_Pin;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
		GPIO_Init(HSPI_SCK_GPIO_Port, &GPIO_InitStructure); 
		
		GPIO_InitStructure.GPIO_Pin = HSPI_MISO_GPIO_Pin;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
		GPIO_Init(HSPI_MISO_GPIO_Port, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin = HSPI_MOSI_GPIO_Pin;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
		GPIO_Init(HSPI_MOSI_GPIO_Port, &GPIO_InitStructure);
		
		SPI1_SCK(0);
	}
	
	SPI1_CS(1);
}

/*
*		SPI_CS	:PB6
*		SPI_SCK	:PB3
*		SPI_MISO:PB4
*		SPI_MOSI:PB5
*/
void JSPI_Init(SPI_TYPE spi_type)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(HSPI_SCK_GPIO_RCC|HSPI_MISO_GPIO_RCC|HSPI_MOSI_GPIO_RCC, ENABLE);
	
	JSPI_GPIO_Init(spi_type);
	
	if(spi_type == H_SPI)
	{
		GPIO_InitStructure.GPIO_Pin = HSPI_SCK_GPIO_Pin|HSPI_MISO_GPIO_Pin|HSPI_MOSI_GPIO_Pin;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
		GPIO_Init(HSPI_SCK_GPIO_Port, &GPIO_InitStructure); 

		GPIO_PinAFConfig(HSPI_SCK_GPIO_Port, HSPI_SCK_GPIO_AFPin, HSPI_SCK_GPIO_AF);
		GPIO_PinAFConfig(HSPI_MISO_GPIO_Port, HSPI_MISO_GPIO_AFPin, HSPI_MISO_GPIO_AF);
		GPIO_PinAFConfig(HSPI_MOSI_GPIO_Port, HSPI_MOSI_GPIO_AFPin, HSPI_MOSI_GPIO_AF);

		HSPI_CLK_ENABLE();

		SPI_InitTypeDef SPI_InitStructure;

		SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
		SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
		SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
		SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
		SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
		SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
		SPI_Init(SPIX,&SPI_InitStructure);
		SPI_Cmd(SPIX,ENABLE);
	}
}

__IO uint32_t  SPITimeout = SPIT_LONG_TIMEOUT; 

/**
  * @brief  等待超时回调函数
  * @param  None.
  * @retval None.
  */
uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode)
{
  /* 等待超时后的处理,输出错误信息 */
  SPI_ERROR("SPI_WaitErrorCode = %d",errorCode);
  return 0;
}

uint8_t SPI_ReadByte(void)
{
	uint8_t temp = 0;
	for(uint8_t i = 0; i<8; i++)
	{
		/********************SPI模式0***********************/
		/********************别忘记改开头的SCK**************/
		SPI1_SCK(1);
		Delay_us(10);
		if(SPI1_ReadMISO())
		{
			temp |= 1<<(7-i);
		}
		SPI1_SCK(0);
		Delay_us(10);
		
		/********************SPI模式3***********************/
		/********************别忘记改开头的SCK**************/
//		SPI1_SCK(0);
//		Delay_us(10);
//		if(SPI1_ReadMISO())
//		{
//			temp |= 1<<(7-i);
//		}
//		SPI1_SCK(1);
//		Delay_us(10);	
	}
	return temp;
}

uint8_t SPI_SendByte(uint8_t byte,SPI_TYPE spi_type)
{
	SPITimeout = SPIT_FLAG_TIMEOUT;
	if(spi_type == H_SPI)
	{
		while(SPI_I2S_GetFlagStatus(SPIX,SPI_I2S_FLAG_TXE) != SET)
		{
			if((SPITimeout--) == 0)
			{
				return SPI_TIMEOUT_UserCallback(0);
			}
		}
		SPI_I2S_SendData(SPIX,byte);
		SPITimeout = SPIT_FLAG_TIMEOUT;
		while(SPI_I2S_GetFlagStatus(SPIX,SPI_I2S_FLAG_RXNE) != SET)
		{
			if((SPITimeout--) == 0)
			{
				return SPI_TIMEOUT_UserCallback(1);
			}
		}
		return SPI_I2S_ReceiveData(SPIX);
	}
	else
	{
		for(uint8_t i = 0; i<8; i++)
		{	
			/********************SPI模式0***********************/
			/********************别忘记改开头的SCK**************/
			if(byte & 1<<(7-i))
			{
				SPI1_MOSI(1);
			}
			else
			{
				SPI1_MOSI(0);
			}
			Delay_us(10);
			SPI1_SCK(1);
			Delay_us(10);
			SPI1_SCK(0);
			Delay_us(10);
			
			/********************SPI模式3***********************/
			/********************别忘记改开头的SCK**************/
//			if(byte & 1<<(7-i))
//			{
//				SPI1_MOSI(1);
//			}
//			else
//			{
//				SPI1_MOSI(0);
//			}
//			Delay_us(10);
//			SPI1_SCK(0);
//			Delay_us(10);
//			SPI1_SCK(1);
//			Delay_us(10);
		}
	}
	
	return 0;
}
