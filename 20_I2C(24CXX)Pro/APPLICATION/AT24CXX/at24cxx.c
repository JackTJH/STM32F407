#include "at24cxx.h"
#include "delay.h"

extern I2C_TypeDef* JI2X[I2Cn];

uint8_t AT24CXX_ReadOneByte(uint16_t ReadAddr,I2C_TYPE i2c_type,I2CX_Typedef i2cx)
{
	uint8_t read;
	if(i2c_type == S_I2C)
	{	
		JI2C_Start(i2cx);
		JI2C_SendByte(0xA0,i2cx);
		while(JI2C_WaitAck(i2cx) != SET);
		
		JI2C_SendByte(ReadAddr,i2cx);
		while(JI2C_WaitAck(i2cx) != SET);
		
		JI2C_Start(i2cx);
		JI2C_SendByte(0xA1,i2cx);
		while(JI2C_WaitAck(i2cx) != SET);
		
		read = JI2C_ReadByte(i2cx);
		
		JI2C_MasterAck(0,i2cx);
		JI2C_Stop(i2cx);
	}
	else
	{	
		I2C_GenerateSTART(JI2X[i2cx], ENABLE);
		JI2CXX_WaitEvent(i2cx,I2C_EVENT_MASTER_MODE_SELECT);

		I2C_Send7bitAddress(JI2X[i2cx], 0xA0, I2C_Direction_Transmitter);
		JI2CXX_WaitEvent(i2cx,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);
		
		I2C_SendData(JI2X[i2cx], ReadAddr);
		JI2CXX_WaitEvent(i2cx,I2C_EVENT_MASTER_BYTE_TRANSMITTED);
//			JI2CXX_WaitEvent(i2cx,I2C_EVENT_MASTER_BYTE_TRANSMITTING);

		I2C_GenerateSTART(JI2X[i2cx], ENABLE);
		JI2CXX_WaitEvent(i2cx,I2C_EVENT_MASTER_MODE_SELECT);

		I2C_Send7bitAddress(JI2X[i2cx], 0xA0, I2C_Direction_Receiver);
		JI2CXX_WaitEvent(i2cx,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED);
		
		I2C_AcknowledgeConfig(JI2X[i2cx], DISABLE);
		I2C_GenerateSTOP(JI2X[i2cx], ENABLE);
		
		JI2CXX_WaitEvent(i2cx,I2C_EVENT_MASTER_BYTE_RECEIVED);
    read = I2C_ReceiveData(JI2X[i2cx]);
		
		I2C_AcknowledgeConfig(JI2X[i2cx], ENABLE);
	}	
	return read;
}

void AT24CXX_ReadPage(uint8_t *pData, uint16_t StartAddr, I2C_TYPE i2c_type, I2CX_Typedef i2cx)
{
	// 读取一页的数据
	for (int i = 0; i < 8; i++)
	{
			pData[i] = AT24CXX_ReadOneByte(StartAddr + i, i2c_type, i2cx);
	}
}

void AT24CXX_ReadBuffer(uint8_t* pBuffer, uint16_t ReadAddr, uint16_t NumByteToRead ,\
												I2C_TYPE i2c_type,I2CX_Typedef i2cx)
{	
	uint16_t remaining = NumByteToRead;
	uint16_t current_addr = ReadAddr;

	while (remaining > 0)
	{
		uint16_t bytes_to_read = (remaining >= 8) ? 8 : remaining;
		AT24CXX_ReadPage(pBuffer, current_addr, i2c_type, i2cx);

		pBuffer += bytes_to_read;
		current_addr += bytes_to_read;
		remaining -= bytes_to_read;
	}
}

void AT24CXX_WriteOneByte(uint8_t pBuffer, uint16_t WriteAddr,I2C_TYPE i2c_type,I2CX_Typedef i2cx)
{
	if(i2c_type == S_I2C)
	{
		JI2C_Start(i2cx);
		JI2C_SendByte(0xA0,i2cx);
		while(JI2C_WaitAck(i2cx) != SET);
		
		JI2C_SendByte(WriteAddr,i2cx);
		while(JI2C_WaitAck(i2cx) != SET);
		
		JI2C_SendByte(pBuffer,i2cx);
		while(JI2C_WaitAck(i2cx) != SET);
		
		JI2C_Stop(i2cx);
		Delay_ms(2);  
	}
	else
	{
		I2C_GenerateSTART(JI2X[i2cx],ENABLE);
		JI2CXX_WaitEvent(i2cx,I2C_EVENT_MASTER_MODE_SELECT);

		I2C_Send7bitAddress(JI2X[i2cx],0xA0,I2C_Direction_Transmitter);
		JI2CXX_WaitEvent(i2cx,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);
		
		I2C_SendData(JI2X[i2cx],WriteAddr);
		JI2CXX_WaitEvent(i2cx,I2C_EVENT_MASTER_BYTE_TRANSMITTING);
		
		I2C_SendData(JI2X[i2cx],pBuffer);
		JI2CXX_WaitEvent(i2cx,I2C_EVENT_MASTER_BYTE_TRANSMITTED);
		
		I2C_GenerateSTOP(JI2X[i2cx],ENABLE);
		Delay_ms(2);//这个无论是软件I2C还是硬件I2C都需要加且至少2ms  
	}
}

void AT24CXX_WritePage(uint8_t* pBuffer, uint16_t WriteAddr, uint16_t NumByteToWrite,\
											 I2C_TYPE i2c_type,I2CX_Typedef i2cx)
{
	
	if(NumByteToWrite > AT24CXX_PerWritePageSize)
	{
		NumByteToWrite = AT24CXX_PerWritePageSize;
	}
	
	while(NumByteToWrite--)
	{
		AT24CXX_WriteOneByte(*pBuffer++,WriteAddr,i2c_type,i2cx);
		WriteAddr++;
	}	
	
//	JI2C_Start(i2cx);
//	JI2C_SendByte(0xA0,i2cx);
//	while(JI2C_WaitAck(i2cx) != SET);
//	
//	JI2C_SendByte(WriteAddr,i2cx);
//	while(JI2C_WaitAck(i2cx) != SET);
//	
//	while(NumByteToWrite)
//	{
//		if(NumByteToWrite == 1)
//		{
//			I2C_GenerateSTOP(JI2X[i2cx],ENABLE);
//		}
//		JI2C_SendByte(*pBuffer++,i2cx);
////		while(JI2C_WaitAck(i2cx) != SET);
//		Delay_ms(10);
//		NumByteToWrite--;
//	}
	
//	I2C_GenerateSTART(JI2X[i2cx],ENABLE);
//	JI2CXX_WaitEvent(i2cx,I2C_EVENT_MASTER_MODE_SELECT);

//	I2C_Send7bitAddress(JI2X[i2cx],0xA0,I2C_Direction_Transmitter);
//	JI2CXX_WaitEvent(i2cx,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);
//	
//	I2C_SendData(JI2X[i2cx],WriteAddr);
//	JI2CXX_WaitEvent(i2cx,I2C_EVENT_MASTER_BYTE_TRANSMITTING);
//	
//	while(NumByteToWrite)
//	{
//		if(NumByteToWrite == 1)
//		{
//			I2C_GenerateSTOP(JI2X[i2cx],ENABLE);
//		}
//		I2C_SendData(JI2X[i2cx],*pBuffer++);
//		JI2CXX_WaitEvent(i2cx,I2C_EVENT_MASTER_BYTE_TRANSMITTING);
//		Delay_ms(2);
//		NumByteToWrite--;
//	}
}

void AT24CXX_WriteBuffer(uint8_t* pBuffer, uint16_t WriteAddr, uint16_t NumByteToWrite,\
												 I2C_TYPE i2c_type,I2CX_Typedef i2cx)
{
  uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;
	
  Addr = WriteAddr % AT24CXX_PerWritePageSize;
  count = AT24CXX_PerWritePageSize - Addr;	
  NumOfPage =  NumByteToWrite / AT24CXX_PerWritePageSize;
  NumOfSingle = NumByteToWrite % AT24CXX_PerWritePageSize;

  if (Addr == 0) 
  {
    if (NumOfPage == 0) 
    {
      AT24CXX_WritePage(pBuffer, WriteAddr, NumByteToWrite, i2c_type,i2cx);
    }
    else
    {
      while (NumOfPage--)
      {
        AT24CXX_WritePage(pBuffer, WriteAddr, AT24CXX_PerWritePageSize, i2c_type,i2cx);
        WriteAddr +=  AT24CXX_PerWritePageSize;
        pBuffer += AT24CXX_PerWritePageSize;
      }
      AT24CXX_WritePage(pBuffer, WriteAddr, NumOfSingle, i2c_type,i2cx);
    }
  }
  else 
  {
    if (NumOfPage == 0) 
    {
      if (NumOfSingle > count) 
      {
        temp = NumOfSingle - count;
        AT24CXX_WritePage(pBuffer, WriteAddr, count, i2c_type,i2cx);
        WriteAddr +=  count;
        pBuffer += count;
        AT24CXX_WritePage(pBuffer, WriteAddr, temp, i2c_type,i2cx);
      }
      else 
      {				
        AT24CXX_WritePage(pBuffer, WriteAddr, NumByteToWrite, i2c_type,i2cx);
      }
    }
    else 
    {
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / AT24CXX_PerWritePageSize;
      NumOfSingle = NumByteToWrite % AT24CXX_PerWritePageSize;

      AT24CXX_WritePage(pBuffer, WriteAddr, count, i2c_type,i2cx);
      WriteAddr +=  count;
      pBuffer += count;
      while (NumOfPage--)
      {
        AT24CXX_WritePage(pBuffer, WriteAddr, AT24CXX_PerWritePageSize, i2c_type,i2cx);
        WriteAddr +=  AT24CXX_PerWritePageSize;
        pBuffer += AT24CXX_PerWritePageSize;
      }
      if (NumOfSingle != 0)
      {
        AT24CXX_WritePage(pBuffer, WriteAddr, NumOfSingle, i2c_type,i2cx);
      }
    }
  }
}





