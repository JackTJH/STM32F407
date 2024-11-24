#ifndef __RTC_H
#define __RTC_H

#ifdef __cplusplus
	extern "C" {
#endif

#include "stm32f4xx.h"

typedef enum
{
	NONEINT,
	ARARMINT,
	WKUPINT
} RTC_INTTypeDef;

// 定义RTC时间结构体
typedef struct {
    int RTC_Hours;
    int RTC_Minutes;
    int RTC_Seconds;
} RTC_SetTimeStructure;

#define FIRST_DATA          	0x45685266
// 备份域寄存器宏定义
#define RTC_BKP_DRX          	RTC_BKP_DR1

// 时间宏定义
#define RTC_H12_AMorPM			 	RTC_H12_AM  
#define HOURS                	12          	// 0~23
#define MINUTES              	10          	// 0~59
#define SECONDS              	10          	// 0~59

// 日期宏定义
#define WEEKDAY              	4         		// 1~7
#define DATE                 	31         	// 1~31
#define MONTH                	10         	// 1~12
#define YEAR                 	24         	// 0~99

#define NVIC_RTC_WKUP_PRE			2
#define NVIC_RTC_WKUP_SUB			0

#define NVIC_RTC_ALARMA_PRE		2
#define NVIC_RTC_ALARMA_SUB		1

// 闹钟相关宏定义
#define ALARM_HOURS           12          // 0~23
#define ALARM_MINUTES         00          // 0~59
#define ALARM_SECONDS         00          // 0~59

// 时间格式宏定义
#define RTC_Format_BINorBCD  RTC_Format_BIN

void My_RTC_Init(RTC_INTTypeDef Interrupt,RTC_INTTypeDef Alarm);
uint8_t BLE_Modify_Time(const char* commend,RTC_SetTimeStructure *SetTimeStructure);
void SetRTCAlarm(RTC_AlarmTypeDef *alarmTime,RTC_SetTimeStructure *setTime); 

#ifdef __cplusplus
}
#endif

#endif
