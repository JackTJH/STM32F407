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

FlashData stmflash_read(uint32_t faddr, DataType dataType)
{
	FlashData data;

	switch (dataType) {
			case DATA_TYPE_UINT8:
					data.u8 = *(volatile uint8_t *)faddr;
					break;
			case DATA_TYPE_UINT16:
					data.u16 = *(volatile uint16_t *)faddr;
					break;
			case DATA_TYPE_UINT32:
					data.u32 = *(volatile uint32_t *)faddr;
					break;
	}
	return data;
}

void stmflash_readfull(uint32_t raddr, void *pbuf, uint32_t length, DataType dataType)
{
    uint32_t i;
    uint32_t alignment = 1; // Ĭ�϶��뷽ʽΪ1�ֽ�

    // ������������������Ӧ�Ķ��뷽ʽ�ʹ�С����
    switch (dataType) 
		{
			case DATA_TYPE_UINT8:
					alignment = 1;
					break;
			case DATA_TYPE_UINT16:
					alignment = 2;
					break;
			case DATA_TYPE_UINT32:
					alignment = 4;
					break;
    }

    for (i = 0; i < length; i++)
		{
        FlashData flashData = stmflash_read(raddr, dataType);

        switch (dataType)
				{
            case DATA_TYPE_UINT8:
                *((uint8_t*)pbuf) = flashData.u8;
                pbuf = (uint8_t*)pbuf + 1; // �ƶ�����һ������λ��
                break;
            case DATA_TYPE_UINT16:
                *((uint16_t*)pbuf) = flashData.u16;
                pbuf = (uint16_t*)pbuf + 1; // �ƶ�����һ������λ��
                break;
            case DATA_TYPE_UINT32:
                *((uint32_t*)pbuf) = flashData.u32;
                pbuf = (uint32_t*)pbuf + 1; // �ƶ�����һ������λ��
                break;
        }

        raddr += alignment; // ƫ�Ƶ���һ����ַ
    }
}

void stmflash_write(uint32_t waddr, void *pbuf, uint32_t length, DataType dataType)
{
	uint32_t sector = 0;
	uint32_t addrx = 0;
	uint32_t endaddr = 0;
	uint32_t alignment = 1; // Ĭ�϶��뷽ʽΪ1�ֽ�
  uint32_t sizeMultiplier = 1; // ���ݴ�С������Ĭ��Ϊ1
  uint8_t voltageRange = VoltageRange_3; // Ĭ�ϵ�ѹ��Χ
	// ������������������Ӧ�Ķ��뷽ʽ����С�����͵�ѹ��Χ
	switch (dataType) 
	{
		case DATA_TYPE_UINT8:
				alignment = 1;
				sizeMultiplier = 1;
				voltageRange = VoltageRange_1;
				break;
		case DATA_TYPE_UINT16:
				alignment = 2;
				sizeMultiplier = 2;
				voltageRange = VoltageRange_2; // ����16λ������Ҫ��ͬ�ĵ�ѹ��Χ
				break;
		case DATA_TYPE_UINT32:
				alignment = 4;
				sizeMultiplier = 4;
				voltageRange = VoltageRange_3;
				break;
	}
	
	// ����ַ�Ƿ�Ϸ�
  if (waddr < ADDR_FLASH_SECTOR_0 || waddr % alignment != 0 || waddr > ADDR_FLASH_SECTOR_8)
	{
		return;
  }
	FLASH_Unlock();
	FLASH->ACR &= ~(1 << 10);       /* FLASH�����ڼ�,�����ֹ���ݻ���!!! */
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
                  FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR);
	addrx = waddr; // д�����ʼ��ַ
  endaddr = waddr + length * sizeMultiplier; // д��Ľ�����ַ
	
	if (addrx < 0X1FFF0000)         /* ֻ�����洢��,����Ҫִ�в�������!! */
	{
		while (addrx < endaddr)     /* ɨ��һ���ϰ�.(�Է�FFFFFFFF�ĵط�,�Ȳ���) */
		{
			FlashData flashData = stmflash_read(addrx, dataType);
      bool isErased = false;

			switch (dataType) 
			{
					case DATA_TYPE_UINT8:
							isErased = (flashData.u8 == 0xFF);
							break;
					case DATA_TYPE_UINT16:
							isErased = (flashData.u16 == 0xFFFF);
							break;
					case DATA_TYPE_UINT32:
							isErased = (flashData.u32 == 0xFFFFFFFF);
							break;
			}
			if (!isErased) 
			{ // �з�ȫF�ĵط�, Ҫ�����������
				sector = GetSector(addrx);
				if (FLASH_EraseSector(sector, voltageRange) != FLASH_COMPLETE)
				{
						break; // ����������
				}
      } 
			else 
			{
        addrx += alignment;
      }
      FLASH_WaitForLastOperation();
		}
	}
	
	if (FLASH_WaitForLastOperation() == FLASH_COMPLETE)
	{
		while (waddr < endaddr)
		{ // д����
			uint32_t dataToWrite = 0;
			switch (dataType)
			{
				case DATA_TYPE_UINT8:
						dataToWrite = *((uint8_t*)pbuf);
						if (FLASH_ProgramByte(waddr, dataToWrite) != FLASH_COMPLETE)
						{ // д������
							break; // д���쳣
						}
						pbuf = (uint8_t*)pbuf + alignment; // �ƶ�����һ������λ��
						break;
				case DATA_TYPE_UINT16:
						dataToWrite = *((uint16_t*)pbuf);
						if (FLASH_ProgramHalfWord(waddr, dataToWrite) != FLASH_COMPLETE)
						{ // д������
							break; // д���쳣
						}
						pbuf = (uint16_t*)pbuf + alignment; // �ƶ�����һ������λ��
						break;
				case DATA_TYPE_UINT32:
						dataToWrite = *((uint32_t*)pbuf);
						if (FLASH_ProgramWord(waddr, dataToWrite) != FLASH_COMPLETE)
						{ // д������
							break; // д���쳣
						}
						pbuf = (uint32_t*)pbuf + alignment; // �ƶ�����һ������λ��						
						break;
			}
	
			waddr += alignment;
			
		}
	}
	
	FLASH->ACR |= 1 << 10; // FLASH��������, ��������fetch
  FLASH_Lock(); // ����
}


///**
// * @brief       ��FLASH ָ��λ��, д��ָ�����ȵ�����(�Զ�����)
// *   @note      ��ΪSTM32F4������ʵ��̫��,û�취���ر�����������,���Ա�����д��ַ�����0XFF
// *              ,��ô���Ȳ������������Ҳ�������������.����д��0XFF�ĵ�ַ,�����������������ݶ�ʧ.
// *              ����д֮ǰȷ��������û����Ҫ����,��������������Ȳ�����,Ȼ����������д.
// *              �ú�����OTP����Ҳ��Ч!��������дOTP��!
// *              OTP�����ַ��Χ:0X1FFF7800~0X1FFF7A0F(ע�⣺���16�ֽڣ�����OTP���ݿ�����������д����)
// * @param       waddr   : ��ʼ��ַ (�˵�ַ����Ϊ4�ı���!!,����д�����!)
// * @param       pbuf    : ����ָ��
// * @param       length  : Ҫд��� ��(32λ)��(����Ҫд���32λ���ݵĸ���)
// * @retval      ��
// */
//void stmflash_write(uint32_t waddr, uint32_t *pbuf, uint32_t length)
//{
//	uint32_t addrx = 0;
//	uint32_t endaddr = 0;
//	uint32_t sector = 0;
//	
//	if (waddr < ADDR_FLASH_SECTOR_0 || waddr % 4 ||        	/* д���ַС�� STM32_FLASH_BASE, ����4��������, �Ƿ�. */
//      waddr > (ADDR_FLASH_SECTOR_8))  										/* д���ַ���� STM32_FLASH_BASE + STM32_FLASH_SIZE, �Ƿ�. */
//  {
//		return;
//  }
//	FLASH_Unlock();
//	FLASH->ACR &= ~(1 << 10);       /* FLASH�����ڼ�,�����ֹ���ݻ���!!! */
//	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
//                  FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR);
//	addrx = waddr;                  /* д�����ʼ��ַ */
//  endaddr = waddr + length * 4;   /* д��Ľ�����ַ */
//	
//	if (addrx < 0X1FFF0000)         /* ֻ�����洢��,����Ҫִ�в�������!! */
//	{
//		while (addrx < endaddr)     /* ɨ��һ���ϰ�.(�Է�FFFFFFFF�ĵط�,�Ȳ���) */
//		{
//				if (stmflash_read_word(addrx) != 0XFFFFFFFF)    /* �з�0XFFFFFFFF�ĵط�,Ҫ����������� */
//				{
//					sector = GetSector(addrx);
//					if(FLASH_EraseSector(sector, VoltageRange_3) != FLASH_COMPLETE) 
//					{
//							break;/* ���������� */
//					}
//				}
//				else
//				{
//						addrx += 4;
//				}
//				FLASH_WaitForLastOperation();
//		}
//	}
//  if (FLASH_WaitForLastOperation() == FLASH_COMPLETE)
//  {
//		while (waddr < endaddr)     /* д���� */
//		{
//			if (FLASH_ProgramWord(waddr, *pbuf) != FLASH_COMPLETE)  /* д������ */
//			{
//					break;              	/* д���쳣 */
//			}
//			waddr += 4;
//			pbuf++;
//		}
//  }
//    
//	FLASH->ACR |= 1 << 10;          /* FLASH��������,��������fetch */

//	FLASH_Lock();              			/* ���� */
//}

///**
// * @brief       ��ָ����ַ��ʼ����ָ�����ȵ�����
// * @param       raddr : ��ʼ��ַ
// * @param       pbuf  : ����ָ��
// * @param       length: Ҫ��ȡ����(32)��,��4���ֽڵ�������
// * @retval      ��
// */
//void stmflash_read(uint32_t raddr, uint32_t *pbuf, uint32_t length)
//{
//    uint32_t i;

//    for (i = 0; i < length; i++)
//    {
//        pbuf[i] = stmflash_read_word(raddr);    /* ��ȡ4���ֽ�. */
//        raddr += 4; /* ƫ��4���ֽ�. */
//    }
//}
