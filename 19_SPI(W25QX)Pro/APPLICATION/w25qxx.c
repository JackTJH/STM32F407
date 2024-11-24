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
  * @brief  读取FLASH ID
  * @param 	无
  * @retval FLASH ID
  */
uint32_t W25QXX_ReadID(SPI_TYPE spi_type)
{
  uint32_t Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

  /* 开始通讯：CS低电平 */
  W25QXX_CS(0);

  /* 发送JEDEC指令，读取ID */
  SPI_SendByte(W25X_JedecDeviceID,spi_type);
	
	if(spi_type == H_SPI)
	{
		/* 读取一个字节数据 */
		Temp0 = SPI_SendByte(Dummy_Byte,spi_type);

		/* 读取一个字节数据 */
		Temp1 = SPI_SendByte(Dummy_Byte,spi_type);

		/* 读取一个字节数据 */
		Temp2 = SPI_SendByte(Dummy_Byte,spi_type);
	}
	else
	{
		Temp0 = SPI_ReadByte();
		Temp1 = SPI_ReadByte();
		Temp2 = SPI_ReadByte();
	}

  /* 停止通讯：CS高电平 */
  W25QXX_CS(1);

	/*把数据组合起来，作为函数的返回值*/
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
  * @brief  等待WIP(BUSY)标志被置0，即等待到FLASH内部数据写入完毕
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
  * @brief  对FLASH写入数据，调用本函数写入数据前需要先擦除扇区
  * @param	pBuffer，要写入数据的指针
  * @param  WriteAddr，写入地址
  * @param  NumByteToWrite，写入数据长度
  * @retval 无
  */
void W25QXX_BufferWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite, SPI_TYPE spi_type)
{
  uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;
	
	/*mod运算求余，若writeAddr是SPI_FLASH_PageSize整数倍，运算结果Addr值为0*/
  Addr = WriteAddr % W25QXX_PageSize;
	
	/*差count个数据值，刚好可以对齐到页地址*/
  count = W25QXX_PageSize - Addr;	
	/*计算出要写多少整数页*/
  NumOfPage =  NumByteToWrite / W25QXX_PageSize;
	/*mod运算求余，计算出剩余不满一页的字节数*/
  NumOfSingle = NumByteToWrite % W25QXX_PageSize;

	 /* Addr=0,则WriteAddr 刚好按页对齐 aligned  */
  if (Addr == 0) 
  {
		/* NumByteToWrite < SPI_FLASH_PageSize */
    if (NumOfPage == 0) 
    {
      W25QXX_PageWrite(pBuffer, WriteAddr, NumByteToWrite, spi_type);
    }
    else /* NumByteToWrite > SPI_FLASH_PageSize */
    {
			/*先把整数页都写了*/
      while (NumOfPage--)
      {
        W25QXX_PageWrite(pBuffer, WriteAddr, W25QXX_PageSize, spi_type);
        WriteAddr +=  W25QXX_PageSize;
        pBuffer += W25QXX_PageSize;
      }
			
			/*若有多余的不满一页的数据，把它写完*/
      W25QXX_PageWrite(pBuffer, WriteAddr, NumOfSingle, spi_type);
    }
  }
	/* 若地址与 SPI_FLASH_PageSize 不对齐  */
  else 
  {
		/* NumByteToWrite < SPI_FLASH_PageSize */
    if (NumOfPage == 0) 
    {
			/*当前页剩余的count个位置比NumOfSingle小，写不完*/
      if (NumOfSingle > count) 
      {
        temp = NumOfSingle - count;
				
				/*先写满当前页*/
        W25QXX_PageWrite(pBuffer, WriteAddr, count, spi_type);
        WriteAddr +=  count;
        pBuffer += count;
				
				/*再写剩余的数据*/
        W25QXX_PageWrite(pBuffer, WriteAddr, temp, spi_type);
      }
      else /*当前页剩余的count个位置能写完NumOfSingle个数据*/
      {				
        W25QXX_PageWrite(pBuffer, WriteAddr, NumByteToWrite, spi_type);
      }
    }
    else /* NumByteToWrite > SPI_FLASH_PageSize */
    {
			/*地址不对齐多出的count分开处理，不加入这个运算*/
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / W25QXX_PageSize;
      NumOfSingle = NumByteToWrite % W25QXX_PageSize;

      W25QXX_PageWrite(pBuffer, WriteAddr, count, spi_type);
      WriteAddr +=  count;
      pBuffer += count;
			
			/*把整数页都写了*/
      while (NumOfPage--)
      {
        W25QXX_PageWrite(pBuffer, WriteAddr, W25QXX_PageSize, spi_type);
        WriteAddr +=  W25QXX_PageSize;
        pBuffer += W25QXX_PageSize;
      }
			/*若有多余的不满一页的数据，把它写完*/
      if (NumOfSingle != 0)
      {
        W25QXX_PageWrite(pBuffer, WriteAddr, NumOfSingle, spi_type);
      }
    }
  }
}

 /**
  * @brief  读取FLASH数据
  * @param 	pBuffer，存储读出数据的指针
  * @param   ReadAddr，读取地址
  * @param   NumByteToRead，读取数据长度
  * @retval 无
  */
void W25QXX_BufferRead(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead, SPI_TYPE spi_type)
{
  /* 选择FLASH: CS低电平 */
  W25QXX_CS(0);

  /* 发送 读 指令 */
  SPI_SendByte(W25X_ReadData,spi_type);

  /* 发送 读 地址高位 */
  SPI_SendByte((ReadAddr & 0xFF0000) >> 16,spi_type);
  /* 发送 读 地址中位 */
  SPI_SendByte((ReadAddr& 0xFF00) >> 8,spi_type);
  /* 发送 读 地址低位 */
  SPI_SendByte(ReadAddr & 0xFF,spi_type);
  
	/* 读取数据 */
  while (NumByteToRead--)
  {
    /* 读取一个字节*/
		if(spi_type == H_SPI)
		{
			*pBuffer = SPI_SendByte(Dummy_Byte,spi_type);
		}
		else
		{
			*pBuffer = SPI_ReadByte();
		}
   
    /* 指向下一个字节缓冲区 */
    pBuffer++;
  }

  /* 停止信号 FLASH: CS 高电平 */
  W25QXX_CS(1);
}

//进入掉电模式
void W25QXX_PowerDown(SPI_TYPE spi_type)   
{ 
  /* 选择 FLASH: CS 低 */
  W25QXX_CS(0);

  /* 发送 掉电 命令 */
  SPI_SendByte(W25X_PowerDown,spi_type);

  /* 停止信号  FLASH: CS 高 */
  W25QXX_CS(1);
}   

//唤醒
void SPI_Flash_WAKEUP(SPI_TYPE spi_type)   
{
  /*选择 FLASH: CS 低 */
  W25QXX_CS(0);

  /* 发上 上电 命令 */
  SPI_SendByte(W25X_ReleasePowerDown,spi_type);

  /* 停止信号 FLASH: CS 高 */
  W25QXX_CS(1);                  //等待TRES1
}  
