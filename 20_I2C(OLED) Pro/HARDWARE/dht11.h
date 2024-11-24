#ifndef __DHT11_H
#define __DHT11_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f4xx.h"

typedef enum
{
	DHT11_ONE_ERR = 0,
	DHT11_TWO_ERR,
	DHT11_INITIAL_SUCC,
	DHT11_READ_BIT_FAILED
} DHT11_Typedef;

#define DHT11_PORT									GPIOA
#define DHT11_PIN										GPIO_Pin_2

#define DHT11_DQ(bit)								GPIO_WriteBit(DHT11_PORT,DHT11_PIN,(bit)?Bit_SET:Bit_RESET)
#define DHT11_DQ_Read() 						GPIO_ReadInputDataBit(DHT11_PORT, DHT11_PIN)

#define DHT11_GPIO_RCC_PORT					RCC_AHB1Periph_GPIOA

void DHT11_Out(void);
void DHT11_In(void);
void DHT11_Start(void);
uint8_t DHT11_Check(void);
uint8_t DHT11_ReadBit(void);
uint8_t DHT11_ReadBytes(void);
void DHT11_ReadData(uint8_t *dat);
void DHT11_Calculator(float *c_dat);



#ifdef __cplusplus
}
#endif


#endif
