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
#include <string.h>

void TIM6_DAC_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM6,TIM_IT_Update) ==SET)
	{		
		TimeBaseUpdata();
		TIM_ClearITPendingBit(TIM6,TIM_IT_Update);
	}

}

/* 输入捕获状态(g_timxchy_cap_sta)
 * [7]  :0,没有成功的捕获;1,成功捕获到一次.
 * [6]  :0,还没捕获到高电平;1,已经捕获到高电平了.
 * [5:0]:捕获高电平后溢出的次数,最多溢出63次,所以最长捕获值 = 63*65536 + 65535 = 4194303
 *       注意:为了通用,我们默认ARR和CCRy都是16位寄存器,对于32位的定时器(如:TIM5),也只按16位使用
 *       按1us的计数频率,最长溢出时间为:4194303 us, 约4.19秒
 *
 *      (说明一下：正常32位定时器来说,1us计数器加1,溢出时间:4294秒)
 */
__IO uint8_t g_timxchy_cap_sta = 0;    /* 输入捕获状态 */
__IO uint16_t g_timxchy_cap_val = 0;   /* 输入捕获值 */	  
float Sonic_Distance = 0;
bool Sonic_OnorOff = 0;

void TIM3_IRQHandler(void)
{ 	
	if((g_timxchy_cap_sta & 0x80) == 0)//还没有成功捕获
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
				g_timxchy_cap_sta |= 0x40;//捕获到了上升沿
				TIM_Cmd(GTIMER_INPUT,DISABLE);
				TIM_SetCounter(GTIMER_INPUT,0);  
				TIM_OC1PolarityConfig(GTIMER_INPUT,TIM_OCPolarity_Low);
				TIM_Cmd(GTIMER_INPUT,ENABLE);
			}
		}
	}
	TIM_ClearITPendingBit(GTIMER_INPUT,TIM_IT_Update|TIM_IT_CC1);
}




void USART1_IRQHandler(void)
{
	uint16_t ReceiveData = 0;
	
	if(USART_GetITStatus(UART1,USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(UART1,USART_IT_RXNE);
		ReceiveData = USART_ReceiveData(UART1);
		USART_SendData(UART2,ReceiveData);
		while(USART_GetFlagStatus(UART1,USART_FLAG_TXE) == RESET);
	}
}



char ReceiveData[255] = {0};
uint8_t ReceiveFlag = 0;
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
				ReceiveData[TempDataIndex] = TempData;
				TempDataIndex++;
			}
		}
		
		if(state == 2)
		{
			state = 0;
			TempDataIndex = 0;
		}	
	}
	ReceiveFlag = 1;
}


int main(int argc,char *argv[])
{ 
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
	printf("All Initial is OK\r\n");
	while(1)
	{
		
		if(TimeBaseGetFlag(BASE_1000MS))
		{
			TimeBaseClearFlag(BASE_1000MS);
			USART_PRINT(UART2,"SonicDis:%f",Sonic_Distance);
    }
		
		if(TimeBaseGetFlag(BASE_100MS))
		{
			TimeBaseClearFlag(BASE_100MS);	
			if(Sonic_OnorOff)
			{
				HC_SR04_Start();
			}
			if(ReceiveFlag)                                         //如果接收到数据
			{
				if(strcmp(ReceiveData, "ON") == 0)                  //如果接收到的数据是LED_ON
				{
					Sonic_OnorOff = 1;
					LED_On(CORE_LED);                                           //执行开灯函数
				}
				else if(strcmp(ReceiveData, "OFF") == 0)            //如果接收到的数据是LED_OFF
				{
					Sonic_OnorOff = 0;
					LED_Off(CORE_LED);                                          //执行关灯函数
				}
				ReceiveFlag=0;                                      //接收到数据标志位置0，为下次接收字符串做准备
			}
			Sonic_Distance = HC_SR04_GetDistance2();
    }
	}
}




