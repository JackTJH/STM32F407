#include "delay.h"


void System_Tick_Init(void)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
}

/** \brief  Delay_us

    The function is to delay.

	\note nus do not exceed 798000
    
 */
void Delay_us(uint32_t nus)
{
	SysTick->CTRL = 0;
	SysTick->LOAD = nus*21-1;
	SysTick->VAL = 0;
	SysTick->CTRL = SysTick_CTRL_ENABLE_Msk;
	while((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == 0);
	SysTick->CTRL = 0;
}


/** \brief  Delay_ms

    The function is to delay.

	\note nms do not exceed 798
    
 */
void Delay_ms(uint32_t nms)
{
	SysTick->CTRL = 0;
	SysTick->LOAD = nms*21*1000-1;
	SysTick->VAL = 0;
	SysTick->CTRL = SysTick_CTRL_ENABLE_Msk;
	while((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == 0);
	SysTick->CTRL = 0;
}

