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
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);//���д����
	IWDG_SetPrescaler(IWDG_Prescaler_256);
	IWDG_SetReload(125-1);//����1s��ι��ʱ��
	IWDG_ReloadCounter();//��ι��һ��
	IWDG_Enable();
}
	
