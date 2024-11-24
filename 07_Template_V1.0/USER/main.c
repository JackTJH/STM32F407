#include "stm32f4xx.h"
#include "base_timer.h"
#include "pwm.h"
#include "delay.h"
#include "led.h"
#include <stdbool.h>

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


uint16_t Timer_Cnt1s;
__IO uint8_t led_pwm = 0;
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

	LED_Init(CORE_LED);				//��ʼ�����İ�LED
	Key_Init();						//������ʼ��
	System_Tick_Init();				//�δ�ʱ����ʼ��
	BASE_Timer_Init(84,1000);		//������ʱ����ʼ��,����������õ�1ms�ж�
	PWM_Config_Init(84,100);		//������Ƶ��Ϊ10k
	
	while(1)
	{
		TIM_SetCompare1(PWM_TIM,led_pwm);
	}
}



