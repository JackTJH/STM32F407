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
	if(TIM_GetITStatus(TIM6,TIM_IT_Update) ==SET)
	{		
		TimeBaseUpdata();
		TIM_ClearITPendingBit(TIM6,TIM_IT_Update);
	}

}

/* ���벶��״̬(g_timxchy_cap_sta)
 * [7]  :0,û�гɹ��Ĳ���;1,�ɹ�����һ��.
 * [6]  :0,��û���񵽸ߵ�ƽ;1,�Ѿ����񵽸ߵ�ƽ��.
 * [5:0]:����ߵ�ƽ������Ĵ���,������63��,���������ֵ = 63*65536 + 65535 = 4194303
 *       ע��:Ϊ��ͨ��,����Ĭ��ARR��CCRy����16λ�Ĵ���,����32λ�Ķ�ʱ��(��:TIM5),Ҳֻ��16λʹ��
 *       ��1us�ļ���Ƶ��,����ʱ��Ϊ:4194303 us, Լ4.19��
 *
 *      (˵��һ�£�����32λ��ʱ����˵,1us��������1,���ʱ��:4294��)
 */
__IO uint8_t g_timxchy_cap_sta = 0;    /* ���벶��״̬ */
__IO uint16_t g_timxchy_cap_val = 0;   /* ���벶��ֵ */	  

void TIM3_IRQHandler(void)
{ 	
	if((g_timxchy_cap_sta & 0x80) == 0)//��û�гɹ�����
	{
		if(TIM_GetITStatus(GTIMER_INPUT,TIM_IT_Update) == SET)
		{
			if((g_timxchy_cap_sta & 0x40) == 0x40)
			{
				if((g_timxchy_cap_sta & 0x3F) == 0x3F)
				{
					GTIMER_INPUT->CCER &= ~(TIM_CCER_CC1P);  
					TIM_OC1PolarityConfig(GTIMER_INPUT,TIM_ICPolarity_Rising);
					g_timxchy_cap_sta |= 0x80;
					g_timxchy_cap_val = 0xFFFF;
				}
				else
				{
					g_timxchy_cap_sta++;
				}
			}
		}
		
		if(TIM_GetITStatus(GTIMER_INPUT,TIM_IT_CC1) == SET)
		{
			if((g_timxchy_cap_sta & 0x40) == 0x40)
			{
				g_timxchy_cap_sta |= 0x80;
				g_timxchy_cap_val = TIM_GetCapture1(GTIMER_INPUT);
				TIM_OC1PolarityConfig(GTIMER_INPUT,TIM_OCPolarity_High);
			}
			else
			{
				g_timxchy_cap_sta = 0;
				g_timxchy_cap_val = 0;
				g_timxchy_cap_sta |= 0x40;//������������
				TIM_Cmd(GTIMER_INPUT,DISABLE);
				TIM_SetCounter(GTIMER_INPUT,0);  
				TIM_OC1PolarityConfig(GTIMER_INPUT,TIM_OCPolarity_Low);
				TIM_Cmd(GTIMER_INPUT,ENABLE);
			}
		}
	}
	TIM_ClearITPendingBit(GTIMER_INPUT,TIM_IT_Update|TIM_IT_CC1);
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
	long long temp=0;  

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);	//�ж����ȼ�����
	LED_Init(CORE_LED);															//��ʼ�����İ�LED
	Button_Init(CORE_BUTTON);												//������ʼ��
	System_Tick_Init();															//�δ�ʱ����ʼ��
	BASE_Timer_Init(84,1000);												//������ʱ����ʼ��,����������õ�1ms�ж�
	PWM_Config_Init(84,100);												//������Ƶ��Ϊ10k
	UART_Init(115200);															//���ڳ�ʼ��UART,PA9-PA10
	SONIC_GPIO_Init();															//��������ʼ��
	GTIM_TIMx_Cap_Chy_Init(84,65536); //��1Mhz��Ƶ�ʼ��� 
	printf("All Initial is OK\r\n");
	while(1)
	{
		if(TimeBaseGetFlag(BASE_1000MS))
		{
			TimeBaseClearFlag(BASE_1000MS);
			printf("Distance:%f(cm)\r\n",Sonic_Distance);
    }
		
		if(TimeBaseGetFlag(BASE_100MS))
		{
			HC_SR04_Start();
			TimeBaseClearFlag(BASE_100MS);	
			if((g_timxchy_cap_sta & 0x80) == 0x80)
			{
				temp = g_timxchy_cap_sta & 0x3F;
				temp *= 0xFFFF;
				temp += g_timxchy_cap_val;
				g_timxchy_cap_sta = 0;
				Sonic_Distance = (temp/1e6)*170*100;
			}
    }
	}
}




