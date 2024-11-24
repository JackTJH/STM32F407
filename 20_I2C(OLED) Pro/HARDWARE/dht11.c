#include "dht11.h"
#include "delay.h"
#include "uart.h"
#include <stdlib.h>

void DHT11_Out(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(DHT11_GPIO_RCC_PORT, ENABLE); 

	GPIO_InitStructure.GPIO_Pin = DHT11_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(DHT11_PORT, &GPIO_InitStructure);
	DHT11_DQ(1);
}

void DHT11_In(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(DHT11_GPIO_RCC_PORT, ENABLE); 

	GPIO_InitStructure.GPIO_Pin = DHT11_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init(DHT11_PORT, &GPIO_InitStructure);
}

void DHT11_Start(void)
{
	DHT11_Out();
	
	DHT11_DQ(0);
	Delay_ms(20);
	
	DHT11_DQ(1);
	Delay_us(30);
}

uint8_t DHT11_Check(void)
{
	uint8_t i = 0;
	
	DHT11_In();
	while((DHT11_DQ_Read() == SET) && (i < 80))
	{
		i++;
		Delay_us(1);
	}
	if(i >= 80)
	{
		return DHT11_ONE_ERR;
	}
	i = 0;
	
	while((DHT11_DQ_Read() == RESET) && (i < 100))
	{
		i++;
		Delay_us(1);
	}
	if(i >= 100)
	{
		return DHT11_TWO_ERR;
	}
	i = 0;
	
	return DHT11_INITIAL_SUCC;
}

uint8_t DHT11_ReadBit(void)
{
	uint16_t i = 0;
	
	while((DHT11_DQ_Read() != RESET) && i < 100)
	{
		i++;
		Delay_us(1);
	}
	i = 0;
	
	while((DHT11_DQ_Read() != SET) && i < 100)
	{
		i++;
		Delay_us(1);
	}
	
	Delay_us(65);
	if(DHT11_DQ_Read() == SET)
	{
		return 1;
	}
	if(DHT11_DQ_Read() == RESET)
	{
		return 0;
	}
	
	return DHT11_READ_BIT_FAILED;
}


uint8_t DHT11_ReadBytes(void)
{
	uint16_t i = 0;
	uint8_t data = 0;
	
	for(i = 0; i<8; i++)
	{
		data |= DHT11_ReadBit()<<(7-i);
	}
	
	return data;
}

void DHT11_ReadData(uint8_t *dat)
{
	uint8_t ret;
	uint8_t i = 0;
	
	DHT11_Start();
	ret = DHT11_Check();
	if(ret != DHT11_INITIAL_SUCC)
	{
		printf("DHT11_Initial_Failed\r\n");
	}
	else
	{
		for(i = 0; i<5; i++)
		{
			dat[i] = DHT11_ReadBytes();
		}
	}
}

void DHT11_Calculator(float *c_dat)
{
//	uint8_t dat[5];
	
	uint8_t *dat = (uint8_t *)malloc(5 * sizeof(uint8_t));  // 动态分配5个字节的内存
	if (dat == NULL)
	{
		printf("Memory allocation failed\r\n");
		return;
	}
	
//	uint8_t *dat = NULL;
	DHT11_ReadData(dat);
	if(dat[0] + dat[1] + dat[2] + dat[3] == dat[4])
	{
		c_dat[0] = dat[0] + dat[1] / 10.0;
		c_dat[1] = dat[2] + dat[3] / 10.0;
	}
	free(dat);  // 释放动态分配的内存
}

//void DHT11_Calculator(float *c_dat)
//{
//	uint8_t *dat;
//	DHT11_ReadData(dat);
//	if(dat[0] + dat[1] + dat[2] + dat[3] == dat[4])
//	{
//		// 将湿度和温度转换为浮点数
//		float humidity = dat[0] + dat[1] / 10.0;
//		float temperature = dat[2] + dat[3] / 10.0;
//	
//		// 将计算结果存储在输出指针中
//		c_dat[0] = humidity;
//		c_dat[1] = temperature;
//	
//		// 输出湿度和温度
//		printf("Humidity: %.1f%%, Temperature: %.1f°C\r\n", c_dat[0], c_dat[1]);
//	}
////	else
////	{
////		printf("ACCESS_Failed\r\n");
////	}
//}

