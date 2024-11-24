#ifndef __GTIMER_H
#define __GTIMER_H


#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f4xx.h"

/* 输入捕获状态(g_timxchy_cap_sta)
 * [7]  :0,没有成功的捕获;1,成功捕获到一次.
 * [6]  :0,还没捕获到高电平;1,已经捕获到高电平了.
 * [5:0]:捕获高电平后溢出的次数,最多溢出63次,所以最长捕获值 = 63*65536 + 65535 = 4194303
 *       注意:为了通用,我们默认ARR和CCRy都是16位寄存器,对于32位的定时器(如:TIM5),也只按16位使用
 *       按1us的计数频率,最长溢出时间为:4194303 us, 约4.19秒
 *
 *      (说明一下：正常32位定时器来说,1us计数器加1,溢出时间:4294秒)
 */

typedef struct
{
	__IO uint8_t gtimer_cap_sta;    /* 输入捕获状态 */
	__IO uint16_t gtimer_cap_val;   /* 输入捕获值 */
}	GtimerInput_Typedef;

#define GTIMER_INPUT										TIM3
#define GTIMER_INPUT_RCC								RCC_APB1Periph_TIM3
#define GTIMER_INPUT_PORT_RCC						RCC_AHB1Periph_GPIOA

#define INPUT_CAPTURE_PORT							GPIOA
#define INPUT_CAPTURE_PIN								GPIO_Pin_6

#define INPUT_AF_PIN										GPIO_PinSource6
#define INPUT_AF												GPIO_AF_TIM3

#define NVIC_INPUT_IRQ									TIM3_IRQn
#define NVIC_INPUT_PRE									1
#define NVIC_INPUT_SUB									1

void GeneralTimer_InputCaptureInit(void);
void GTIM_TIMx_Cap_Chy_Init(uint16_t psc,uint32_t arr);



#ifdef __cplusplus
}
#endif

#endif
