#include "hspi.h"
#include "delay.h"

void JSPI_Delay(void)
{
	Delay_us(5);
}

GPIO_TypeDef* SPI_GPIO_PORT[SPIn] = {SPI1_GPIO_Port,SPI2_GPIO_Port};
const uint32_t SPI_CS_GPIO_PIN[SPIn] = {JSPI1_CS_GPIO_Pin,JSPI2_CS_GPIO_Pin};
const uint32_t SPI_SCK_GPIO_PIN[SPIn] = {JSPI1_SCK_GPIO_Pin,JSPI2_SCK_GPIO_Pin};
const uint32_t SPI_MOSI_GPIO_PIN[SPIn] = {JSPI1_MOSI_GPIO_Pin,JSPI2_MOSI_GPIO_Pin};
const uint32_t SPI_MISO_GPIO_PIN[SPIn] = {JSPI1_MISO_GPIO_Pin,JSPI2_MISO_GPIO_Pin};

void JSPI_GPIO_Init(SPI_TYPE spi_type,SPIX_Typedef spix)
{
	SPIX_GPIO_CLK_ENABLE(spix);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = SPI_CS_GPIO_PIN[spix];
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(SPI_GPIO_PORT[spix], &GPIO_InitStructure); 
	
	if(spi_type == S_SPI)
	{
		GPIO_InitStructure.GPIO_Pin = SPI_SCK_GPIO_PIN[spix];
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
		GPIO_Init(SPI_GPIO_PORT[spix], &GPIO_InitStructure); 
		
		GPIO_InitStructure.GPIO_Pin = SPI_MISO_GPIO_PIN[spix];
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
		GPIO_Init(SPI_GPIO_PORT[spix], &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin = SPI_MOSI_GPIO_PIN[spix];
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
		GPIO_Init(SPI_GPIO_PORT[spix], &GPIO_InitStructure);
		
	}
	
	SPIX_CS(1,spix);
}

SPI_TypeDef* SPIx[SPIn] = {SPI1,SPI2};

/*
*		SPI_CS	:PB6
*		SPI_SCK	:PB3
*		SPI_MISO:PB4
*		SPI_MOSI:PB5
*/
void JSPI_Init(SPI_TYPE spi_type,SPIX_Typedef spix)
{		
	JSPI_GPIO_Init(spi_type,spix);
	
	if(spi_type == H_SPI)
	{
		GPIO_InitTypeDef GPIO_InitStructure;
		if(spix == JSPI1)
		{
			GPIO_InitStructure.GPIO_Pin = JSPI1_SCK_GPIO_Pin|JSPI1_MISO_GPIO_Pin|JSPI1_MOSI_GPIO_Pin;
		}
		else if(spix == JSPI2)
		{
			GPIO_InitStructure.GPIO_Pin = JSPI2_SCK_GPIO_Pin|JSPI2_MISO_GPIO_Pin|JSPI2_MOSI_GPIO_Pin;
		}
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
		GPIO_Init(SPI_GPIO_PORT[spix], &GPIO_InitStructure); 

		if(spix == JSPI1)
		{
			GPIO_PinAFConfig(SPI_GPIO_PORT[spix], JSPI1_SCK_GPIO_AFPin, JSPI1_SCK_GPIO_AF);
			GPIO_PinAFConfig(SPI_GPIO_PORT[spix], JSPI1_MISO_GPIO_AFPin, JSPI1_SCK_GPIO_AF);
			GPIO_PinAFConfig(SPI_GPIO_PORT[spix], JSPI1_MOSI_GPIO_AFPin, JSPI1_SCK_GPIO_AF);
		}
		else if(spix == JSPI2)
		{
			GPIO_PinAFConfig(SPI_GPIO_PORT[spix], JSPI2_SCK_GPIO_AFPin, JSPI2_SCK_GPIO_AF);
			GPIO_PinAFConfig(SPI_GPIO_PORT[spix], JSPI2_MISO_GPIO_AFPin, JSPI2_SCK_GPIO_AF);
			GPIO_PinAFConfig(SPI_GPIO_PORT[spix], JSPI2_MOSI_GPIO_AFPin, JSPI2_SCK_GPIO_AF);
		}


		JSPIX_CLK_ENABLE(spix);

		SPI_InitTypeDef SPI_InitStructure;

		SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
		SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
		SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
		SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
		SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
		SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
		SPI_Init(SPIx[spix],&SPI_InitStructure);
		SPI_Cmd(SPIx[spix],ENABLE);
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

uint8_t SPI_ReadByte(SPIX_Typedef spix)
{
	uint8_t temp = 0;
	if(spix == JSPI1)
	{
		for(uint8_t i = 0; i<8; i++)
		{
			/********************SPI模式0***********************/
			/********************别忘记改开头的SCK**************/
			SPI1_SCK(1);
			JSPI_Delay();
			if(SPI1_ReadMISO())
			{
				temp |= 1<<(7-i);
			}
			SPI1_SCK(0);
			JSPI_Delay();
			
			/********************SPI模式3***********************/
			/********************别忘记改开头的SCK**************/
//			SPI1_SCK(1);
//			JSPI_Delay();
//			SPI1_SCK(0);
//			JSPI_Delay();
//			if(SPI1_ReadMISO())
//			{
//				temp |= 1<<(7-i);
//			}
//			SPI1_SCK(1);
//			JSPI_Delay();	
		}
	}
	else if(spix == JSPI2)
	{
		for(uint8_t i = 0; i<8; i++)
		{
			/********************SPI模式0***********************/
			/********************别忘记改开头的SCK**************/
			SPI2_SCK(1);
			JSPI_Delay();
			if(SPI2_ReadMISO())
			{
				temp |= 1<<(7-i);
			}
			SPI2_SCK(0);
			JSPI_Delay();
			
			/********************SPI模式3***********************/
			/********************别忘记改开头的SCK**************/
//			SPI2_SCK(1);
//			JSPI_Delay();
//			SPI2_SCK(0);
//			JSPI_Delay();
//			if(SPI2_ReadMISO())
//			{
//				temp |= 1<<(7-i);
//			}
//			SPI2_SCK(1);
//			JSPI_Delay();	
		}
	}

	return temp;
}

uint8_t SPI_SendByte(uint8_t byte,SPI_TYPE spi_type,SPIX_Typedef spix)
{
	SPITimeout = SPIT_FLAG_TIMEOUT;
	if(spi_type == H_SPI)
	{
		while(SPI_I2S_GetFlagStatus(SPIx[spix],SPI_I2S_FLAG_TXE) != SET)
		{
			if((SPITimeout--) == 0)
			{
				return SPI_TIMEOUT_UserCallback(0);
			}
		}
		SPI_I2S_SendData(SPIx[spix],byte);
		SPITimeout = SPIT_FLAG_TIMEOUT;
		while(SPI_I2S_GetFlagStatus(SPIx[spix],SPI_I2S_FLAG_RXNE) != SET)
		{
			if((SPITimeout--) == 0)
			{
				return SPI_TIMEOUT_UserCallback(1);
			}
		}
		return SPI_I2S_ReceiveData(SPIx[spix]);
	}
	else
	{
		if(spix == JSPI1)
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
				JSPI_Delay();
				SPI1_SCK(1);
				JSPI_Delay();
				SPI1_SCK(0);
				JSPI_Delay();
				
				/********************SPI模式3***********************/
				/********************别忘记改开头的SCK**************/
//				if(byte & 1<<(7-i))
//				{
//					SPI1_MOSI(1);
//				}
//				else
//				{
//					SPI1_MOSI(0);
//				}
//				JSPI_Delay();
//				SPI1_SCK(0);
//				JSPI_Delay();
//				SPI1_SCK(1);
//				JSPI_Delay();
			}
		}
		else if(spix == JSPI2)
		{
			for(uint8_t i = 0; i<8; i++)
			{	
				/********************SPI模式0***********************/
				/********************别忘记改开头的SCK**************/
				if(byte & 1<<(7-i))
				{
					SPI2_MOSI(1);
				}
				else
				{
					SPI2_MOSI(0);
				}
				JSPI_Delay();
				SPI2_SCK(1);
				JSPI_Delay();
				SPI2_SCK(0);
				JSPI_Delay();
				
				/********************SPI模式3***********************/
				/********************别忘记改开头的SCK**************/
//				if(byte & 1<<(7-i))
//				{
//					SPI2_MOSI(1);
//				}
//				else
//				{
//					SPI2_MOSI(0);
//				}
//				JSPI_Delay();
//				SPI2_SCK(0);
//				JSPI_Delay();
//				SPI2_SCK(1);
//				JSPI_Delay();
			}
		}	
	}
	
	return 0;
}
