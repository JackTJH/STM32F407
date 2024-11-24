#include "mfrc522.h"
#include "delay.h"

uint8_t MRFRC522_ReadReg(uint8_t address, SPI_TYPE spi_type) 
{
	uint8_t	val;

	SPI1_CS(0);
	SPI_SendByte(address, spi_type);
	if(spi_type == H_SPI)
	{
		val = SPI_SendByte(0x00, spi_type);
	}
	else
	{
		val = SPI_ReadByte();
	}
	SPI1_CS(1);
	return val;
}

void MRFRC522_WriteReg(uint8_t address, uint8_t value, SPI_TYPE spi_type)
{
	SPI1_CS(0);
	SPI_SendByte(address,spi_type);
	SPI_SendByte(value,spi_type);
	SPI1_CS(1);
}

uint8_t MFRC522_ReadRegister(uint8_t addr, SPI_TYPE spi_type)
{
	uint8_t val;

	addr = ((addr << 1) & 0x7E) | 0x80;
	val = MRFRC522_ReadReg(addr, spi_type);
	return val;	
}

void MFRC522_WriteRegister(uint8_t addr, uint8_t val, SPI_TYPE spi_type)
{
	addr = (addr << 1) & 0x7E;															// Address format: 0XXXXXX0
  MRFRC522_WriteReg(addr, val, spi_type);
}

void MFRC522_SetBitMask(uint8_t reg, uint8_t mask, SPI_TYPE spi_type) 
{
	MFRC522_WriteRegister(reg, MFRC522_ReadRegister(reg,spi_type) | mask , spi_type);
}

void MFRC522_ClearBitMask(uint8_t reg, uint8_t mask, SPI_TYPE spi_type)
{
	MFRC522_WriteRegister(reg, MFRC522_ReadRegister(reg,spi_type) & (~mask), spi_type);
}

uint8_t MFRC522_ToCard(uint8_t command, uint8_t* sendData, uint8_t sendLen, uint8_t* backData, uint16_t* backLen, SPI_TYPE spi_type)
{
	uint8_t status = MI_ERR;
	uint8_t irqEn = 0x00;
	uint8_t waitIRq = 0x00;
	uint8_t lastBits;
	uint8_t n;
	uint16_t i;

	switch (command)
	{
		case PCD_AUTHENT:
			irqEn = 0x12;
			waitIRq = 0x10;
			break;
		case PCD_TRANSCEIVE:
			irqEn = 0x77;
			waitIRq = 0x30;
			break;
		default:
			break;
	}

	MFRC522_WriteRegister(MFRC522_REG_COMM_IE_N, irqEn | 0x80, spi_type);
	MFRC522_ClearBitMask(MFRC522_REG_COMM_IRQ, 0x80, spi_type);
	MFRC522_SetBitMask(MFRC522_REG_FIFO_LEVEL, 0x80, spi_type);
	MFRC522_WriteRegister(MFRC522_REG_COMMAND, PCD_IDLE, spi_type);

	// Writing data to the FIFO
	for (i = 0; i < sendLen; i++)
	{
		MFRC522_WriteRegister(MFRC522_REG_FIFO_DATA, sendData[i], spi_type);
	}

	// Execute the command
	MFRC522_WriteRegister(MFRC522_REG_COMMAND, command, spi_type);
	
	if (command == PCD_TRANSCEIVE)
	{
		MFRC522_SetBitMask(MFRC522_REG_BIT_FRAMING, 0x80, spi_type);		// StartSend=1,transmission of data starts 
	}

	// Waiting to receive data to complete
	i = 2000;	// i according to the clock frequency adjustment, the operator M1 card maximum waiting time 25ms
	do {
		// CommIrqReg[7..0]
		// Set1 TxIRq RxIRq IdleIRq HiAlerIRq LoAlertIRq ErrIRq TimerIRq
		n = MFRC522_ReadRegister(MFRC522_REG_COMM_IRQ, spi_type);
		i--;
	} while ((i!=0) && !(n&0x01) && !(n&waitIRq));

	MFRC522_ClearBitMask(MFRC522_REG_BIT_FRAMING, 0x80, spi_type);																// StartSend=0

	if(i != 0)
	{
		if(!(MFRC522_ReadRegister(MFRC522_REG_ERROR, spi_type) & 0x1B))
		{
			status = MI_OK;
			if (n & irqEn & 0x01)
			{
				status = MI_NOTAGERR;
			}
			if (command == PCD_TRANSCEIVE)
			{
				n = MFRC522_ReadRegister(MFRC522_REG_FIFO_LEVEL, spi_type);
				lastBits = MFRC522_ReadRegister(MFRC522_REG_CONTROL, spi_type) & 0x07;
				if (lastBits)
				{
					*backLen = (n-1)*8+lastBits;
				}
				else
				{
					*backLen = n*8;
				}
				if (n == 0)
				{
					n = 1;
				}
				if (n > MFRC522_MAX_LEN)
				{					
					n = MFRC522_MAX_LEN;
				}	
				for (i = 0; i < n; i++)
				{
					backData[i] = MFRC522_ReadRegister(MFRC522_REG_FIFO_DATA, spi_type);		// Reading the received data in FIFO
				}
			}
		} 
		else
		{
			status = MI_ERR;
		}
	}
	return status;
}

void MFRC522_Reset(SPI_TYPE spi_type)
{
	MFRC522_WriteRegister(MFRC522_REG_COMMAND, PCD_RESETPHASE, spi_type);
}

void MFRC522_AntennaOn(SPI_TYPE spi_type)
{
	uint8_t temp;

	temp = MFRC522_ReadRegister(MFRC522_REG_TX_CONTROL, spi_type);
	if (!(temp & 0x03))//Tx1RFEn和Tx1RFEn==0
	{		
		MFRC522_SetBitMask(MFRC522_REG_TX_CONTROL, 0x03, spi_type);
	}
}

void MFRC522_AntennaOff(SPI_TYPE spi_type)
{
	MFRC522_ClearBitMask(MFRC522_REG_TX_CONTROL, 0x03, spi_type);
}


/*
功    能: 寻卡
参数说明: req_code[IN]:寻卡方式
                0x52   = 寻感应区内所有符合14443A标准的卡
                0x26   = 寻未进入休眠状态的卡
          			pTagType[OUT]:卡片类型代码
                0x4400 = Mifare_UltraLight
                0x0400 = Mifare_One(S50)
                0x0200 = Mifare_One(S70)
                0x0800 = Mifare_Pro(X)
                0x4403 = Mifare_DESFire
返 回 值: 成功返回MI_OK
*/

uint8_t MFRC522_Request(uint8_t reqMode, uint8_t* TagType, SPI_TYPE spi_type)
{
	uint8_t status; 
	uint8_t ComMF522Buf[MFRC522_MAX_LEN];
	uint16_t Len;																	

	MFRC522_ClearBitMask(MFRC522_REG_STATUS2,0x08,spi_type);
	MFRC522_WriteRegister(MFRC522_REG_BIT_FRAMING, 0x07, spi_type);		// TxLastBists = BitFramingReg[2..0]
	MFRC522_SetBitMask(MFRC522_REG_TX_CONTROL,0x03,spi_type);
	
	ComMF522Buf[0] = reqMode;
	status = MFRC522_ToCard(PCD_TRANSCEIVE, ComMF522Buf, 1, ComMF522Buf, &Len, spi_type);
	if((status == MI_OK)&&(Len == 0x10))
	{
		*TagType = ComMF522Buf[0];
		*(TagType + 1) = ComMF522Buf[1];
	}
	return status;
}


/*
功    能: 防冲撞
参数说明: pSnr[OUT]:卡片序列号，4字节
返    回: 成功返回MI_OK
*/

uint8_t MFRC522_Anticoll(uint8_t* serNum, SPI_TYPE spi_type)
{
	uint8_t status;
	uint8_t i;
	uint8_t serNumCheck = 0;
	uint16_t unLen;
	uint8_t ComMF522Buf[MFRC522_MAX_LEN];

	
	MFRC522_ClearBitMask(MFRC522_REG_STATUS2,0x08,spi_type);
	MFRC522_WriteRegister(MFRC522_REG_BIT_FRAMING, 0x00, spi_type);
	MFRC522_ClearBitMask(MFRC522_REG_COLL,0x80,spi_type);
	
	ComMF522Buf[0] = PICC_ANTICOLL;
	ComMF522Buf[1] = 0x20;	
	status = MFRC522_ToCard(PCD_TRANSCEIVE, ComMF522Buf, 2, ComMF522Buf, &unLen, spi_type);
	
	if (status == MI_OK)
	{
		for (i = 0; i < 4; i++)
		{
			*(serNum + i) = ComMF522Buf[i];
			serNumCheck ^= serNum[i];
		}
		if (serNumCheck != ComMF522Buf[i])
		{
			status = MI_ERR;
		}
	}
	MFRC522_SetBitMask(MFRC522_REG_COLL,0x80,spi_type);
	
	return status;
}

/*
功    能：用MF522计算CRC16函数
参    数：
				*pIn ：要读数CRC的数据
				len：-数据长度
				*pOut：计算的CRC结果
*/
void MFRC522_CalculateCRC(uint8_t*  pIndata, uint8_t len, uint8_t* pOutData, SPI_TYPE spi_type) 
{
	uint8_t i, n;

	MFRC522_ClearBitMask(MFRC522_REG_DIV_IRQ, 0x04, spi_type);	
	MRFRC522_WriteReg(MFRC522_REG_COMMAND, PCD_IDLE,spi_type);	
	MFRC522_SetBitMask(MFRC522_REG_FIFO_LEVEL, 0x80, spi_type);							


	// Writing data to the FIFO	
	for (i = 0; i < len; i++)
	{
		MFRC522_WriteRegister(MFRC522_REG_FIFO_DATA, *(pIndata+i), spi_type);
	}
	MFRC522_WriteRegister(MFRC522_REG_COMMAND, PCD_CALCCRC, spi_type);

	// Wait CRC calculation is complete
	i = 0xFF;
	do 
	{
		n = MFRC522_ReadRegister(MFRC522_REG_DIV_IRQ, spi_type);
		i--;
	} while ((i!=0) && !(n&0x04));																						// CRCIrq = 1

	// Read CRC calculation result
	pOutData[0] = MFRC522_ReadRegister(MFRC522_REG_CRC_RESULT_L, spi_type);
	pOutData[1] = MFRC522_ReadRegister(MFRC522_REG_CRC_RESULT_M, spi_type);
}

/*
功能描述:选卡读取卡存储器容量
输入参数:serNum 传入卡序列号
返 回 值:成功返回卡容量
*/
uint8_t MFRC522_SelectTag(uint8_t* serNum, SPI_TYPE spi_type)
{	
	uint8_t ComMF522Buf[MFRC522_MAX_LEN];
	uint16_t Len;
	uint8_t i;
	uint8_t status;
	uint8_t size;
	
	ComMF522Buf[0] = PICC_ANTICOLL;
	ComMF522Buf[1] = 0x70;
	ComMF522Buf[6] = 0;
	for(i = 0; i< 4; i++)
	{
		ComMF522Buf[i+2] = *(serNum+i);
		ComMF522Buf[6] ^= *(serNum+i);
	}
	MFRC522_CalculateCRC(ComMF522Buf,7,&ComMF522Buf[7],spi_type);
	MFRC522_ClearBitMask(MFRC522_REG_STATUS2,0x08,spi_type);
	
	status = MFRC522_ToCard(PCD_TRANSCEIVE,ComMF522Buf,9,ComMF522Buf,&Len,spi_type);
	if((status == MI_OK)&&(Len == 0x18))
	{
		size = ComMF522Buf[0];
	}
	else
	{
		size = 0;
	}
	return size;
}

/*
功    能：验证卡片密码
参数说明：auth_mode[IN]: 密码验证模式
                 0x60 = 验证A密钥
                 0x61 = 验证B密钥 
          addr[IN]：块地址
          pKey[IN]：扇区密码
          pSnr[IN]：卡片序列号，4字节
返    回：成功返回MI_OK
*/

uint8_t MFRC522_Auth(uint8_t authMode, uint8_t BlockAddr, uint8_t* Sectorkey, uint8_t* serNum, SPI_TYPE spi_type)
{
	uint8_t status;
	uint16_t Len;
	uint8_t i;
	uint8_t ComMF522Buf[MFRC522_MAX_LEN]; 
	
	ComMF522Buf[0] = authMode;
	ComMF522Buf[1] = BlockAddr;
	
	for(i = 0; i<6; i++)
	{
		ComMF522Buf[i+2] = *(Sectorkey+i);
	}
	
	for(i = 0; i<6; i++)
	{
		ComMF522Buf[i+8] = *(serNum+i);
	}
	
	status = MFRC522_ToCard(PCD_AUTHENT,ComMF522Buf,12,ComMF522Buf,&Len,spi_type);
	if((status != MI_OK) || (!(MFRC522_ReadRegister(MFRC522_REG_STATUS2,spi_type)&0x08)))
	{
		status = MI_ERR;
	}
	
	return status;
}

/*
功    能：写数据到M1卡指定块
参数说明：addr：块地址
          p   ：向块写入的数据，16字节
返    回：成功返回MI_OK
*/ 

uint8_t MFRC522_Write(uint8_t blockAddr, uint8_t* writeData, SPI_TYPE spi_type)
{
	uint8_t status;
	uint16_t Len;
	uint8_t i;
	uint8_t ComMF522Buf[MFRC522_MAX_LEN];

	ComMF522Buf[0] = PICC_WRITE;
	ComMF522Buf[1] = blockAddr;
	MFRC522_CalculateCRC(ComMF522Buf,2,&ComMF522Buf[2],spi_type);
	status = MFRC522_ToCard(PCD_TRANSCEIVE,ComMF522Buf,4,ComMF522Buf,&Len,spi_type);
	if((status != MI_OK) || (Len != 4) || ((ComMF522Buf[0]&0x0F) != 0x0A))
	{
		status = MI_ERR;
	}
	
	if(status == MI_OK)
	{
		for(i = 0; i < 16; i++)
		{
			ComMF522Buf[i] = *(writeData+i);
		}
		MFRC522_CalculateCRC(ComMF522Buf,16,&ComMF522Buf[16],spi_type);
		
		status = MFRC522_ToCard(PCD_TRANSCEIVE,ComMF522Buf,18,ComMF522Buf,&Len,spi_type);
		
		if((status != MI_OK) || (Len != 4) || ((ComMF522Buf[0]&0x0F) != 0x0A))
		{
			status = MI_ERR;
		}
	}
	
	return status;
}

/////////////////////////////////////////////////////////////////////
//功    能：写入钱包金额
//参数说明: ucAddr[IN]：块地址
//          pData：写入的金额
//返    回: 成功返回MI_OK
/////////////////////////////////////////////////////////////////////
uint8_t WriteAmount( uint8_t ucAddr, uint32_t pData ,SPI_TYPE spi_type)
{
	char status;
	uint8_t ucComMF522Buf[16];
	ucComMF522Buf[0] = (pData&((uint32_t)0x000000ff));
	ucComMF522Buf[1] = (pData&((uint32_t)0x0000ff00))>>8;
	ucComMF522Buf[2] = (pData&((uint32_t)0x00ff0000))>>16;
	ucComMF522Buf[3] = (pData&((uint32_t)0xff000000))>>24;	
	
	ucComMF522Buf[4] = ~(pData&((uint32_t)0x000000ff));
	ucComMF522Buf[5] = ~(pData&((uint32_t)0x0000ff00))>>8;
	ucComMF522Buf[6] = ~(pData&((uint32_t)0x00ff0000))>>16;
	ucComMF522Buf[7] = ~(pData&((uint32_t)0xff000000))>>24;	
	
	ucComMF522Buf[8] = (pData&((uint32_t)0x000000ff));
	ucComMF522Buf[9] = (pData&((uint32_t)0x0000ff00))>>8;
	ucComMF522Buf[10] = (pData&((uint32_t)0x00ff0000))>>16;
	ucComMF522Buf[11] = (pData&((uint32_t)0xff000000))>>24;	
	
	ucComMF522Buf[12] = ucAddr;
	ucComMF522Buf[13] = ~ucAddr;
	ucComMF522Buf[14] = ucAddr;
	ucComMF522Buf[15] = ~ucAddr;
  status = MFRC522_Write(ucAddr,ucComMF522Buf,spi_type);
	return status;
}

/*
功    能：读取M1卡一块数据
参数说明： 
					addr：块地址
          p   ：读出的块数据，16字节
返    回：成功返回MI_OK
*/ 

uint8_t MFRC522_Read(uint8_t blockAddr, uint8_t* recvData, SPI_TYPE spi_type)
{
	uint8_t i;
	uint8_t status;
	uint16_t Len;
	uint8_t ComMF522Buf[MFRC522_MAX_LEN];
	
	ComMF522Buf[0] = PICC_READ;
	ComMF522Buf[1] = blockAddr;
	MFRC522_CalculateCRC(ComMF522Buf,2,&ComMF522Buf[2],spi_type);
	
	status = MFRC522_ToCard(PCD_TRANSCEIVE,ComMF522Buf,4,ComMF522Buf,&Len,spi_type);
	
	if((status == MI_OK) && (Len == 0x90))
	{
		for(i = 0; i< 16; i++)
		{
			*(recvData+i) = ComMF522Buf[i];
		}
	}
	else
	{
		status = MI_ERR;
	}
	return status;
}

/////////////////////////////////////////////////////////////////////
//功    能：读取钱包金额
//参数说明: ucAddr[IN]：块地址
//          *pData：读出的金额
//返    回: 成功返回MI_OK
/////////////////////////////////////////////////////////////////////
uint8_t ReadAmount( uint8_t ucAddr, uint32_t *pData, SPI_TYPE spi_type)
{
	
	uint8_t status = MI_ERR;
	uint8_t i;
	uint8_t ucComMF522Buf[16];
	
  status = MFRC522_Read(ucAddr,ucComMF522Buf,spi_type);
	if(status != MI_OK)
	{
		return status;
	}
	for(i=0;i<4;i++)
	{
		if((ucComMF522Buf[i] != ucComMF522Buf[i+8]) && (ucComMF522Buf[i] != ~ucComMF522Buf[i+4]))//验证一下是不是钱包的数据
		{
			break;
		}
	}
	if(i == 4)
	{
		status = MI_OK;
		*pData = ucComMF522Buf[0] + (ucComMF522Buf[1]<<8) + (ucComMF522Buf[2]<<16) + (ucComMF522Buf[3]<<24);
	}
	else
	{
		status = MI_ERR;
		*pData = 0;
	}
  return status;	
}


/*
功    能：命令卡片进入休眠状态
返    回：成功返回MI_OK
*/

void MFRC522_Halt(SPI_TYPE spi_type) 
{
	uint16_t unLen;
	uint8_t buff[4]; 

	buff[0] = PICC_HALT;
	buff[1] = 0;
	MFRC522_CalculateCRC(buff, 2, &buff[2], spi_type);
	MFRC522_ToCard(PCD_TRANSCEIVE, buff, 4, buff, &unLen, spi_type);
}


void MFRC522_Init(SPI_TYPE spi_type)
{	
	MFRC522_Reset(spi_type);
	while((MFRC522_ReadRegister(MFRC522_REG_COMMAND,spi_type)&MFRC522_REG_RESERVED10) != RESET);//看看PowerDown位是否为0
	MFRC522_WriteRegister(MFRC522_REG_MODE, 0x3D, spi_type);//CRCPreset-6363h
	MFRC522_WriteRegister(MFRC522_REG_T_RELOAD_L, 30, spi_type);           
	MFRC522_WriteRegister(MFRC522_REG_T_RELOAD_H, 0, spi_type);
	MFRC522_WriteRegister(MFRC522_REG_T_MODE, 0x8D, spi_type);
	MFRC522_WriteRegister(MFRC522_REG_T_PRESCALER, 0x3E, spi_type);//13.56M/(2*62+1)=108.48KHZ
	MFRC522_WriteRegister(MFRC522_REG_TX_AUTO, 0x40, spi_type);
	MFRC522_WriteRegister(MFRC522_REG_RF_CFG, 0x70, spi_type);// 48dB gain	
	MFRC522_AntennaOn(spi_type);	
	printf("MFRC522_Init is Ok\r\n");
}

/**
  * @brief  判断 ucAddr 是否数据块
  * @param  ucAddr，块绝对地址（0-63）
  * @retval 返回值 1:是数据块；0:不是数据块
  */
uint8_t IsDataBlock(uint8_t addr)
{
  if(addr == 0)
  {
    printf("第0扇区的块0不可更改,不应对其进行操作\r\n");
    return 0;
  }
  /* 如果是数据块(不包含数据块0) */
  if( (addr<64) && (((addr+1)%4) != 0) )
  {
    return 1;
  } 
  printf("块地址不是指向数据块\r\n");
	
  return 0;
}

/**
  * @brief  写 pData 字符串到M1卡中的数据块
  * @param  ucAddr，数据块地址（不能写入控制块）
  * @param  pData，写入的数据，16字节
  * @retval 状态值= MI_OK，成功
  */
uint8_t PcdWriteString( uint8_t addr, uint8_t * pData, SPI_TYPE spi_type)
{
  /* 如果是数据块(不包含数据块0)，则写入 */
  if( IsDataBlock(addr) )
  {
    return MFRC522_Write(addr, pData,spi_type);
  }

  return MI_ERR;
}

/**
  * @brief  读取M1卡中的一块数据到 pData
  * @param  ucAddr，数据块地址（不读取控制块）
  * @param  pData，读出的数据，16字节
  * @retval 状态值= MI_OK，成功
  */
uint8_t PcdReadString(uint8_t ucAddr, uint8_t * pData, SPI_TYPE spi_type)
{
  /* 如果是数据块(不包含数据块0)，则读取 */
  if(IsDataBlock(ucAddr))
  {
    return MFRC522_Read(ucAddr, pData,spi_type);
  }

  return MI_ERR;
}

float mfrc_Temp[2] = {0.1,0.2};
float rmfrc_Temp[2];

uint8_t KeyValue[6] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
void My_ScanCard(MFRC522_Typedef * rc522, SPI_TYPE spi_type)
{	
	uint8_t ReadData[20];
	uint8_t ucComMF522Buf[16] = {1,2,3};
	if((rc522->StatusReturn = MFRC522_Request(PICC_REQALL,rc522->CardID,spi_type)) != MI_OK)
	{
//		printf("ScanCard Failed\r\n");
		rc522->StatusReturn = MFRC522_Request(PICC_REQALL,rc522->CardID,spi_type);
	}
	
	if(rc522->StatusReturn == MI_OK)
	{
		if(MFRC522_Anticoll(rc522->CardID,spi_type) == MI_OK)
		{
			rc522->CardSize = MFRC522_SelectTag(rc522->CardID,spi_type);	
			
			MFRC522_Auth(PICC_AUTHENT1B,6,KeyValue,rc522->CardID,spi_type);
			

//			PcdWriteString(6,(void*)mfrc_Temp,spi_type);
//			PcdReadString(6,(void*)rmfrc_Temp,spi_type);
//			printf("READ:%f\r\n",rmfrc_Temp[0]);

			if(MFRC522_Write(6,ucComMF522Buf,spi_type) == MI_OK)
			{
				MFRC522_Read(6,ReadData,spi_type);
				printf("READ:%d\r\n",ReadData[2]);
			}
//			MFRC522_Write(6,ucComMF522Buf,spi_type);
			
//			MFRC522_Auth(PICC_AUTHENT1B,4,KeyValue,rc522->CardID,spi_type);
////			PcdWriteString(4,(uint8_t*)"This is JackTJH",spi_type);
//			PcdReadString(4,ReadData,spi_type);
//			printf("%s\r\n",ReadData);
			
			
//			MFRC522_Auth(PICC_AUTHENT1B,5,KeyValue,rc522->CardID,spi_type);
////			WriteAmount(5,rc522->WriteValue,spi_type);
//			if(ReadAmount(5,&rc522->ReadValue,spi_type) == MI_OK)
//			{
//				sprintf ((char *)rc522->PrintfStr, "CardID: %02X%02X%02X%02X,Size:%d",rc522->CardID[0], rc522->CardID[1], rc522->CardID[2],rc522->CardID[3],rc522->CardSize);
//				printf ("%s\r\n",rc522->PrintfStr);  //打印卡片ID	
//				printf ("Balance:%d\r\n",rc522->ReadValue);
//				MFRC522_Halt(spi_type);
//			}			
		}
	}
}

