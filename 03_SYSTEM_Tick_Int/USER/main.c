#include "stm32f4xx.h"

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
	
	
	//���ð����ж�
	EXTI_InitTypeDef EXTI_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_Init(&EXTI_InitStructure);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,EXTI_PinSource0);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
}

void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0) == SET)
	{
		GPIO_ToggleBits(GPIOB,GPIO_Pin_2);
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
}

__IO uint32_t TimingDelay;

void Delay_us(__IO uint32_t nTime)
{ 
	TimingDelay = nTime;

	while(TimingDelay != 0);
}

void Delay_ms(__IO uint32_t nTime)
{ 
	TimingDelay = nTime*1000;

	while(TimingDelay != 0);
}

int main(int argc,char *argv[])
{

	LED_Init();		//��ʼ�����İ�LED
	Key_Init();		//������ʼ��
//	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
//	SysTick_Config(SystemCoreClock / 1000000);
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	SysTick_Config(SystemCoreClock / 8000000);
	while(1)
	{
		GPIO_ToggleBits(GPIOB,GPIO_Pin_2);
		Delay_ms(1000);
	}
}




