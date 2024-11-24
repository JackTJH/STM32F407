#include "w25qxx.h"
#include "hspi.h"

uint32_t W25QXX_ReadDeviceID(SPI_TYPE spi_type)
{
	uint32_t Temp = 0;
	
	W25QXX_CS(0);
	SPI_SendByte(W25X_DeviceID,spi_type);
	SPI_SendByte(Dummy_Byte,spi_type);
	SPI_SendByte(Dummy_Byte,spi_type);
	SPI_SendByte(Dummy_Byte,spi_type);
	
	if(spi_type == H_SPI)
	{
		Temp = SPI_SendByte(Dummy_Byte,spi_type);
	}
	else
	{
		Temp = SPI_ReadByte();
	}
	W25QXX_CS(1);
	
	return Temp;
}

 /**
  * @brief  ��ȡFLASH ID
  * @param 	��
  * @retval FLASH ID
  */
uint32_t W25QXX_ReadID(SPI_TYPE spi_type)
{
  uint32_t Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

  /* ��ʼͨѶ��CS�͵�ƽ */
  W25QXX_CS(0);

  /* ����JEDECָ���ȡID */
  SPI_SendByte(W25X_JedecDeviceID,spi_type);
	
	if(spi_type == H_SPI)
	{
		/* ��ȡһ���ֽ����� */
		Temp0 = SPI_SendByte(Dummy_Byte,spi_type);

		/* ��ȡһ���ֽ����� */
		Temp1 = SPI_SendByte(Dummy_Byte,spi_type);

		/* ��ȡһ���ֽ����� */
		Temp2 = SPI_SendByte(Dummy_Byte,spi_type);
	}
	else
	{
		Temp0 = SPI_ReadByte();
		Temp1 = SPI_ReadByte();
		Temp2 = SPI_ReadByte();
	}

  /* ֹͣͨѶ��CS�ߵ�ƽ */
  W25QXX_CS(1);

	/*�����������������Ϊ�����ķ���ֵ*/
  Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;

  return Temp;
}

void W25QXX_WriteEnable(SPI_TYPE spi_type)
{
	W25QXX_CS(0);
	SPI_SendByte(W25X_WriteEnable,spi_type);
	W25QXX_CS(1);
}

 /**
  * @brief  �ȴ�WIP(BUSY)��־����0�����ȴ���FLASH�ڲ�����д�����
  * @param  none
  * @retval none
  */
void W25QXX_WaitForWriteEnd(SPI_TYPE spi_type)
{
	uint8_t Status = 0;
	
	W25QXX_CS(0);
	SPI_SendByte(W25X_ReadStatusReg,spi_type);
	SPITimeout = SPIT_FLAG_TIMEOUT;
	do
	{
		if(spi_type == H_SPI)
		{
			Status = SPI_SendByte(Dummy_Byte,spi_type);
		}
		else
		{
			Status = SPI_ReadByte();
		}
		{
			if((SPITimeout--) == 0)
			{
				SPI_TIMEOUT_UserCallback(4);
				return;
			}
		}
	}while((Status & WIP_Flag) == SET);
	W25QXX_CS(1);
}

void W25QXX_SectorErase(uint32_t SectorAddr,SPI_TYPE spi_type)
{
	W25QXX_WriteEnable(spi_type);
	W25QXX_WaitForWriteEnd(spi_type);
	W25QXX_CS(0);
	SPI_SendByte(W25X_SectorErase,spi_type);
	SPI_SendByte((SectorAddr & 0xFF0000)>>16,spi_type);
	SPI_SendByte((SectorAddr & 0xFF00)>>8,spi_type);
	SPI_SendByte(SectorAddr & 0xFF,spi_type);
	W25QXX_CS(1);
	W25QXX_WaitForWriteEnd(spi_type);
}

void W25QXX_PageWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite,SPI_TYPE spi_type)
{
	W25QXX_WriteEnable(spi_type);
	W25QXX_CS(0);
	SPI_SendByte(W25X_PageProgram,spi_type);
	SPI_SendByte((WriteAddr & 0XFF0000)>>16,spi_type);
	SPI_SendByte((WriteAddr & 0XFF00)>>8,spi_type);
	SPI_SendByte((WriteAddr & 0XFF),spi_type);
	
	if(NumByteToWrite > W25QXX_PerWritePageSize)
	{
		NumByteToWrite = W25QXX_PerWritePageSize;
		SPI_ERROR("W25QXX_PageWrite too large!");
	}
	
	while(NumByteToWrite--)
	{
		SPI_SendByte(*pBuffer,spi_type);
		pBuffer++;
	}
	W25QXX_CS(1);
	W25QXX_WaitForWriteEnd(spi_type);
}

/**
  * @brief  ��FLASHд�����ݣ����ñ�����д������ǰ��Ҫ�Ȳ�������
  * @param	pBuffer��Ҫд�����ݵ�ָ��
  * @param  WriteAddr��д���ַ
  * @param  NumByteToWrite��д�����ݳ���
  * @retval ��
  */
void W25QXX_BufferWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite, SPI_TYPE spi_type)
{
  uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;
	
	/*mod�������࣬��writeAddr��SPI_FLASH_PageSize��������������AddrֵΪ0*/
  Addr = WriteAddr % W25QXX_PageSize;
	
	/*��count������ֵ���պÿ��Զ��뵽ҳ��ַ*/
  count = W25QXX_PageSize - Addr;	
	/*�����Ҫд��������ҳ*/
  NumOfPage =  NumByteToWrite / W25QXX_PageSize;
	/*mod�������࣬�����ʣ�಻��һҳ���ֽ���*/
  NumOfSingle = NumByteToWrite % W25QXX_PageSize;

	 /* Addr=0,��WriteAddr �պð�ҳ���� aligned  */
  if (Addr == 0) 
  {
		/* NumByteToWrite < SPI_FLASH_PageSize */
    if (NumOfPage == 0) 
    {
      W25QXX_PageWrite(pBuffer, WriteAddr, NumByteToWrite, spi_type);
    }
    else /* NumByteToWrite > SPI_FLASH_PageSize */
    {
			/*�Ȱ�����ҳ��д��*/
      while (NumOfPage--)
      {
        W25QXX_PageWrite(pBuffer, WriteAddr, W25QXX_PageSize, spi_type);
        WriteAddr +=  W25QXX_PageSize;
        pBuffer += W25QXX_PageSize;
      }
			
			/*���ж���Ĳ���һҳ�����ݣ�����д��*/
      W25QXX_PageWrite(pBuffer, WriteAddr, NumOfSingle, spi_type);
    }
  }
	/* ����ַ�� SPI_FLASH_PageSize ������  */
  else 
  {
		/* NumByteToWrite < SPI_FLASH_PageSize */
    if (NumOfPage == 0) 
    {
			/*��ǰҳʣ���count��λ�ñ�NumOfSingleС��д����*/
      if (NumOfSingle > count) 
      {
        temp = NumOfSingle - count;
				
				/*��д����ǰҳ*/
        W25QXX_PageWrite(pBuffer, WriteAddr, count, spi_type);
        WriteAddr +=  count;
        pBuffer += count;
				
				/*��дʣ�������*/
        W25QXX_PageWrite(pBuffer, WriteAddr, temp, spi_type);
      }
      else /*��ǰҳʣ���count��λ����д��NumOfSingle������*/
      {				
        W25QXX_PageWrite(pBuffer, WriteAddr, NumByteToWrite, spi_type);
      }
    }
    else /* NumByteToWrite > SPI_FLASH_PageSize */
    {
			/*��ַ����������count�ֿ������������������*/
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / W25QXX_PageSize;
      NumOfSingle = NumByteToWrite % W25QXX_PageSize;

      W25QXX_PageWrite(pBuffer, WriteAddr, count, spi_type);
      WriteAddr +=  count;
      pBuffer += count;
			
			/*������ҳ��д��*/
      while (NumOfPage--)
      {
        W25QXX_PageWrite(pBuffer, WriteAddr, W25QXX_PageSize, spi_type);
        WriteAddr +=  W25QXX_PageSize;
        pBuffer += W25QXX_PageSize;
      }
			/*���ж���Ĳ���һҳ�����ݣ�����д��*/
      if (NumOfSingle != 0)
      {
        W25QXX_PageWrite(pBuffer, WriteAddr, NumOfSingle, spi_type);
      }
    }
  }
}

 /**
  * @brief  ��ȡFLASH����
  * @param 	pBuffer���洢�������ݵ�ָ��
  * @param   ReadAddr����ȡ��ַ
  * @param   NumByteToRead����ȡ���ݳ���
  * @retval ��
  */
void W25QXX_BufferRead(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead, SPI_TYPE spi_type)
{
  /* ѡ��FLASH: CS�͵�ƽ */
  W25QXX_CS(0);

  /* ���� �� ָ�� */
  SPI_SendByte(W25X_ReadData,spi_type);

  /* ���� �� ��ַ��λ */
  SPI_SendByte((ReadAddr & 0xFF0000) >> 16,spi_type);
  /* ���� �� ��ַ��λ */
  SPI_SendByte((ReadAddr& 0xFF00) >> 8,spi_type);
  /* ���� �� ��ַ��λ */
  SPI_SendByte(ReadAddr & 0xFF,spi_type);
  
	/* ��ȡ���� */
  while (NumByteToRead--)
  {
    /* ��ȡһ���ֽ�*/
		if(spi_type == H_SPI)
		{
			*pBuffer = SPI_SendByte(Dummy_Byte,spi_type);
		}
		else
		{
			*pBuffer = SPI_ReadByte();
		}
   
    /* ָ����һ���ֽڻ����� */
    pBuffer++;
  }

  /* ֹͣ�ź� FLASH: CS �ߵ�ƽ */
  W25QXX_CS(1);
}

//�������ģʽ
void W25QXX_PowerDown(SPI_TYPE spi_type)   
{ 
  /* ѡ�� FLASH: CS �� */
  W25QXX_CS(0);

  /* ���� ���� ���� */
  SPI_SendByte(W25X_PowerDown,spi_type);

  /* ֹͣ�ź�  FLASH: CS �� */
  W25QXX_CS(1);
}   

//����
void SPI_Flash_WAKEUP(SPI_TYPE spi_type)   
{
  /*ѡ�� FLASH: CS �� */
  W25QXX_CS(0);

  /* ���� �ϵ� ���� */
  SPI_SendByte(W25X_ReleasePowerDown,spi_type);

  /* ֹͣ�ź� FLASH: CS �� */
  W25QXX_CS(1);                  //�ȴ�TRES1
}  
