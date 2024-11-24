#ifndef __HSPI_H
#define __HSPI_H

#ifdef __cplusplus
	extern "C" {
#endif

#include "stm32f4xx.h"
#include "uart.h"

typedef enum
{
	H_SPI,
	S_SPI
}	SPI_TYPE;

#define SPI_MODE	H_SPI

#define SPIX										SPI1

#define HSPI_CS_GPIO_RCC				RCC_AHB1Periph_GPIOB
#define HSPI_SCK_GPIO_RCC				RCC_AHB1Periph_GPIOB
#define HSPI_MISO_GPIO_RCC			RCC_AHB1Periph_GPIOB
#define HSPI_MOSI_GPIO_RCC			RCC_AHB1Periph_GPIOB

#define HSPI_CS_GPIO_Port				GPIOB
#define HSPI_SCK_GPIO_Port			GPIOB
#define HSPI_MISO_GPIO_Port			GPIOB
#define HSPI_MOSI_GPIO_Port			GPIOB

#define HSPI_CS_GPIO_Pin				GPIO_Pin_6
#define HSPI_SCK_GPIO_Pin				GPIO_Pin_3
#define HSPI_MISO_GPIO_Pin			GPIO_Pin_4
#define HSPI_MOSI_GPIO_Pin			GPIO_Pin_5

#define HSPI_SCK_GPIO_AFPin			GPIO_PinSource3
#define HSPI_MISO_GPIO_AFPin		GPIO_PinSource4
#define HSPI_MOSI_GPIO_AFPin		GPIO_PinSource5

#define HSPI_SCK_GPIO_AF				GPIO_AF_SPI1
#define HSPI_MISO_GPIO_AF				GPIO_AF_SPI1
#define HSPI_MOSI_GPIO_AF				GPIO_AF_SPI1

#define HSPI_CLK_ENABLE() 			do{RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE); \
                                   }while(0)

#define SPI1_CS(bit)						GPIO_WriteBit(HSPI_CS_GPIO_Port,HSPI_CS_GPIO_Pin,(bit)?Bit_SET:Bit_RESET)
#define SPI1_SCK(bit)						GPIO_WriteBit(HSPI_SCK_GPIO_Port,HSPI_SCK_GPIO_Pin,(bit)?Bit_SET:Bit_RESET)
#define SPI1_ReadMISO()					GPIO_ReadInputDataBit(HSPI_MISO_GPIO_Port,HSPI_MISO_GPIO_Pin)
#define SPI1_MOSI(bit)					GPIO_WriteBit(HSPI_MOSI_GPIO_Port,HSPI_MOSI_GPIO_Pin,(bit)?Bit_SET:Bit_RESET)
																	 
/*等待超时时间*/
#define SPIT_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define SPIT_LONG_TIMEOUT         ((uint32_t)(10 * SPIT_FLAG_TIMEOUT))
																	 
#define SPI_ERROR(fmt,arg...)			printf("<<-SPI-ERROR->> "fmt"\r\n",##arg)	

extern __IO uint32_t  SPITimeout; 																	 

void HGPIO_Init(SPI_TYPE spi_type);
void JSPI_Init(SPI_TYPE spi_type);
uint8_t SPI_SendByte(uint8_t byte,SPI_TYPE spi_type);
uint8_t SPI_ReadByte(void);
uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode);

#ifdef __cplusplus
	}
#endif


#endif
