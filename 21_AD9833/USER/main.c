#include "stm32f4xx.h"
#include "base_timer.h"
#include "pwm.h"
#include "delay.h"
#include "led.h"
#include "uart.h"
#include "button.h"
#include "sonic.h"
#include "gtimer.h"
#include "dht11.h"
#include "iwdg.h"
#include "wwdg.h"
#include "rtc.h"
#include "flash.h"
#include "adc.h"
#include "hspi.h"
#include "w25qxx.h"
#include "mfrc522.h"
#include "at24cxx.h"
#include "OLED.h"
#include "ad9833.h"
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

My_Flash_Typedef My_Flash;
W25QXX_Typedef W25Q64;

const uint8_t Tx_Buffer[] = "JackTJH@nuaa.edu.cn";
#define BufferSize 			sizeof(Tx_Buffer)

void TIM6_DAC_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM6,TIM_IT_Update) ==SET)
	{		
		TimeBaseUpdata();
#if defined(WWDG_ON	)
		if(WWDG_Cnt40.Cnt++ >= 24)
		{
			WWDG_SetCounter(127);
			WWDG_Cnt40.Cnt = 0;
		}
#endif
		TIM_ClearITPendingBit(TIM6,TIM_IT_Update);
	}
}















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
	float Sonic_Distance = 0;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//中断优先级分组
	LED_Init(CORE_LED);															//初始化核心板LED
	System_Tick_Init();															//滴答定时器初始化
	BASE_Timer_Init(84,1000);												//基本定时器初始化,这里参数配置的1ms中断
	PWM_Config_Init(84,100);												//方波的频率为10k
	UARTx_Init(UART_1,115200);											//串口初始化UART,PA9-PA10
	UARTx_Init(UART_2,9600);												//串口初始化UART,PA9-PA10
	Button_Init(CORE_BUTTON);												//按键初始化
	key_setup();															
	InternalFlash_Read((void*)My_Flash.ReadDataTemp,INTERNAL_FLASH_STAR_ADDR,sizeof(My_Flash.ReadDataTemp));
	JSPI_Init(SPI_MODE);
//	AD9833_SetFrequencyQuick(1000.0,AD9833_OUT_TRIANGLE);//写输出频率1000.0Hz,输出三角波	
//	AD9833_SetFrequencyQuick(2000.0,AD9833_OUT_SINUS);//写输出频率1000.0Hz,输出正弦波
	AD9833_SetFrequencyQuick(1000.0,AD9833_OUT_MSB);//写输出频率1000.0Hz,输出方波
	
	printf("All Initial is OK\r\n");
	while(1)
	{		
		if(TimeBaseGetFlag(BASE_100MS))
		{
			
			if(UartBLEReceive.ReceiveFlag)                                         
			{
				UartBLEReceive.ReceiveFlag = 0;
//				if(BLE_Modify_Time((const char*)UartBLEReceive.ReceiveData,&RTC_SetTime1) == SET)
//				{
//					RTC_WriteBackupRegister(RTC_BKP_DRX,FIRST_DATA);
//					SetRTCAlarm(&RTC_AlarmStructure,&RTC_SetTime1);
//					My_Flash.WriteData[0] = RTC_SetTime1.RTC_Hours;
//					My_Flash.WriteData[1] = RTC_SetTime1.RTC_Minutes;
//					My_Flash.WriteData[2] = RTC_SetTime1.RTC_Seconds;
//					
//					InternalFlash_Erase(INTERNAL_FLASH_STAR_ADDR);
//					InternalFlash_Write((void*)My_Flash.WriteData,INTERNAL_FLASH_STAR_ADDR,sizeof(My_Flash.WriteData));
//				}
			}
			TimeBaseClearFlag(BASE_100MS);
    }
		else if(TimeBaseGetFlag(BASE_1000MS))
		{
			TimeBaseClearFlag(BASE_1000MS);
    }
	}
}




