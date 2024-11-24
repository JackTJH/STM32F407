#ifndef __FLASH_H
#define __FLASH_H

#ifdef __cplusplus
	extern "C" {
#endif

#include "stm32f4xx.h"

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


#define FLASH_USER_START_ADDR   ADDR_FLASH_SECTOR_4    /* Start address of user Flash area */

#define FLASH_WRITE_NUM	3

//extern uint8_t WriteData[FLASH_WRITE_NUM];

/////* 要写入到STM32 FLASH的字符串数组 */
////const uint8_t g_text_buf[] = {"STM32 FLASH TEST"};

//#define TEXT_LENTH sizeof(WriteData[FLASH_WRITE_NUM]) /* 数组长度 */

///*SIZE表示半字长(4字节), 大小必须是4的整数倍, 如果不是的话, 强制对齐到4的整数倍 */
//#define SIZE TEXT_LENTH / 4 + ((TEXT_LENTH % 4) ? 1 : 0)

typedef struct
{
	uint8_t WriteData[FLASH_WRITE_NUM];
	uint8_t ReadDataTemp[FLASH_WRITE_NUM];
} My_Flash_Typedef;



uint32_t GetSector(uint32_t Address);
uint32_t stmflash_read_word(uint32_t faddr);
void stmflash_write(uint32_t waddr, uint32_t *pbuf, uint32_t length);
void stmflash_read(uint32_t raddr, uint32_t *pbuf, uint32_t length);



#ifdef __cplusplus
	{
#endif	

#endif
