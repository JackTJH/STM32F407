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
	SONIC_GPIO_Init();															//超声波初始化
	GeneralTimer_InputCaptureInit();								//输入捕获设置
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
			Sonic_Distance = HC_SR04_GetDistance1();
    }
	}
}




