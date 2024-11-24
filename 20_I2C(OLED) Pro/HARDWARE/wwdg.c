#include "wwdg.h"
#include "uart.h"

void WWDG_Init(uint8_t IsOnInterrupt)
{
	if(RCC_GetFlagStatus(RCC_FLAG_WWDGRST) == SET)
	{
		RCC_ClearFlag();
		printf("SystemReset By WWDG\r\n");
	}
	else
	{
		printf("SystemReset By User\r\n");
	}
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);
	WWDG_SetPrescaler(WWDG_Prescaler_8);//一次多长时间(4096*8)/42M = 780us
	WWDG_SetWindowValue(100);//时间范围(127-100)*780 = 21.06us - (127-63)*780 = 49.92ms 必须在这个时间内喂狗
	WWDG_Enable(127);
	WWDG_ClearFlag();
	
//	WWDG->CR |= (1<<7);
//	WWDG->CR |= (1<<7);
//	
//	WWDG->CR = (1<<7)|127;
//	WWDG->CFR = 0|((2<<7)|(1<<7))|80;
	
	if(IsOnInterrupt == SET)
	{
		NVIC_SetPreandSub_IRQx(WWDG_IRQn,0,0);
		WWDG_EnableIT();
	}
}
