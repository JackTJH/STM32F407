#include "iwdg.h"
#include "uart.h"


void IWDG_Init(void)
{
	if(RCC_GetFlagStatus(RCC_FLAG_IWDGRST) == SET)
	{
		RCC_ClearFlag();
		printf("SystemReset By IWDG\r\n");
	}
	else
	{
		printf("SystemReset By User\r\n");
	}
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);//解除写保护
	IWDG_SetPrescaler(IWDG_Prescaler_256);
	IWDG_SetReload(125-1);//设置1s的喂狗时间
	IWDG_ReloadCounter();//先喂狗一次
	IWDG_Enable();
}
	
