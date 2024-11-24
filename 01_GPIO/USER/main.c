#include "stm32f4xx.h"

#define RCC_AHB1ENR				(*(volatile unsigned int *)(0x40023800 + 0x30))
#define GPIOB_MODER				(*(volatile unsigned int *)(0x40020400 + 0x00))
#define GPIOB_OTYPER			(*(volatile unsigned int *)(0x40020400 + 0x04))
#define GPIOB_OSPEEDR			(*(volatile unsigned int *)(0x40020400 + 0x08))
#define GPIOB_PUPDR				(*(volatile unsigned int *)(0x40020400 + 0x0C))
#define GPIOB_ODR				(*(volatile unsigned int *)(0x40020400 + 0x14))
	
#define GPIOA_MODER				(*(volatile unsigned int *)(0x40020000 + 0x00))
#define GPIOA_IDR				(*(volatile unsigned int *)(0x40020000 + 0x10))

//GPIOx_MODER
void LED_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	/* GPIOG Peripheral clock enable */
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
//	RCC_AHB1ENR |= (1<<1);
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	GPIOB->MODER |= GPIO_MODER_MODER2_0;
//	GPIOB->OTYPER &= ~(1<<2);
	GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR2;
	GPIOB->PUPDR |= GPIO_PUPDR_PUPDR2_1;
//	GPIOB_MODER |= (1<<4);
//	GPIOB_OTYPER &= ~(1<<2);
//	GPIOB_OSPEEDR |= (3<<4);
//	GPIOB_PUPDR |= (2<<4);
	

	/* Configure PG6 and PG8 in output pushpull mode */
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void Key_Init(void)
{
//	GPIO_InitTypeDef  GPIO_InitStructure;
//	
//	/* GPIOG Peripheral clock enable */
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

//	/* Configure PG6 and PG8 in output pushpull mode */
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
//	RCC_AHB1ENR |= (1<<0);
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
//	GPIOA->MODER &= ~(0<<0);
//	GPIOA_MODER &= ~(0<<0);
}


int main(int argc,char *argv[])
{

	LED_Init();		//初始化核心板LED
	Key_Init();		//按键初始化

	while(1)
	{
//		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0) == SET)
		if((GPIOA->IDR & GPIO_IDR_IDR_0) == SET)
		{
//			GPIO_WriteBit(GPIOB,GPIO_Pin_2,Bit_SET);
//			GPIOB_ODR |= (1<<2);
			GPIOB->ODR |= GPIO_ODR_ODR_2;
		}
		else
		{
//			GPIO_WriteBit(GPIOB,GPIO_Pin_2,Bit_RESET);
			GPIOB->ODR &= ~GPIO_ODR_ODR_2;
//			GPIOB_ODR &= ~(1<<2);
		}
	}
}




