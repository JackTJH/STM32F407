#include "flash.h"
#include "uart.h"
#include <stdbool.h>

uint32_t GetSector(uint32_t Address)
{
  uint32_t sector = 0;
  
  if((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0))
  {
    sector = FLASH_Sector_0;  
  }
  else if((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1))
  {
    sector = FLASH_Sector_1;  
  }
  else if((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2))
  {
    sector = FLASH_Sector_2;  
  }
  else if((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3))
  {
    sector = FLASH_Sector_3;  
  }
  else if((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4))
  {
    sector = FLASH_Sector_4;  
  }
  else if((Address < ADDR_FLASH_SECTOR_6) && (Address >= ADDR_FLASH_SECTOR_5))
  {
    sector = FLASH_Sector_5;  
  }
  else if((Address < ADDR_FLASH_SECTOR_7) && (Address >= ADDR_FLASH_SECTOR_6))
  {
    sector = FLASH_Sector_6;  
  }
  else if((Address < ADDR_FLASH_SECTOR_8) && (Address >= ADDR_FLASH_SECTOR_7))
  {
    sector = FLASH_Sector_7;  
  }
	
	return sector;
}




void InternalFlash_WaitForEnd(void)
{
	while(FLASH_WaitForLastOperation() != FLASH_COMPLETE);
}

void InternalFlash_Erase(uint32_t startAddress)
{
	uint32_t startSector = GetSector(startAddress);

	// 解锁Flash
	FLASH_Unlock();

	// 清除所有的标志位
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);
	
	FLASH_EraseSector(startSector, VoltageRange_3);
	
	InternalFlash_WaitForEnd();

	// 锁定Flash
	FLASH_Lock();
}

void InternalFlash_Read(uint32_t* buffer, uint32_t startAddress, uint32_t length)
{
    // 确保起始地址和缓冲区不为空
    if (startAddress == 0 || buffer == NULL)
    {
        return;
    }

    // 从Flash中读取数据
    for (uint32_t i = 0; i < length; i++)
    {
        buffer[i] = *(__IO uint32_t*)(startAddress + i * 4);
    }
}

void InternalFlash_Write(uint32_t* data, uint32_t startAddress, uint32_t length)
{		
		// 解锁Flash
    FLASH_Unlock();

    // 清除所有的标志位
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);

    // 写入数据
    for (uint32_t i = 0; i < length; i++)
    {
			// 写入一个字
			FLASH_ProgramWord(startAddress + i * 4, data[i]);

			InternalFlash_WaitForEnd();

			// 检查写入是否成功
			if (FLASH_GetStatus() != FLASH_COMPLETE)
			{
					break;
			}
    }

    // 锁定Flash
    FLASH_Lock();
}





