#include "ad7746.h"
#include "led.h"
#include	"delay.h"
#include "uart.h"

static volatile uint8_t ad774x_status_flag = 0;

/**
*	@brief					AD7745和AD7746起始复位(手册里面说了要延时至少150us，最多200us)
*	@param					i2c_type那种类型--I2C1MODE(H_I2C)在jack_i2c里面有宏定义
*	@param					i2cx使用的哪个I2C--JI2CX
* @retval					无
*/
void AD774X_Init_Reset(I2C_TYPE i2c_type,I2CX_Typedef i2cx)
{
	JI2C_Start(i2cx);
	JI2C_SendByte(0x90,i2cx);
	JI2C_WaitAck(i2cx);
	
	JI2C_SendByte(0xBF,i2cx);
	JI2C_WaitAck(i2cx);
	JI2C_Stop(i2cx);	
	Delay_us(200);
}

uint8_t AD774X_GetStatusFlag(AD774X_Status status,I2C_TYPE i2c_type,I2CX_Typedef i2cx)
{
	JI2C_Start(i2cx);
	JI2C_SendByte(0x90,i2cx);
	JI2C_WaitAck(i2cx);
	
	JI2C_SendByte(0x00,i2cx);
	JI2C_WaitAck(i2cx);
	
	JI2C_Start(i2cx);
	JI2C_SendByte(0x91,i2cx);
	JI2C_WaitAck(i2cx);
	
	ad774x_status_flag = JI2C_ReadByte(i2cx);
	JI2C_MasterAck(0,i2cx);
	JI2C_Stop(i2cx);
	
//	printf("%#x\r\n",ad774x_status_flag);
	
	return (ad774x_status_flag & (1<<(uint8_t)status));
}

void AD774X_WriteByte(uint8_t data, uint8_t reg,I2C_TYPE i2c_type,I2CX_Typedef i2cx)
{
	JI2C_Start(i2cx);
	JI2C_SendByte(0x90,i2cx);
	JI2C_WaitAck(i2cx);
	
	JI2C_SendByte(reg,i2cx);
	JI2C_WaitAck(i2cx);

	JI2C_SendByte(data,i2cx);
	JI2C_WaitAck(i2cx);
	
//	if(JI2C_WaitAck(i2cx))
//	{
//		LED_On(CORE_LED);
//	}
	
	JI2C_Stop(i2cx);	
}

uint8_t AD774X_ReadByte(uint8_t reg,I2C_TYPE i2c_type,I2CX_Typedef i2cx)
{
	uint8_t read = 0;
	
	JI2C_Start(i2cx);
	JI2C_SendByte(0x90,i2cx);
	JI2C_WaitAck(i2cx);
	
	JI2C_SendByte(reg,i2cx);
	JI2C_WaitAck(i2cx);
	
	JI2C_Start(i2cx);
	JI2C_SendByte(0x91,i2cx);
	JI2C_WaitAck(i2cx);
	
	read = JI2C_ReadByte(i2cx);
	JI2C_MasterAck(0,i2cx);
	JI2C_Stop(i2cx);
	
	return read;
}

void AD774X_WriteBytes(uint8_t *data, uint8_t reg,uint8_t len,I2C_TYPE i2c_type,I2CX_Typedef i2cx)
{
	JI2C_Start(i2cx);
	JI2C_SendByte(0x90,i2cx);
	JI2C_WaitAck(i2cx);
	
	JI2C_SendByte(reg,i2cx);
	JI2C_WaitAck(i2cx);
	
	for(uint8_t i = 0; i < len; i++)
	{
		JI2C_SendByte(data[i],i2cx);
		JI2C_WaitAck(i2cx);
	}
	
	JI2C_Stop(i2cx);	
}

void AD774X_ReadBytes(uint8_t *buf,uint8_t reg,uint8_t len,I2C_TYPE i2c_type,I2CX_Typedef i2cx)
{
	JI2C_Start(i2cx);
	JI2C_SendByte(0x90,i2cx);
	JI2C_WaitAck(i2cx);
	
	JI2C_SendByte(reg,i2cx);
	JI2C_WaitAck(i2cx);
	
	JI2C_Start(i2cx);
	JI2C_SendByte(0x91,i2cx);
	JI2C_WaitAck(i2cx);
	
	while(len)
	{
		if(len == 1)
		{
			*buf = JI2C_ReadByte(i2cx);
			JI2C_MasterAck(0,i2cx);
		}
		else
		{
			*buf = JI2C_ReadByte(i2cx);
			JI2C_MasterAck(1,i2cx);
		}
		len--;
		buf++;
	}
	JI2C_Stop(i2cx);
}





void AD7746_Init(I2C_TYPE i2c_type,I2CX_Typedef i2cx)
{
	AD774X_Init_Reset(i2c_type,i2cx);
	AD774X_WriteByte(CAPEN,CapSetup,i2c_type,i2cx);
	AD774X_WriteByte(EMPTY,VTSetup,i2c_type,i2cx);
	AD774X_WriteByte(EXCA|EXCLVL1|EXCLVL0,EXCSetup,i2c_type,i2cx);
	AD774X_WriteByte(CAPF2|CAPF1|CAPF0|MD0,CONFIGURATION,i2c_type,i2cx);
	AD774X_WriteByte(EMPTY,DACAREGISTER,i2c_type,i2cx);
	AD774X_WriteByte(EMPTY,DACBREGISTER,i2c_type,i2cx);
}

uint32_t AD774X_GetCapacitance(I2C_TYPE i2c_type,I2CX_Typedef i2cx)
{
	uint32_t capacitance = 0;
	uint8_t buffer[3];
	buffer[0] = AD774X_ReadByte(CapDataH,i2c_type,i2cx);
	buffer[1] = AD774X_ReadByte(CapDataM,i2c_type,i2cx);
	buffer[2] = AD774X_ReadByte(CapDataL,i2c_type,i2cx);
	capacitance = ((uint32_t)buffer[0]<<16) | ((uint32_t)buffer[1]<<8) | ((uint32_t)buffer[2]);
	return capacitance;
}

float AD7746_CapConvert(uint8_t capdac,I2C_TYPE i2c_type,I2CX_Typedef i2cx)
{
	uint32_t data;
	float capconvert = 0;
	data = AD774X_GetCapacitance(i2c_type,i2cx);
	capconvert = ((float)data/16777216)*8.192 + (capdac - 4);
	return capconvert;	
}
