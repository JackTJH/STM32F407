#include "jack_i2c.h"
#include "delay.h"

GPIO_TypeDef* I2C_GPIO_PORT[I2Cn] = {I2C1_GPIO_Port,I2C2_GPIO_Port};

void JI2C_GPIO_Init(I2C_TYPE i2c_type,I2CX_Typedef i2cx)
{
	I2CX_GPIO_CLK_ENABLE(i2cx);
	
	GPIO_InitTypeDef GPIO_InitStructure;

	if(i2cx == JI2C1)
	{
		if(i2c_type == S_I2C)
		{
			GPIO_InitStructure.GPIO_Pin = I2C1_SCL_GPIO_Pin|I2C1_SDA_GPIO_Pin;
		}
		else
		{
			GPIO_InitStructure.GPIO_Pin = I2C1_SCL_GPIO_Pin;
		}
	}
	if(i2cx == JI2C2)
	{
		if(i2c_type == S_I2C)
		{
			GPIO_InitStructure.GPIO_Pin = I2C2_SCL_GPIO_Pin|I2C2_SDA_GPIO_Pin;
		}
		else
		{
			GPIO_InitStructure.GPIO_Pin = I2C2_SCL_GPIO_Pin;
		}
	}
	
	if(i2c_type == S_I2C)
	{
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	}
	else if(i2c_type == H_I2C)
	{
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	}
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(I2C_GPIO_PORT[i2cx], &GPIO_InitStructure); 
	
	if(i2c_type == H_I2C)
	{
		if(i2cx == JI2C1)
		{
			GPIO_InitStructure.GPIO_Pin = I2C1_SDA_GPIO_Pin;
			GPIO_Init(I2C_GPIO_PORT[i2cx], &GPIO_InitStructure); 
			GPIO_PinAFConfig(I2C_GPIO_PORT[i2cx], I2C1_SCL_GPIO_AFPin, I2C1_GPIO_AF);
			GPIO_PinAFConfig(I2C_GPIO_PORT[i2cx], I2C1_SDA_GPIO_AFPin, I2C1_GPIO_AF);
		}
		else if(i2cx == JI2C2)
		{
			GPIO_InitStructure.GPIO_Pin = I2C2_SDA_GPIO_Pin;
			GPIO_Init(I2C_GPIO_PORT[i2cx], &GPIO_InitStructure); 
			GPIO_PinAFConfig(I2C_GPIO_PORT[i2cx], I2C2_SCL_GPIO_AFPin, I2C2_GPIO_AF);
			GPIO_PinAFConfig(I2C_GPIO_PORT[i2cx], I2C2_SDA_GPIO_AFPin, I2C2_GPIO_AF);
		}
	}
	
	if(i2c_type == S_I2C)
	{
		if(i2cx == JI2C1)
		{
			I2C1_SCL(1);
			I2C1_SDA(1);
		}
		else if(i2cx == JI2C2)
		{
			I2C2_SCL(1);
			I2C2_SDA(1);
		}
	}
}

I2C_TypeDef* JI2X[I2Cn] = {I2C1,I2C2};

void JI2C_Init(I2C_TYPE i2c_type,I2CX_Typedef i2cx)
{
	JI2C_GPIO_Init(i2c_type,i2cx);
	
	if(i2c_type == H_I2C)
	{
		I2CX_CLK_ENABLE(i2cx);
		
		I2C_InitTypeDef I2C_InitStructure;
		I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
		I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
		I2C_InitStructure.I2C_ClockSpeed = 100000;
		I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
		I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
		I2C_InitStructure.I2C_OwnAddress1 = 0xBE;
		I2C_Init(JI2X[i2cx],&I2C_InitStructure);
		I2C_Cmd(JI2X[i2cx],ENABLE);
		I2C_AcknowledgeConfig(JI2X[i2cx], ENABLE);
	}
}


/******************************************这是软件模拟的I2C************************************************/
/**
 * @brief       产生IIC起始信号
 * @param       无
 * @retval      无
 */
void JI2C_Start(I2CX_Typedef i2cx)
{
	if(i2cx == JI2C1)
	{
		I2C1_SCL(1);
		I2C1_SDA(1);
		Delay_us(5);
		I2C1_SDA(0);
		Delay_us(5);
		I2C1_SCL(0);
		Delay_us(5);
	}
	else if(i2cx == JI2C2)
	{
		I2C2_SCL(1);
		I2C2_SDA(1);
		Delay_us(5);
		I2C2_SDA(0);
		Delay_us(5);
		I2C2_SCL(0);
		Delay_us(5);
	}
}

/**
 * @brief       产生IIC停止信号
 * @param       无
 * @retval      无
 */
void JI2C_Stop(I2CX_Typedef i2cx)
{	
	if(i2cx == JI2C1)
	{
		I2C1_SCL(0);
		I2C1_SDA(0);
		Delay_us(5);
		I2C1_SCL(1);
		Delay_us(5);
		I2C1_SDA(1);
		Delay_us(5);
	}
	else if(i2cx == JI2C2)
	{
		I2C2_SCL(0);
		I2C2_SDA(0);
		Delay_us(5);
		I2C2_SCL(1);
		Delay_us(5);
		I2C2_SDA(1);
		Delay_us(5);
	}
}

/**
 * @brief       等待应答信号到来
 * @param       无
 * @retval      0，接收应答失败
 *              1，接收应答成功
 */
uint8_t JI2C_WaitAck(I2CX_Typedef i2cx)
{
  uint8_t ack = 0;

	if(i2cx == JI2C1)
	{
		I2C1_SDA(1);
		Delay_us(5);
		I2C1_SCL(1);
		Delay_us(5);
		if(I2C1_ReadSDA() == SET)
		{
			ack = 0;//无应答
		}
		else
		{
			ack = 1;
		}
		I2C1_SCL(0);
		Delay_us(5);
	}
	else if(i2cx == JI2C2)
	{
		I2C2_SDA(1);
		Delay_us(5);
		I2C2_SCL(1);
		Delay_us(5);
		if(I2C2_ReadSDA() == SET)
		{
			ack = 0;//无应答
		}
		else
		{
			ack = 1;
		}
		I2C2_SCL(0);
		Delay_us(5);
	}
	
	return ack;
}

/**
 * @brief       主机给从机的一个应答信号(从机像主机发送数据)
 * @param       i2cx	第几个I2C
 * @param       ack		是否做出应答 0-不应答 1-应答 
 * @retval      无
 *              
 */
void JI2C_MasterAck(uint8_t ack, I2CX_Typedef i2cx)
{
	if(i2cx == JI2C1)
	{
		I2C1_SCL(0);
		I2C1_SDA(0);
		
		if(ack == 1)
		{
			I2C1_SDA(0);
		}
		else
		{
			I2C1_SDA(1);
		}
		Delay_us(5);
		I2C1_SCL(1);
		Delay_us(5);
		I2C1_SCL(0);
		Delay_us(5);
	}
//	else if(i2cx == JI2C1)
//	{
//		I2C2_SCL(0);
//		I2C2_SDA(0);
//		
//		if(ack == 1)
//		{
//			I2C2_SDA(0);
//		}
//		else
//		{
//			I2C2_SDA(1);
//		}
//		Delay_us(5);
//		I2C2_SCL(1);
//		Delay_us(5);
//		I2C2_SCL(0);
//		Delay_us(5);
//	}
}

/**
 * @brief       IIC发送一个字节
 * @param       data: 要发送的数据
 * @retval      无
 */
void JI2C_SendByte(uint8_t data,I2CX_Typedef i2cx)
{		
	if(i2cx == JI2C1)
	{
		I2C1_SCL(0);
		I2C1_SDA(0);
		for(uint8_t i = 0; i <8; i++)
		{
			if(data & 1<<(7-i))
			{
				I2C1_SDA(1);
			}
			else
			{
				I2C1_SDA(0);
			}
			Delay_us(5);
			I2C1_SCL(1);
			Delay_us(5);
			I2C1_SCL(0);
			Delay_us(5);
		}
		I2C1_SCL(1);
		Delay_us(5);
	}
	else if(i2cx == JI2C2)
	{
		I2C2_SCL(0);
		I2C2_SDA(0);
		for(uint8_t i = 0; i <8; i++)
		{
			if(data & 1<<(7-i))
			{
				I2C2_SDA(1);
			}
			else
			{
				I2C2_SDA(0);
			}
			Delay_us(5);
			I2C2_SCL(1);
			Delay_us(5);
			I2C2_SCL(0);
			Delay_us(5);
		}
		I2C2_SCL(1);
		Delay_us(5);
	}
}

/**
 * @brief       IIC读取一个字节
 * @param       ack:  ack=1时，发送ack; ack=0时，发送nack
 * @retval      接收到的数据
 */
uint8_t JI2C_ReadByte(I2CX_Typedef i2cx)
{
	uint8_t temp = 0;
	I2C1_SCL(0);
	for(uint8_t i = 0; i <8; i++)
	{
		I2C1_SCL(1);
		Delay_us(5);
		if(I2C1_ReadSDA())
		{
			temp |= 1<<(7-i);
		}
		I2C1_SCL(0);
		Delay_us(5);
	}
  return temp;
}

/******************************************这是软件模拟的I2C************************************************/

/**
  * 函    数：MPU6050等待事件
  * 参    数：同I2C_CheckEvent
  * 返 回 值：无
  */
void JI2CXX_WaitEvent(I2CX_Typedef i2cx, uint32_t I2C_EVENT)
{
	uint32_t Timeout;
	Timeout = 10000;									//给定超时计数时间
	while (I2C_CheckEvent(JI2X[i2cx], I2C_EVENT) != SUCCESS)	//循环等待指定事件
	{
		Timeout --;										//等待时，计数值自减
		if (Timeout == 0)								//自减到0后，等待超时
		{
			/*超时的错误处理代码，可以添加到此处*/
			break;										//跳出等待，不等了
		}
	}
}
