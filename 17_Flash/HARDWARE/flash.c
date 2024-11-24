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
 * @brief       ��ָ����ַ��ȡһ���� (32λ����)
 * @param       faddr   : ��ȡ��ַ (�˵�ַ����Ϊ4����!!)
 * @retval      ��ȡ�������� (32λ)
 */
uint32_t stmflash_read_word(uint32_t faddr)
{
    return *(volatile uint32_t *)faddr;
}

/**
 * @brief       ��FLASH ָ��λ��, д��ָ�����ȵ�����(�Զ�����)
 *   @note      ��ΪSTM32F4������ʵ��̫��,û�취���ر�����������,���Ա�����д��ַ�����0XFF
 *              ,��ô���Ȳ������������Ҳ�������������.����д��0XFF�ĵ�ַ,�����������������ݶ�ʧ.
 *              ����д֮ǰȷ��������û����Ҫ����,��������������Ȳ�����,Ȼ����������д.
 *              �ú�����OTP����Ҳ��Ч!��������дOTP��!
 *              OTP�����ַ��Χ:0X1FFF7800~0X1FFF7A0F(ע�⣺���16�ֽڣ�����OTP���ݿ�����������д����)
 * @param       waddr   : ��ʼ��ַ (�˵�ַ����Ϊ4�ı���!!,����д�����!)
 * @param       pbuf    : ����ָ��
 * @param       length  : Ҫд��� ��(32λ)��(����Ҫд���32λ���ݵĸ���)
 * @retval      ��
 */
void stmflash_write(uint32_t waddr, uint32_t *pbuf, uint32_t length)
{
	uint32_t addrx = 0;
	uint32_t endaddr = 0;
	uint32_t sector = 0;
	
	if (waddr < ADDR_FLASH_SECTOR_0 || waddr % 4 ||        	/* д���ַС�� STM32_FLASH_BASE, ����4��������, �Ƿ�. */
      waddr > (ADDR_FLASH_SECTOR_8))  										/* д���ַ���� STM32_FLASH_BASE + STM32_FLASH_SIZE, �Ƿ�. */
  {
		return;
  }
	FLASH_Unlock();
	FLASH->ACR &= ~(1 << 10);       /* FLASH�����ڼ�,�����ֹ���ݻ���!!! */
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
                  FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR);
	addrx = waddr;                  /* д�����ʼ��ַ */
  endaddr = waddr + length * 4;   /* д��Ľ�����ַ */
	
	if (addrx < 0X1FFF0000)         /* ֻ�����洢��,����Ҫִ�в�������!! */
	{
		while (addrx < endaddr)     /* ɨ��һ���ϰ�.(�Է�FFFFFFFF�ĵط�,�Ȳ���) */
		{
				if (stmflash_read_word(addrx) != 0XFFFFFFFF)    /* �з�0XFFFFFFFF�ĵط�,Ҫ����������� */
				{
					sector = GetSector(addrx);
					if(FLASH_EraseSector(sector, VoltageRange_3) != FLASH_COMPLETE) 
					{
							break;/* ���������� */
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
		while (waddr < endaddr)     /* д���� */
		{
			if (FLASH_ProgramWord(waddr, *pbuf) != FLASH_COMPLETE)  /* д������ */
			{
					break;              	/* д���쳣 */
			}
			waddr += 4;
			pbuf++;
		}
  }
    
	FLASH->ACR |= 1 << 10;          /* FLASH��������,��������fetch */

	FLASH_Lock();              			/* ���� */
}

/**
 * @brief       ��ָ����ַ��ʼ����ָ�����ȵ�����
 * @param       raddr : ��ʼ��ַ
 * @param       pbuf  : ����ָ��
 * @param       length: Ҫ��ȡ����(32)��,��4���ֽڵ�������
 * @retval      ��
 */
void stmflash_read(uint32_t raddr, uint32_t *pbuf, uint32_t length)
{
    uint32_t i;

    for (i = 0; i < length; i++)
    {
        pbuf[i] = stmflash_read_word(raddr);    /* ��ȡ4���ֽ�. */
        raddr += 4; /* ƫ��4���ֽ�. */
    }
}
