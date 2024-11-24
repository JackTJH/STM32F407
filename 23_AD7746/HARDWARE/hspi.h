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

typedef enum
{
	JSPI1,
	JSPI2
} SPIX_Typedef;

#define SPIn															2

#define SPI_MODE													S_SPI

#define SPI1_GPIO_RCC											RCC_AHB1Periph_GPIOB
#define SPI2_GPIO_RCC											RCC_AHB1Periph_GPIOB

#define SPI1_GPIO_Port										GPIOB
#define SPI2_GPIO_Port										GPIOB

#define JSPI1_CS_GPIO_Pin									GPIO_Pin_6
#define JSPI1_SCK_GPIO_Pin								GPIO_Pin_3
#define JSPI1_MISO_GPIO_Pin								GPIO_Pin_4
#define JSPI1_MOSI_GPIO_Pin								GPIO_Pin_5

#define JSPI2_CS_GPIO_Pin									GPIO_Pin_12
#define JSPI2_SCK_GPIO_Pin								GPIO_Pin_13
#define JSPI2_MISO_GPIO_Pin								GPIO_Pin_14
#define JSPI2_MOSI_GPIO_Pin								GPIO_Pin_15

#define JSPI1_SCK_GPIO_AFPin							GPIO_PinSource3
#define JSPI1_MISO_GPIO_AFPin							GPIO_PinSource4
#define JSPI1_MOSI_GPIO_AFPin							GPIO_PinSource5

#define JSPI2_SCK_GPIO_AFPin							GPIO_PinSource13
#define JSPI2_MISO_GPIO_AFPin							GPIO_PinSource14
#define JSPI2_MOSI_GPIO_AFPin							GPIO_PinSource15

#define JSPI1_SCK_GPIO_AF									GPIO_AF_SPI1
#define JSPI2_SCK_GPIO_AF									GPIO_AF_SPI2

#define SPI1_GPIO_CLK_ENABLE()						RCC_AHB1PeriphClockCmd(SPI1_GPIO_RCC, ENABLE)
#define SPI2_GPIO_CLK_ENABLE()						RCC_AHB1PeriphClockCmd(SPI2_GPIO_RCC, ENABLE)

#define SPIX_GPIO_CLK_ENABLE(__INDEX__) 	do{if			((__INDEX__) == 0) SPI1_GPIO_CLK_ENABLE();\
																						 else if((__INDEX__) == 1) SPI2_GPIO_CLK_ENABLE();\
																					}while(0)

#define JSPIX_CLK_ENABLE(__INDEX__) 			do{if			((__INDEX__) == 0) RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE); \
																						 else if((__INDEX__) == 1) RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE); \
																					}while(0)
																					
#define SPIX_CS(bit,__INDEX__) 						do{if     ((__INDEX__) == 0) GPIO_WriteBit(SPI1_GPIO_Port,JSPI1_CS_GPIO_Pin,(bit)?Bit_SET:Bit_RESET); \
																						 else if((__INDEX__) == 1) GPIO_WriteBit(SPI2_GPIO_Port,JSPI2_CS_GPIO_Pin,(bit)?Bit_SET:Bit_RESET); \
																					}while(0)																				
																					
#define SPI1_SCK(bit)											GPIO_WriteBit(SPI1_GPIO_Port,JSPI1_SCK_GPIO_Pin,(bit)?Bit_SET:Bit_RESET)
#define SPI1_ReadMISO()										GPIO_ReadInputDataBit(SPI1_GPIO_Port,JSPI1_MISO_GPIO_Pin)
#define SPI1_MOSI(bit)										GPIO_WriteBit(SPI1_GPIO_Port,JSPI1_MOSI_GPIO_Pin,(bit)?Bit_SET:Bit_RESET)
																					
#define SPI2_SCK(bit)											GPIO_WriteBit(SPI2_GPIO_Port,JSPI2_SCK_GPIO_Pin,(bit)?Bit_SET:Bit_RESET)
#define SPI2_ReadMISO()										GPIO_ReadInputDataBit(SPI2_GPIO_Port,JSPI2_MISO_GPIO_Pin)
#define SPI2_MOSI(bit)										GPIO_WriteBit(SPI2_GPIO_Port,JSPI2_MOSI_GPIO_Pin,(bit)?Bit_SET:Bit_RESET)
																	 
/*等待超时时间*/
#define SPIT_FLAG_TIMEOUT         				((uint32_t)0x1000)
#define SPIT_LONG_TIMEOUT         				((uint32_t)(10 * SPIT_FLAG_TIMEOUT))
																	 
#define SPI_ERROR(fmt,arg...)							printf("<<-SPI-ERROR->> "fmt"\r\n",##arg)	

extern __IO uint32_t  SPITimeout; 	

void JSPI_Delay(void);																	 

void JSPI_Init(SPI_TYPE spi_type,SPIX_Typedef spix);
uint8_t SPI_SendByte(uint8_t byte,SPI_TYPE spi_type,SPIX_Typedef spix);
uint8_t SPI_ReadByte(SPIX_Typedef spix);
uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode);

#ifdef __cplusplus
	}
#endif


#endif
