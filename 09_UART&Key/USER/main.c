#include "stm32f4xx.h"
#include "base_timer.h"
#include "pwm.h"
#include "delay.h"
#include "led.h"
#include "uart.h"
#include "button.h"
#include <stdbool.h>

__IO uint16_t Timer_Cnt1s;
__IO uint16_t Timer_Cnt2s;
__IO uint8_t Key_cnt = 0;
__IO bool Press_Flag = 0;
__IO uint32_t Press_Time = 0;
__IO bool Button_Timer_En = 0;
__IO uint8_t Test = 0;

void TIM6_DAC_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM6,TIM_IT_Update) ==SET)
	{
		if(Get_Button_Status(CORE_BUTTON) == SET && Press_Flag == 0)
		{
			Key_cnt++;
			Press_Flag = 1;
//			Button_Timer_En = 1;
		}
		else if(Get_Button_Status(CORE_BUTTON) == RESET)
		{
			Press_Flag = 0;
		}
		
		if(Press_Flag == 1)
		{
			Press_Time++;
		}
		
		if(++Timer_Cnt1s >= 1000)
		{
			Timer_Cnt1s = 0;
			printf("Run_Time:%d,Key_Cnt:%d,Press_Time:%d,Status:%d\r\n",Timer_Cnt2s,Key_cnt,Press_Time,Test);
		}
		if(Button_Timer_En == 1)
		{
			if(++Timer_Cnt2s >= 2000)
			{
				Timer_Cnt2s = 0;
				Press_Time = 0;
				Key_cnt = 0;
				Button_Timer_En = 0;
			}
		}
		TIM_ClearITPendingBit(TIM6,TIM_IT_Update);
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

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);	//中断优先级分组
	LED_Init(CORE_LED);															//初始化核心板LED
	Button_Init(CORE_BUTTON);												//按键初始化
	System_Tick_Init();															//滴答定时器初始化
	BASE_Timer_Init(84,1000);												//基本定时器初始化,这里参数配置的1ms中断
	PWM_Config_Init(84,100);												//方波的频率为10k
	UART_Init(115200);															//串口初始化UART,PA9-PA10
	printf("All Initial is OK\r\n");
	while(1)
	{
		if(Key_cnt == 1)
		{
			Test = 1;
			LED_Toggle(CORE_LED);
		}
//		else if(Timer_Cnt2s >= 1500 && Key_cnt == 2)
//		{
//			Test = 2;
//		}
//		else if(Timer_Cnt2s >= 1500 && Key_cnt == 1 && Press_Time >= 1000)
//		{
//			Test = 3;
//			LED_Toggle(CORE_LED);
//		}

	}
}




