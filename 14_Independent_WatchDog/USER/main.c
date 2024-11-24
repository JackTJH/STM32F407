#include "stm32f4xx.h"
#include "base_timer.h"
#include "pwm.h"
#include "delay.h"
#include "led.h"
#include "uart.h"
#include "button.h"
#include "sonic.h"
#include "general_timer.h"
#include "dht11.h"
#include "iwdg.h"
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

void TIM6_DAC_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM6,TIM_IT_Update) ==SET)
	{		
		TimeBaseUpdata();
		TIM_ClearITPendingBit(TIM6,TIM_IT_Update);
	}

}


GtimerInput_Typedef Gtimer3Input = {0,0};
void TIM3_IRQHandler(void)
{ 	
	if((Gtimer3Input.gtimer_cap_sta & 0x80) == 0)//还没有成功捕获
	{
		if(TIM_GetITStatus(GTIMER_INPUT,TIM_IT_Update) == SET)
		{
			if((Gtimer3Input.gtimer_cap_sta & 0x40) == 0x40)
			{
				if((Gtimer3Input.gtimer_cap_sta & 0x3F) == 0x3F)
				{
					GTIMER_INPUT->CCER &= ~(TIM_CCER_CC1P);  
					TIM_OC1PolarityConfig(GTIMER_INPUT,TIM_ICPolarity_Rising);
					Gtimer3Input.gtimer_cap_sta |= 0x80;
					Gtimer3Input.gtimer_cap_val = 0xFFFF;
				}
				else
				{
					Gtimer3Input.gtimer_cap_sta++;
				}
			}
		}
		
		if(TIM_GetITStatus(GTIMER_INPUT,TIM_IT_CC1) == SET)
		{
			if((Gtimer3Input.gtimer_cap_sta & 0x40) == 0x40)
			{
				Gtimer3Input.gtimer_cap_sta |= 0x80;
				Gtimer3Input.gtimer_cap_val = TIM_GetCapture1(GTIMER_INPUT);
				TIM_OC1PolarityConfig(GTIMER_INPUT,TIM_OCPolarity_High);
			}
			else
			{
				Gtimer3Input.gtimer_cap_sta = 0;
				Gtimer3Input.gtimer_cap_val = 0;
				Gtimer3Input.gtimer_cap_sta |= 0x40;//捕获到了上升沿
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
//	if(USART_GetITStatus(UART1,USART_IT_RXNE) != RESET)
//	{
//		USART_ClearITPendingBit(UART1,USART_IT_RXNE);
//		ReceiveData = USART_ReceiveData(UART1);
//		USART_SendData(UART1,ReceiveData);
//		while(USART_GetFlagStatus(UART1,USART_FLAG_TXE) == RESET);
//	}
//}


UartReceive_TypeDef UartBLEReceive;
void USART2_IRQHandler(void)
{
	
	uint8_t TempData = 0;
	static uint8_t state = 0;
	static uint8_t TempDataIndex = 0;
	
	if(USART_GetITStatus(UART2,USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(UART2,USART_IT_RXNE);
		TempData = USART_ReceiveData(UART2);
		if(state == 0)
		{
			if(TempData == 'S')
			{
				state = 1;
			}
		}
		else if(state == 1)
		{
			if(TempData == 'E')
			{
				state = 2;
			}
			else
			{
				UartBLEReceive.ReceiveData[TempDataIndex] = TempData;
				TempDataIndex++;
			}
		}
		else if(state == 2)
		{
			if(TempData == '$')
			{
				UartBLEReceive.ReceiveFlag = 1;
				state = 0;
				TempDataIndex = 0;
			}
		}	
	}
}




int main(int argc,char *argv[])
{ 
	char *StrTemp = NULL;														//定义一个蓝牙串口缓冲，这里初始值要，防止野指针
	int num = 0;
	float Humi_Temp[2] = {0};
	float Sonic_Distance = 0;
	bool Sonic_OnorOff = 0;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);	//中断优先级分组
	LED_Init(CORE_LED);															//初始化核心板LED
	Button_Init(CORE_BUTTON);												//按键初始化
	System_Tick_Init();															//滴答定时器初始化
	BASE_Timer_Init(84,1000);												//基本定时器初始化,这里参数配置的1ms中断
	PWM_Config_Init(84,100);												//方波的频率为10k
	UARTx_Init(UART_1,9600);												//串口初始化UART,PA9-PA10
	UARTx_Init(UART_2,9600);												//串口初始化UART,PA9-PA10
	SONIC_GPIO_Init();															//超声波初始化
	GTIM_TIMx_Cap_Chy_Init(84,65536); 							//超声波输入捕获
#if defined(IWDG_ON	)
	IWDG_Init();																		//独立看门狗1s的喂狗时间
#endif
	printf("All Initial is OK\r\n");
	while(1)
	{
		
		if(TimeBaseGetFlag(BASE_100MS))
		{
			TimeBaseClearFlag(BASE_100MS);		
			Sonic_Distance = HC_SR04_GetDistance2();
			DHT11_Calculator(Humi_Temp);
			
			if(Sonic_OnorOff)
			{
				HC_SR04_Start();
			}
			if(UartBLEReceive.ReceiveFlag)                                         
			{
				StrTemp = strstr((const char*)UartBLEReceive.ReceiveData,"Sonic");
				StrTemp = strtok(StrTemp,"=");
				StrTemp = strtok(NULL,"=");
				num = atoi(StrTemp);
				if(num == 10)
				{
					Sonic_OnorOff = 1;
					LED_On(CORE_LED); 
				}
				if(num == 11)
				{
					Sonic_OnorOff = 0;
					LED_Off(CORE_LED); 
				}
				UartBLEReceive.ReceiveFlag = 0;                              
			}
    }
		if(TimeBaseGetFlag(BASE_1000MS))
		{
			TimeBaseClearFlag(BASE_1000MS);
#if defined(IWDG_ON	)
			IWDG_ReloadCounter();
#endif
			printf("Humi:%f%%;Temp:%fC\r\n",Humi_Temp[0],Humi_Temp[1]);
			USART_PRINT(UART2,"SonicDis:%f",Sonic_Distance);
    }
	}
}




