#ifndef __AT24CXX_H
#define __AT24CXX_H


#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f4xx.h"
#include "jack_i2c.h"

typedef struct
{
	uint8_t Write_Flag;
	uint8_t u8_wbuffer[20];
	uint8_t u8_rbuffer[20];
	double double_wbuffer[10];
	double double_rbuffer[10];
	uint32_t u32_wbufffer[10];
	uint32_t u32_rbufffer[10];
}	Storage_Typedef;

#define AT24CXX_PerWritePageSize   8

uint8_t AT24CXX_ReadOneByte(uint16_t ReadAddr,I2C_TYPE i2c_type,I2CX_Typedef i2cx);
void AT24CXX_ReadPage(uint8_t *pData, uint16_t StartAddr, I2C_TYPE i2c_type, I2CX_Typedef i2cx);
void AT24CXX_ReadBuffer(uint8_t* pBuffer, uint16_t ReadAddr, uint16_t NumByteToRead ,\
												I2C_TYPE i2c_type,I2CX_Typedef i2cx);

void AT24CXX_WriteOneByte(uint8_t pBuffer, uint16_t WriteAddr,I2C_TYPE i2c_type,I2CX_Typedef i2cx);
void AT24CXX_WritePage(uint8_t* pBuffer, uint16_t WriteAddr, uint16_t NumByteToWrite,\
											 I2C_TYPE i2c_type,I2CX_Typedef i2cx);
void AT24CXX_WriteBuffer(uint8_t* pBuffer, uint16_t WriteAddr, uint16_t NumByteToWrite,\
												 I2C_TYPE i2c_type,I2CX_Typedef i2cx);
	 	 	 
#ifdef __cplusplus
}
#endif

#endif	

