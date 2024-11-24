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
MyADC_Typedef ADC_Temp;
MFRC522_Typedef RC522;	
Storage_Typedef AT24c02;
Storage_Typedef W25Q64;

//typedef struct
//{
//	uint8_t Index[3];
//	uint8_t Data[16*8];
//}MyChineseCell_t;

//const MyChineseCell_t image1[]= {
//"唐",
//0x00,0x00,0x00,0x00,0x00,0xE0,0xC0,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x44,
//0xCC,0xF8,0x70,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x60,0x60,0x40,0x00,0x00,0x00,
//0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0x40,0x40,0x44,0x44,0x44,0x44,0x44,0x44,0x44,
//0xFF,0xFF,0x44,0x44,0x44,0x44,0x44,0x44,0xFE,0xFC,0x44,0x60,0x60,0x40,0x00,0x00,
//0x00,0x00,0x00,0x00,0x80,0xFF,0x3F,0x00,0x00,0x04,0xC4,0xC4,0x44,0x44,0x44,0x44,
//0x7F,0x7F,0x44,0x44,0x44,0x44,0x44,0x44,0xEF,0xCF,0x40,0x00,0x00,0x00,0x00,0x00,
//0x00,0x40,0x20,0x1C,0x07,0x01,0x00,0x00,0x00,0x00,0x3F,0x3F,0x10,0x10,0x10,0x10,
//0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x3F,0x3F,0x00,0x00,0x00,0x00,0x00,0x00,/*"唐",0*/
//"佳",
//0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xF0,0x78,0x18,0x00,0x00,0x00,0x00,0x00,
//0x00,0x00,0x00,0xFC,0xF8,0x08,0x00,0x00,0x00,0x80,0x80,0x00,0x00,0x00,0x00,0x00,
//0x00,0x00,0x00,0x80,0x60,0x30,0x0C,0xFF,0xF9,0x00,0x40,0x40,0x41,0x41,0x41,0x41,
//0x41,0x41,0x41,0x7F,0x7F,0x41,0x41,0x41,0x41,0x41,0x41,0x61,0x60,0x40,0x00,0x00,
//0x00,0x02,0x01,0x00,0x00,0x00,0x00,0xFF,0xFF,0x00,0x00,0x00,0x20,0x20,0x20,0x20,
//0x20,0x20,0x20,0xFF,0xFF,0x22,0x20,0x20,0x20,0x20,0x30,0x20,0x00,0x00,0x00,0x00,
//0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7F,0x3F,0x10,0x10,0x10,0x10,0x10,0x10,0x10,
//0x10,0x10,0x10,0x1F,0x1F,0x10,0x10,0x10,0x10,0x10,0x10,0x18,0x18,0x10,0x00,0x00,/*"佳",1*/
//"辉",
//0x00,0x00,0x20,0xC0,0x80,0x00,0x00,0xFC,0xF8,0x08,0x00,0x80,0xE0,0x40,0x80,0xC0,
//0xF8,0x10,0x10,0x10,0xD0,0x90,0x90,0x10,0x10,0x10,0x90,0x70,0x38,0x30,0x00,0x00,
//0x00,0x40,0x40,0x40,0x4F,0xC6,0xC0,0x7F,0x7F,0xD0,0xCC,0x43,0x41,0x60,0x41,0x09,
//0x88,0xE8,0x78,0x1F,0x0F,0x29,0xC8,0x48,0x08,0x08,0x08,0x0C,0x08,0x00,0x00,0x00,
//0x00,0x00,0x00,0x00,0x80,0xFF,0x0F,0x00,0x00,0xFF,0xFF,0x00,0x80,0x40,0xA0,0x83,
//0x83,0x82,0x82,0x82,0x82,0x82,0xFF,0x82,0x82,0x82,0x82,0x83,0xC2,0x80,0x00,0x00,
//0x00,0x40,0x30,0x0C,0x07,0x00,0x00,0x00,0x00,0x07,0x0F,0x03,0x01,0x00,0x00,0x00,
//0x00,0x00,0x00,0x00,0x00,0x00,0x7F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"辉",2*/

//};

uint8_t Img[]={
0xFF,0x01,0xE1,0x11,0x49,0x25,0x45,0x05,0x45,0x25,0x49,0x11,0xE1,0x01,0xFF,
0x7F,0x40,0x43,0x44,0x48,0x51,0x52,0x52,0x52,0x51,0x48,0x44,0x43,0x40,0x7F
};

const uint8_t Chinese1[][32] = {
	
};

const uint8_t Tx_Buffer[] = "JackTJH@nuaa.edu.cn";
#define BufferSize 			sizeof(Tx_Buffer)

#if defined(WWDG_ON	)
WWDG_Cnt_Typedef WWDG_Cnt40;
#endif
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

extern uint8_t OLED_DisplayBuf[8][128];
bool RTC_WKUP_Flag = 0;
RTC_TimeTypeDef RTC_TimeStructure;
RTC_AlarmTypeDef RTC_AlarmStructure;
RTC_SetTimeStructure RTC_SetTime1;
void RTC_WKUP_IRQHandler(void)
{
  if(RTC_GetITStatus(RTC_IT_WUT) != RESET)
  {
    RTC_WKUP_Flag = 1;
    RTC_ClearITPendingBit(RTC_IT_WUT);
    EXTI_ClearITPendingBit(EXTI_Line22);
  }
}

// 闹钟中断服务函数
void RTC_Alarm_IRQHandler(void)
{
  if(RTC_GetITStatus(RTC_IT_ALRA) != RESET)
  {    
    RTC_ClearITPendingBit(RTC_IT_ALRA);
    EXTI_ClearITPendingBit(EXTI_Line17);
  }
	printf("Time_Arrive\r\n");	
}

#if defined(WWDG_ON	)
void WWDG_IRQHandler(void)
{
	if(WWDG_GetFlagStatus() == SET)
	{
		WWDG_ClearFlag();
		WWDG_SetCounter(127);
	}
}
#endif


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
	MYADC_Init();																		//ADC初始化，规则通道，开启扫描加DMA，2通道	
	InternalFlash_Read((void*)My_Flash.ReadDataTemp,INTERNAL_FLASH_STAR_ADDR,sizeof(My_Flash.ReadDataTemp));
	My_RTC_Init(WKUPINT,ARARMINT);
	

	JI2C_Init(I2C1MODE,JI2C2);
	OLED_Init(I2C1MODE,JI2C2);
	
	OLED_Clear1();
	
	
	OLED_ShowString1(0,0,"Tangjiahui111",8);
	
//	Show_Chinese(0,0,"唐佳辉");
//	
	OLED_Update(I2C1MODE,JI2C2);
	
//	OLED_Clear1();
//	OLED_ShowChar1(2,10,'#',6);
//	OLED_ShowChar1(4,10,'#',8);
//	OLED_ShowString1(0,0,"Tangjiahui111",8);
//	OLED_ShowImage1(0,10,32,4,image1[0]);
	
//	OLED_SetCursor(5,20,I2C1MODE,JI2C2);
//	OLED_WriteData1(0x88,I2C1MODE,JI2C2);
//	OLED_WriteData1(0x88,I2C1MODE,JI2C2);
//	OLED_WriteData1(0x88,I2C1MODE,JI2C2);
//	OLED_WriteData1(0x88,I2C1MODE,JI2C2);
//	OLED_WriteData1(0x88,I2C1MODE,JI2C2);
//	OLED_WriteData1(0x88,I2C1MODE,JI2C2);
//	OLED_WriteData1(0x88,I2C1MODE,JI2C2);
//	OLED_WriteData1(0x88,I2C1MODE,JI2C2);

	
#if defined(IWDG_ON)
	IWDG_Init();																		//独立看门狗1s的喂狗时间
#endif
#if defined(WWDG_ON	)
	WWDG_Init(RESET);																//窗口看门狗,喂狗时间范围21.06us-49.92ms 这里虽然算出来21.06但至少要大于它三秒左右
#endif
	printf("All Initial is OK\r\n");
	while(1)
	{	
		if(RTC_WKUP_Flag)
		{
			RTC_WKUP_Flag = 0;
			RTC_GetTime(RTC_Format_BINorBCD,&RTC_TimeStructure);
//			printf("%02d:%02d:%02d\r\n",RTC_TimeStructure.RTC_Hours,\
//			RTC_TimeStructure.RTC_Minutes,RTC_TimeStructure.RTC_Seconds);
		}
		
		if(TimeBaseGetFlag(BASE_100MS))
		{
//			HC_SR04_Start();			
//			Sonic_Distance = HC_SR04_GetDistance2();
			Get_Kernal_Tempature(&ADC_Temp,10);
//			DHT11_Calculator(Humi_Temp);//这个函数里面有大量延时函数，如果时间基准要求严格会影响到其他程序
//			My_ScanCard(&RC522,SPI_MODE,JSPI1);
//			AD9833_SetFrequencyQuick(AD9833_1.frequent,AD9833_OUT_MSB,SPI_MODE,JSPI1);//写输出频率1000.0Hz,输出方波

			
			if(UartBLEReceive.ReceiveFlag)                                         
			{
				UartBLEReceive.ReceiveFlag = 0;
				if(BLE_Modify_Time((const char*)UartBLEReceive.ReceiveData,&RTC_SetTime1) == SET)
				{
					RTC_WriteBackupRegister(RTC_BKP_DRX,FIRST_DATA);
					SetRTCAlarm(&RTC_AlarmStructure,&RTC_SetTime1);
					My_Flash.WriteData[0] = RTC_SetTime1.RTC_Hours;
					My_Flash.WriteData[1] = RTC_SetTime1.RTC_Minutes;
					My_Flash.WriteData[2] = RTC_SetTime1.RTC_Seconds;
					
					InternalFlash_Erase(INTERNAL_FLASH_STAR_ADDR);
					InternalFlash_Write((void*)My_Flash.WriteData,INTERNAL_FLASH_STAR_ADDR,sizeof(My_Flash.WriteData));
				}
			}
			TimeBaseClearFlag(BASE_100MS);
    }
		else if(TimeBaseGetFlag(BASE_1000MS))
		{
#if defined(IWDG_ON)
			IWDG_ReloadCounter();
#endif
//			printf("ADC1:%3.2fC,%d\r\n",ADC_Temp.Tempture,ADC_Temp.AdcValue[1]);
//			USART_PRINT(UART2,"SonicDis:%f",Sonic_Distance);
			TimeBaseClearFlag(BASE_1000MS);
    }
	}
}




