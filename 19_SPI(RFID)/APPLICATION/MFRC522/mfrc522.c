#include "mfrc522.h"

void RC522_GPIO_Init(void)
{
	RCC_AHB1PeriphClockCmd(MFRCC_RST_GPIO_RCC, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = MFRCC_RST_GPIO_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(MFRCC_RST_GPIO_Port, &GPIO_InitStructure);
}

/******template******/
// 0-255数字转换为字符  0xFF--"FF"
void char_to_hex(uint8_t data, uint8_t *retStr) {
	uint8_t digits[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
	
	if (data < 16) {
		retStr[0] = '0';
		retStr[1] = digits[data];
	} else {
		retStr[0] = digits[(data & 0xF0)>>4];
		retStr[1] = digits[(data & 0x0F)];
	}
}

void MRFRC522_WriteReg(uint8_t address, uint8_t value, SPI_TYPE spi_type)
{
	SPI1_CS(0);
	SPI_SendByte(address,spi_type);
	SPI_SendByte(value,spi_type);
	SPI1_CS(1);
}

uint8_t MRFRC522_ReadReg(uint8_t address, SPI_TYPE spi_type) 
{
	uint8_t	val;

	SPI1_CS(0);
	SPI_SendByte(address,spi_type);
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

void MFRC522_WriteRegister(uint8_t addr, uint8_t val, SPI_TYPE spi_type) {
	addr = (addr << 1) & 0x7E;															// Address format: 0XXXXXX0
  MRFRC522_WriteReg(addr, val, spi_type);
}

uint8_t MFRC522_ReadRegister(uint8_t addr, SPI_TYPE spi_type) {
	uint8_t val;

	addr = ((addr << 1) & 0x7E) | 0x80;
	val = MRFRC522_ReadReg(addr, spi_type);
	return val;	
}

void MFRC522_SetBitMask(uint8_t reg, uint8_t mask, SPI_TYPE spi_type) 
{
	MFRC522_WriteRegister(reg, MFRC522_ReadRegister(reg,spi_type) | mask , spi_type);
}

void MFRC522_ClearBitMask(uint8_t reg, uint8_t mask, SPI_TYPE spi_type)
{
	MFRC522_WriteRegister(reg, MFRC522_ReadRegister(reg,spi_type) & (~mask), spi_type);
}

void MFRC522_CalculateCRC(uint8_t*  pIndata, uint8_t len, uint8_t* pOutData, SPI_TYPE spi_type) 
{
	uint8_t i, n;

	MFRC522_ClearBitMask(MFRC522_REG_DIV_IRQ, 0x04, spi_type);													// CRCIrq = 0
	MFRC522_SetBitMask(MFRC522_REG_FIFO_LEVEL, 0x80, spi_type);													// Clear the FIFO pointer
	// Write_MFRC522(CommandReg, PCD_IDLE);

	// Writing data to the FIFO	
	for (i = 0; i < len; i++) MFRC522_WriteRegister(MFRC522_REG_FIFO_DATA, *(pIndata+i), spi_type);
	MFRC522_WriteRegister(MFRC522_REG_COMMAND, PCD_CALCCRC, spi_type);

	// Wait CRC calculation is complete
	i = 0xFF;
	do {
		n = MFRC522_ReadRegister(MFRC522_REG_DIV_IRQ, spi_type);
		i--;
	} while ((i!=0) && !(n&0x04));																						// CRCIrq = 1

	// Read CRC calculation result
	pOutData[0] = MFRC522_ReadRegister(MFRC522_REG_CRC_RESULT_L, spi_type);
	pOutData[1] = MFRC522_ReadRegister(MFRC522_REG_CRC_RESULT_M, spi_type);
}

uint8_t MFRC522_Compare(uint8_t* CardID, uint8_t* CompareID)
{
	uint8_t i;
	for (i = 0; i < 5; i++) {
		if (CardID[i] != CompareID[i]) return MI_ERR;
	}
	return MI_OK;
}

uint8_t MFRC522_ToCard(uint8_t command, uint8_t* sendData, uint8_t sendLen, uint8_t* backData, uint16_t* backLen, SPI_TYPE spi_type)
{
	uint8_t status = MI_ERR;
	uint8_t irqEn = 0x00;
	uint8_t waitIRq = 0x00;
	uint8_t lastBits;
	uint8_t n;
	uint16_t i;

	switch (command) {
		case PCD_AUTHENT: {
			irqEn = 0x12;
			waitIRq = 0x10;
			break;
		}
		case PCD_TRANSCEIVE: {
			irqEn = 0x77;
			waitIRq = 0x30;
			break;
		}
		default:
		break;
	}

	MFRC522_WriteRegister(MFRC522_REG_COMM_IE_N, irqEn | 0x80, spi_type);
	MFRC522_ClearBitMask(MFRC522_REG_COMM_IRQ, 0x80, spi_type);
	MFRC522_SetBitMask(MFRC522_REG_FIFO_LEVEL, 0x80, spi_type);
	MFRC522_WriteRegister(MFRC522_REG_COMMAND, PCD_IDLE, spi_type);

	// Writing data to the FIFO
	for (i = 0; i < sendLen; i++) MFRC522_WriteRegister(MFRC522_REG_FIFO_DATA, sendData[i], spi_type);

	// Execute the command
	MFRC522_WriteRegister(MFRC522_REG_COMMAND, command, spi_type);
	if (command == PCD_TRANSCEIVE) MFRC522_SetBitMask(MFRC522_REG_BIT_FRAMING, 0x80, spi_type);		// StartSend=1,transmission of data starts 

	// Waiting to receive data to complete
	i = 2000;	// i according to the clock frequency adjustment, the operator M1 card maximum waiting time 25ms
	do {
		// CommIrqReg[7..0]
		// Set1 TxIRq RxIRq IdleIRq HiAlerIRq LoAlertIRq ErrIRq TimerIRq
		n = MFRC522_ReadRegister(MFRC522_REG_COMM_IRQ, spi_type);
		i--;
	} while ((i!=0) && !(n&0x01) && !(n&waitIRq));

	MFRC522_ClearBitMask(MFRC522_REG_BIT_FRAMING, 0x80, spi_type);																// StartSend=0

	if (i != 0)  {
		if (!(MFRC522_ReadRegister(MFRC522_REG_ERROR, spi_type) & 0x1B)) {
			status = MI_OK;
			if (n & irqEn & 0x01) status = MI_NOTAGERR;
			if (command == PCD_TRANSCEIVE) {
				n = MFRC522_ReadRegister(MFRC522_REG_FIFO_LEVEL, spi_type);
				lastBits = MFRC522_ReadRegister(MFRC522_REG_CONTROL, spi_type) & 0x07;
				if (lastBits) *backLen = (n-1)*8+lastBits; else *backLen = n*8;
				if (n == 0) n = 1;
				if (n > MFRC522_MAX_LEN) n = MFRC522_MAX_LEN;
				for (i = 0; i < n; i++) backData[i] = MFRC522_ReadRegister(MFRC522_REG_FIFO_DATA, spi_type);		// Reading the received data in FIFO
			}
		} else status = MI_ERR;
	}
	return status;
}

uint8_t MFRC522_Request(uint8_t reqMode, uint8_t* TagType, SPI_TYPE spi_type)
{
	uint8_t status;  
	uint16_t backBits;																			// The received data bits

	MFRC522_WriteRegister(MFRC522_REG_BIT_FRAMING, 0x07, spi_type);		// TxLastBists = BitFramingReg[2..0]
	TagType[0] = reqMode;
	status = MFRC522_ToCard(PCD_TRANSCEIVE, TagType, 1, TagType, &backBits, spi_type);
	if ((status != MI_OK) || (backBits != 0x10)) status = MI_ERR;
	return status;
}

uint8_t MFRC522_Anticoll(uint8_t* serNum, SPI_TYPE spi_type) {
	uint8_t status;
	uint8_t i;
	uint8_t serNumCheck = 0;
	uint16_t unLen;

	MFRC522_WriteRegister(MFRC522_REG_BIT_FRAMING, 0x00, spi_type);												// TxLastBists = BitFramingReg[2..0]
	serNum[0] = PICC_ANTICOLL;
	serNum[1] = 0x20;
	status = MFRC522_ToCard(PCD_TRANSCEIVE, serNum, 2, serNum, &unLen, spi_type);
	if (status == MI_OK) {
		// Check card serial number
		for (i = 0; i < 4; i++) serNumCheck ^= serNum[i];
		if (serNumCheck != serNum[i]) status = MI_ERR;
	}
	return status;
} 

uint8_t MFRC522_Check(uint8_t* id, SPI_TYPE spi_type)
{
	uint8_t status;
	status = MFRC522_Request(PICC_REQIDL, id, spi_type);							// Find cards, return card type
	if (status == MI_OK) status = MFRC522_Anticoll(id, spi_type);			// Card detected. Anti-collision, return card serial number 4 bytes
	MFRC522_Halt(spi_type);																					// Command card into hibernation 
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
	if (!(temp & 0x03)) MFRC522_SetBitMask(MFRC522_REG_TX_CONTROL, 0x03, spi_type);
}

void MFRC522_AntennaOff(SPI_TYPE spi_type)
{
	MFRC522_ClearBitMask(MFRC522_REG_TX_CONTROL, 0x03, spi_type);
}

void MFRC522_Halt(SPI_TYPE spi_type) 
{
	uint16_t unLen;
	uint8_t buff[4]; 

	buff[0] = PICC_HALT;
	buff[1] = 0;
	MFRC522_CalculateCRC(buff, 2, &buff[2], spi_type);
	MFRC522_ToCard(PCD_TRANSCEIVE, buff, 4, buff, &unLen, spi_type);
}

uint8_t MFRC522_Read(uint8_t blockAddr, uint8_t* recvData, SPI_TYPE spi_type)
{
	uint8_t status;
	uint16_t unLen;

	recvData[0] = PICC_READ;
	recvData[1] = blockAddr;
	MFRC522_CalculateCRC(recvData,2, &recvData[2], spi_type);
	status = MFRC522_ToCard(PCD_TRANSCEIVE, recvData, 4, recvData, &unLen, spi_type);
	if ((status != MI_OK) || (unLen != 0x90)) status = MI_ERR;
	return status;
}

uint8_t MFRC522_Write(uint8_t blockAddr, uint8_t* writeData, SPI_TYPE spi_type)
{
	uint8_t status;
	uint16_t recvBits;
	uint8_t i;
	uint8_t buff[18]; 

	buff[0] = PICC_WRITE;
	buff[1] = blockAddr;
	MFRC522_CalculateCRC(buff, 2, &buff[2], spi_type);
	status = MFRC522_ToCard(PCD_TRANSCEIVE, buff, 4, buff, &recvBits, spi_type);
	if ((status != MI_OK) || (recvBits != 4) || ((buff[0] & 0x0F) != 0x0A)) status = MI_ERR;
	if (status == MI_OK) {
		// Data to the FIFO write 16Byte
		for (i = 0; i < 16; i++) buff[i] = *(writeData+i);
		MFRC522_CalculateCRC(buff, 16, &buff[16], spi_type);
		status = MFRC522_ToCard(PCD_TRANSCEIVE, buff, 18, buff, &recvBits, spi_type);
		if ((status != MI_OK) || (recvBits != 4) || ((buff[0] & 0x0F) != 0x0A)) status = MI_ERR;
	}
	return status;
}

uint8_t MFRC522_Auth(uint8_t authMode, uint8_t BlockAddr, uint8_t* Sectorkey, uint8_t* serNum, SPI_TYPE spi_type)
{
	uint8_t status;
	uint16_t recvBits;
	uint8_t i;
	uint8_t buff[12]; 

	// Verify the command block address + sector + password + card serial number
	buff[0] = authMode;
	buff[1] = BlockAddr;
	for (i = 0; i < 6; i++) buff[i+2] = *(Sectorkey+i);
	for (i=0; i<4; i++) buff[i+8] = *(serNum+i);
	status = MFRC522_ToCard(PCD_AUTHENT, buff, 12, buff, &recvBits, spi_type);
	if ((status != MI_OK) || (!(MFRC522_ReadRegister(MFRC522_REG_STATUS2, spi_type) & 0x08))) status = MI_ERR;
	return status;
}

uint8_t MFRC522_SelectTag(uint8_t* serNum, SPI_TYPE spi_type)
{
	uint8_t i;
	uint8_t status;
	uint8_t size;
	uint16_t recvBits;
	uint8_t buffer[9]; 

	buffer[0] = PICC_SElECTTAG;
	buffer[1] = 0x70;
	for (i = 0; i < 5; i++) buffer[i+2] = *(serNum+i);
	MFRC522_CalculateCRC(buffer, 7, &buffer[7], spi_type);		//??
	status = MFRC522_ToCard(PCD_TRANSCEIVE, buffer, 9, buffer, &recvBits, spi_type);
	if ((status == MI_OK) && (recvBits == 0x18)) size = buffer[0]; else size = 0;
	return size;
}

#define DEBUG_printf

/**
  * @brief  扫描RFID卡
  * @param  cardID 四字节数组
  * @retval 0扫描到RFID卡
  */
uint8_t RC522_cardScan(uint8_t *cardID, SPI_TYPE spi_type)
{
	if(!MFRC522_Request(PICC_REQIDL, cardID, spi_type)) {
		if(!MFRC522_Anticoll(cardID, spi_type)) {
			
#ifdef DEBUG_printf
			uint8_t cardIDString[8] = {0};
			for(uint8_t i = 0; i < 4; i++) {
				char_to_hex(cardID[i], cardIDString+i*2);
			}
			printf("card id:%s\n", cardIDString);
#endif
			return 0;
		}
	}
	return 1;
}

void MFRC522_Init(SPI_TYPE spi_type)
{
	MFRC522_Reset(spi_type);
	MFRC522_WriteRegister(MFRC522_REG_T_MODE, 0x8D, spi_type);
	MFRC522_WriteRegister(MFRC522_REG_T_PRESCALER, 0x3E, spi_type);
	MFRC522_WriteRegister(MFRC522_REG_T_RELOAD_L, 30, spi_type);           
	MFRC522_WriteRegister(MFRC522_REG_T_RELOAD_H, 0, spi_type);
	MFRC522_WriteRegister(MFRC522_REG_RF_CFG, 0x70, spi_type);				// 48dB gain	
	MFRC522_WriteRegister(MFRC522_REG_TX_AUTO, 0x40, spi_type);
	MFRC522_WriteRegister(MFRC522_REG_MODE, 0x3D, spi_type);
	MFRC522_AntennaOn(spi_type);																		// Open the antenna
}

