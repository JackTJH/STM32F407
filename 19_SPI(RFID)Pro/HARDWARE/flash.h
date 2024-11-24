#ifndef __FLASH_H
#define __FLASH_H

#ifdef __cplusplus
	extern "C" {
#endif

#include "stm32f4xx.h"
#include "stm32f4xx_flash.h"

/* Base address of the Flash sectors */ 
#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) /* Base address of Sector 0, 16 Kbytes   */
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08004000) /* Base address of Sector 1, 16 Kbytes   */
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08008000) /* Base address of Sector 2, 16 Kbytes   */
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x0800C000) /* Base address of Sector 3, 16 Kbytes   */
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08010000) /* Base address of Sector 4, 64 Kbytes   */
#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08020000) /* Base address of Sector 5, 128 Kbytes  */
#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08040000) /* Base address of Sector 6, 128 Kbytes  */
#define ADDR_FLASH_SECTOR_7     ((uint32_t)0x08060000) /* Base address of Sector 7, 128 Kbytes  */
#define ADDR_FLASH_SECTOR_8     ((uint32_t)0x08080000) /* Base address of Sector 8, 128 Kbytes  */


#define INTERNAL_FLASH_STAR_ADDR   ADDR_FLASH_SECTOR_4    /* Start address of user Flash area */

#define FLASH_WRITE_NUM	3

typedef struct
{
	uint8_t WriteData[FLASH_WRITE_NUM];
	uint8_t ReadDataTemp[FLASH_WRITE_NUM];
} My_Flash_Typedef;


void InternalFlash_Erase(uint32_t startAddress);
void InternalFlash_Read(uint32_t* buffer, uint32_t startAddress, uint32_t length);
void InternalFlash_Write(uint32_t* data, uint32_t startAddress, uint32_t length);



#ifdef __cplusplus
	{
#endif	

#endif
