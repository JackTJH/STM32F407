#include "stm32f4xx.h"
#include "base_timer.h"
#include "pwm.h"
#include "delay.h"
#include "led.h"
#include "uart.h"
#include "button.h"
#include "sonic.h"
#include "general_timer.h"
#include <stdbool.h>

float Sonic_Distance = 0;

void TIM6_DAC_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM6,TIM_IT_Update) == SET)
	{		
		TimeBaseUpdata();
		TIM_ClearITPendingBit(TIM6,TIM_IT_Update);
	}

}

void TIM7_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM7,TIM_IT_Update) == SET)
	{		
		TIM_ClearITPendingBit(TIM7,TIM_IT_Update);
	}
}

void EXTI2_IRQHandler(void)
{

	if(EXTI_GetITStatus(EXTI_Line2) != RESET )
	{
		EXTI_ClearITPendingBit(EXTI_Line2);
		TIM_SetCounter(BASE_TIMER7,0);
		TIM_Cmd(BASE_TIMER7,ENABLE);
		while(ECHO_Read() == SET); 
		TIM_Cmd(BASE_TIMER7,DISABLE);
		Sonic_Distance = TIM_GetCounter(BASE_TIMER7)*340/200.0;
	}
}

//void USART1_IRQHandler(void)
//{
//	uint16_t ReceiveData = 0;
//	
//	if(USART_GetITStatus(UART,USART_IT_RXNE) != RESET)
//	{
//		USART_ClearITPendingBit(UART,USART_IT_RXNE);
//		ReceiveData = USART_ReceiveData(UART);
//		USART_SendData(UART,ReceiveData);
//		while(USART_GetFlagStatus(UART,USART_FLAG_TXE) == RESET);
//	}
//}

int main(int argc,char *argv[])
{

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);	//�ж����ȼ�����
	LED_Init(CORE_LED);															//��ʼ�����İ�LED
	Button_Init(CORE_BUTTON);												//������ʼ��
	System_Tick_Init();															//�δ�ʱ����ʼ��
	BASE_Timer_Init(84,1000);												//������ʱ����ʼ��,����������õ�1ms�ж�
	PWM_Config_Init(84,100);												//������Ƶ��Ϊ10k
	UART_Init(115200);															//���ڳ�ʼ��UART,PA9-PA10
	SONIC_GPIO_Init();															//��������ʼ��
	BASE_Timer7_Init();
	printf("All Initial is OK\r\n");
	while(1)
	{
		if(TimeBaseGetFlag(BASE_1000MS))
		{
			TimeBaseClearFlag(BASE_1000MS);
			printf("Sonic_Distance:%f\r\n",Sonic_Distance);
    }
		
		if(TimeBaseGetFlag(BASE_100MS))
		{
			TimeBaseClearFlag(BASE_100MS);
//			TRIG(1);
//			Delay_us(20);
//			TRIG(0);
//			Sonic_Distance = HC_SR04_GetDistance1();
    }
	}
}




