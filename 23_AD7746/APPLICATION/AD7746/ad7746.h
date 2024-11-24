#ifndef __AD7746_H
#define __AD7746_H

#ifdef __cplusplus
	extern "C"{
#endif

#include "jack_i2c.h"
#include "stm32f4xx.h"

typedef enum
{
	RDYCAP = 0,
	RDYVT,
	RDY,
	EXCERR
}AD774X_Status;

#define CapDataH							0x01
#define CapDataM							0x02
#define CapDataL							0x03

#define CapSetup							0x07
#define CAPEN									0x80
#define CIN2									0x40
#define CAPDIFF								0x20
#define CAPCHOP								0x01

#define VTSetup								0x08
#define VTEN									0x80
#define VTMD1									0x40
#define VTMD0									0x20
#define EXTREF								0x10
#define VTSHORT								0x02
#define	VTCHOP								0x01

#define EXCSetup							0x09
#define CLKCTRL								0x80
#define EXCON									0x40
#define EXCB									0x20
#define EXCB_NO								0x10
#define EXCA									0x08
#define EXCA_NO								0x04
#define EXCLVL1								0x02
#define EXCLVL0								0x01

#define CONFIGURATION					0x0A
#define VTF1									0x80
#define VTF0									0x40
#define CAPF2									0x20
#define CAPF1									0x10
#define CAPF0									0x08
#define	MD2										0x04
#define MD1										0x02
#define MD0										0x01

#define DACAREGISTER					0x0B
#define DACAENA								0x80

#define DACBREGISTER					0x0C
#define DACAENA								0x80



#define EMPTY									0x00

typedef struct
{
	uint32_t original;
	float capicitance;
}AD774X_Typedef;

void AD774X_Init_Reset(I2C_TYPE i2c_type,I2CX_Typedef i2cx);
uint8_t AD774X_GetStatusFlag(AD774X_Status status,I2C_TYPE i2c_type,I2CX_Typedef i2cx);
void AD7746_Init(I2C_TYPE i2c_type,I2CX_Typedef i2cx);
uint32_t AD774X_GetCapacitance(I2C_TYPE i2c_type,I2CX_Typedef i2cx);
float AD7746_CapConvert(uint8_t capdac,I2C_TYPE i2c_type,I2CX_Typedef i2cx);






#ifdef __cplusplus
	}
#endif

#endif
