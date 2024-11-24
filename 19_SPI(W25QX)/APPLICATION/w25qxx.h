#ifndef __W25QXX_H
#define __W25QXX_H

#ifdef	__cplusplus
	extern "C" {
#endif

#include "hspi.h"

typedef struct
{
	__IO uint32_t DeviceID;
	__IO uint32_t FlashID;
}	W25QXX_Typedef;

/* Private define ------------------------------------------------------------*/
/*命令定义-开头*******************************/
#define W25X_WriteEnable		      0x06 
#define W25X_WriteDisable		      0x04 
#define W25X_ReadStatusReg		    0x05 
#define W25X_WriteStatusReg		  	0x01 
#define W25X_ReadData			        0x03 
#define W25X_FastReadData		      0x0B 
#define W25X_FastReadDual		      0x3B 
#define W25X_PageProgram		      0x02 
#define W25X_BlockErase			      0xD8 
#define W25X_SectorErase		      0x20 
#define W25X_ChipErase			      0xC7 
#define W25X_PowerDown			      0xB9 
#define W25X_ReleasePowerDown	  	0xAB 
#define W25X_DeviceID			        0xAB 
#define W25X_ManufactDeviceID   	0x90 
#define W25X_JedecDeviceID		    0x9F 

#define WIP_Flag                  0x01  /* Write In Progress (WIP) flag */
#define Dummy_Byte                0xFF

#define SPI_FLASH_PageSize              256
#define SPI_FLASH_PerWritePageSize      256

#define  W25QXX_WriteAddress     0x00000
#define  W25QXX_ReadAddress      W25QXX_WriteAddress
#define  W25QXX_SectorToErase    W25QXX_WriteAddress

uint32_t W25QXX_ReadDeviceID(SPI_TYPE spi_type);
uint32_t W25QXX_ReadID(SPI_TYPE spi_type);
void W25QXX_SectorErase(uint32_t SectorAddr,SPI_TYPE spi_type);
void W25QXX_PageWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite,SPI_TYPE spi_type);
void W25QXX_BufferWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite, SPI_TYPE spi_type);
void W25QXX_BufferRead(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead, SPI_TYPE spi_type);
void W25QXX_PowerDown(SPI_TYPE spi_type);
void SPI_Flash_WAKEUP(SPI_TYPE spi_type);


#ifdef __cplusplus
	}
#endif

#endif
