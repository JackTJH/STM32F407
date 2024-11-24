#include "stm32f4xx.h"
#include "base_timer.h"
#include "pwm.h"
#include <stdbool.h>

void LED_Init(void)
{
//	GPIO_InitTypeDef  GPIO_InitStructure;
	
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
	
	
	//配置按键中断
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

void System_Tick_Init(void)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
}

/** \brief  Delay_us

    The function is to delay.

	\note nus do not exceed 798000
    
 */
void Delay_us(uint32_t nus)
{
	SysTick->CTRL = 0;
	SysTick->LOAD = nus*21-1;
	SysTick->VAL = 0;
	SysTick->CTRL = SysTick_CTRL_ENABLE_Msk;
	while((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == 0);
	SysTick->CTRL = 0;
}


/** \brief  Delay_ms

    The function is to delay.

	\note nms do not exceed 798
    
 */
void Delay_ms(uint32_t nms)
{
	SysTick->CTRL = 0;
	SysTick->LOAD = nms*21*1000-1;
	SysTick->VAL = 0;
	SysTick->CTRL = SysTick_CTRL_ENABLE_Msk;
	while((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == 0);
	SysTick->CTRL = 0;
}


uint16_t Timer_Cnt1s;
//uint32_t led_pwm = 0;
__IO uint32_t led_pwm = 0;
bool PWM_Dir = 0;

void TIM6_DAC_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM6,TIM_IT_Update) ==SET)
	{
		if(++Timer_Cnt1s >= 1000)
		{
			GPIO_ToggleBits(GPIOB,GPIO_Pin_2);
			if(PWM_Dir == 0) led_pwm += 20;
			else led_pwm -= 20;
			if(led_pwm == 100)
			{
				PWM_Dir = 1;
			}
			if(led_pwm == 0)
			{
				PWM_Dir = 0;
			}
			Timer_Cnt1s = 0;
		}
		TIM_ClearITPendingBit(TIM6,TIM_IT_Update);
	}

}

int main(int argc,char *argv[])
{
	
	uint8_t LED_Pluse = 0;

	LED_Init();						//初始化核心板LED
	Key_Init();						//按键初始化
	System_Tick_Init();				//滴答定时器初始化
	BASE_Timer_Init(84,1000);		//基本定时器初始化,这里参数配置的1ms中断
	PWM_Config_Init(84,100);		//方波的频率为10k
	
	while(1)
	{		
		for(LED_Pluse = 0; LED_Pluse<100; LED_Pluse += 10)
		{
//			PWM_TIM->CCR1 = LED_Pluse;
			TIM_SetCompare1(PWM_TIM,LED_Pluse);
			Delay_ms(300);
		}
		
		for(LED_Pluse = 100; LED_Pluse>0; LED_Pluse -= 10)
		{
//			PWM_TIM->CCR1 = LED_Pluse;
			TIM_SetCompare1(PWM_TIM,LED_Pluse);
			Delay_ms(300);
		}
		
//		TIM_SetCompare1(PWM_TIM,led_pwm);
//		PWM_TIM->CCR1 = led_pwm;
	}
}




