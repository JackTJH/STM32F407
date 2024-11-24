#ifndef __GTIMER_H
#define __GTIMER_H


#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f4xx.h"

/* ���벶��״̬(g_timxchy_cap_sta)
 * [7]  :0,û�гɹ��Ĳ���;1,�ɹ�����һ��.
 * [6]  :0,��û���񵽸ߵ�ƽ;1,�Ѿ����񵽸ߵ�ƽ��.
 * [5:0]:����ߵ�ƽ������Ĵ���,������63��,���������ֵ = 63*65536 + 65535 = 4194303
 *       ע��:Ϊ��ͨ��,����Ĭ��ARR��CCRy����16λ�Ĵ���,����32λ�Ķ�ʱ��(��:TIM5),Ҳֻ��16λʹ��
 *       ��1us�ļ���Ƶ��,����ʱ��Ϊ:4194303 us, Լ4.19��
 *
 *      (˵��һ�£�����32λ��ʱ����˵,1us��������1,���ʱ��:4294��)
 */

typedef struct
{
	__IO uint8_t gtimer_cap_sta;    /* ���벶��״̬ */
	__IO uint16_t gtimer_cap_val;   /* ���벶��ֵ */
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
