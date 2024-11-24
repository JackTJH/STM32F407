#include "flash.h"
#include "uart.h"

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



/**
 * @brief       从指定地址读取一个字 (32位数据)
 * @param       faddr   : 读取地址 (此地址必须为4倍数!!)
 * @retval      读取到的数据 (32位)
 */
uint32_t stmflash_read_word(uint32_t faddr)
{
    return *(volatile uint32_t *)faddr;
}

/**
 * @brief       在FLASH 指定位置, 写入指定长度的数据(自动擦除)
 *   @note      因为STM32F4的扇区实在太大,没办法本地保存扇区数据,所以本函数写地址如果非0XFF
 *              ,那么会先擦除整个扇区且不保存扇区数据.所以写非0XFF的地址,将导致整个扇区数据丢失.
 *              建议写之前确保扇区里没有重要数据,最好是整个扇区先擦除了,然后慢慢往后写.
 *              该函数对OTP区域也有效!可以用来写OTP区!
 *              OTP区域地址范围:0X1FFF7800~0X1FFF7A0F(注意：最后16字节，用于OTP数据块锁定，别乱写！！)
 * @param       waddr   : 起始地址 (此地址必须为4的倍数!!,否则写入出错!)
 * @param       pbuf    : 数据指针
 * @param       length  : 要写入的 字(32位)数(就是要写入的32位数据的个数)
 * @retval      无
 */
void stmflash_write(uint32_t waddr, uint32_t *pbuf, uint32_t length)
{
	uint32_t addrx = 0;
	uint32_t endaddr = 0;
	uint32_t sector = 0;
	
	if (waddr < ADDR_FLASH_SECTOR_0 || waddr % 4 ||        	/* 写入地址小于 STM32_FLASH_BASE, 或不是4的整数倍, 非法. */
      waddr > (ADDR_FLASH_SECTOR_8))  										/* 写入地址大于 STM32_FLASH_BASE + STM32_FLASH_SIZE, 非法. */
  {
		return;
  }
	FLASH_Unlock();
	FLASH->ACR &= ~(1 << 10);       /* FLASH擦除期间,必须禁止数据缓存!!! */
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
                  FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR);
	addrx = waddr;                  /* 写入的起始地址 */
  endaddr = waddr + length * 4;   /* 写入的结束地址 */
	
	if (addrx < 0X1FFF0000)         /* 只有主存储区,才需要执行擦除操作!! */
	{
		while (addrx < endaddr)     /* 扫清一切障碍.(对非FFFFFFFF的地方,先擦除) */
		{
				if (stmflash_read_word(addrx) != 0XFFFFFFFF)    /* 有非0XFFFFFFFF的地方,要擦除这个扇区 */
				{
					sector = GetSector(addrx);
					if(FLASH_EraseSector(sector, VoltageRange_3) != FLASH_COMPLETE) 
					{
							break;/* 发生错误了 */
					}
				}
				else
				{
						addrx += 4;
				}
				FLASH_WaitForLastOperation();
		}
	}
  if (FLASH_WaitForLastOperation() == FLASH_COMPLETE)
  {
		while (waddr < endaddr)     /* 写数据 */
		{
			if (FLASH_ProgramWord(waddr, *pbuf) != FLASH_COMPLETE)  /* 写入数据 */
			{
					break;              	/* 写入异常 */
			}
			waddr += 4;
			pbuf++;
		}
  }
    
	FLASH->ACR |= 1 << 10;          /* FLASH擦除结束,开启数据fetch */

	FLASH_Lock();              			/* 上锁 */
}

/**
 * @brief       从指定地址开始读出指定长度的数据
 * @param       raddr : 起始地址
 * @param       pbuf  : 数据指针
 * @param       length: 要读取的字(32)数,即4个字节的整数倍
 * @retval      无
 */
void stmflash_read(uint32_t raddr, uint32_t *pbuf, uint32_t length)
{
    uint32_t i;

    for (i = 0; i < length; i++)
    {
        pbuf[i] = stmflash_read_word(raddr);    /* 读取4个字节. */
        raddr += 4; /* 偏移4个字节. */
    }
}
