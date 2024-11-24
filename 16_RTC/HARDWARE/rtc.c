#include "rtc.h"
#include "uart.h"
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

void RTC_CLK_Config(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
	PWR_BackupAccessCmd(ENABLE);
	RCC_LSEConfig(RCC_LSE_ON);
	while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) != SET);
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	RCC_RTCCLKCmd(ENABLE);
	RTC_WaitForSynchro();
	
	RTC_InitTypeDef RTC_InitStructure;
	
	RTC_InitStructure.RTC_AsynchPrediv = 0X7F;
	RTC_InitStructure.RTC_SynchPrediv = 0XFF;
	RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
	RTC_Init(&RTC_InitStructure);
}

void RTC_TimeAndDateSet(void)
{
	RTC_TimeTypeDef RTC_TimeStructure;
	RTC_TimeStructure.RTC_H12 = RTC_H12_AMorPM;
	RTC_TimeStructure.RTC_Hours = HOURS;
	RTC_TimeStructure.RTC_Minutes = MINUTES;
	RTC_TimeStructure.RTC_Seconds = SECONDS;
	RTC_SetTime(RTC_Format_BINorBCD,&RTC_TimeStructure);
	RTC_WriteBackupRegister(RTC_BKP_DRX,FIRST_DATA);

	RTC_DateTypeDef RTC_DateStructure;
	RTC_DateStructure.RTC_Date = DATE;
	RTC_DateStructure.RTC_Month = MONTH;
	RTC_DateStructure.RTC_WeekDay = WEEKDAY;
	RTC_DateStructure.RTC_Year = YEAR;
	RTC_SetDate(RTC_Format_BINorBCD,&RTC_DateStructure);
	RTC_WriteBackupRegister(RTC_BKP_DRX,FIRST_DATA);
}

void ALARM_Interrupt_Init(void)
{
	My_EXTI_Set(EXTI_Line17,EXTI_Mode_Interrupt,EXTI_Trigger_Rising);
	NVIC_SetPreandSub_IRQx(RTC_Alarm_IRQn,NVIC_RTC_ALARMA_PRE,NVIC_RTC_ALARMA_SUB);
	RTC_AlarmCmd(RTC_Alarm_A,DISABLE);
	RTC_AlarmTypeDef RTC_AlarmStructure;
	RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours = ALARM_HOURS;
	RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes = ALARM_MINUTES;
	RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds = ALARM_SECONDS;
	RTC_AlarmStructure.RTC_AlarmMask = RTC_AlarmMask_DateWeekDay;
	RTC_SetAlarm(RTC_Format_BINorBCD,RTC_Alarm_A,&RTC_AlarmStructure);
	RTC_ITConfig(RTC_IT_ALRA,ENABLE);
	RTC_AlarmCmd(RTC_Alarm_A,ENABLE);
	/* ��������жϱ�־λ */
  RTC_ClearFlag(RTC_FLAG_ALRAF);
	/* ��� EXTI Line 17 ����λ (�ڲ����ӵ�RTC Alarm) */
  EXTI_ClearITPendingBit(EXTI_Line17);
}

void WKUP_Interrupt_Init(void)
{
	//�����ⲿ�����ж�
	My_EXTI_Set(EXTI_Line22,EXTI_Mode_Interrupt,EXTI_Trigger_Rising);
	NVIC_SetPreandSub_IRQx(RTC_WKUP_IRQn,NVIC_RTC_WKUP_PRE,NVIC_RTC_WKUP_SUB);
	RTC_WakeUpCmd(DISABLE);
	RTC_WakeUpClockConfig(RTC_WakeUpClock_CK_SPRE_16bits);
	RTC_SetWakeUpCounter(0);
	RTC_WakeUpCmd(ENABLE);
	RTC_ITConfig(RTC_IT_WUT,ENABLE);
	RTC_ClearITPendingBit(RTC_IT_WUT);
	EXTI_ClearITPendingBit(EXTI_Line22);
}

void My_RTC_Init(RTC_INTTypeDef Interrupt,RTC_INTTypeDef Alarm)
{
	RTC_CLK_Config();
	if(Interrupt == WKUPINT)
	{
		WKUP_Interrupt_Init();
	}
	if(Alarm == ARARMINT)
	{
			ALARM_Interrupt_Init();
	}
	if(RTC_ReadBackupRegister(RTC_BKP_DRX) != FIRST_DATA)
	{	
		RTC_TimeAndDateSet();
	}
	else
	{
		/* ʹ�� PWR ʱ�� */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
		/* PWR_CR:DBF��1��ʹ��RTC��RTC���ݼĴ����ͱ���SRAM�ķ��� */
		PWR_BackupAccessCmd(ENABLE);
		/* �ȴ� RTC APB �Ĵ���ͬ�� */
		RTC_WaitForSynchro(); 
	}
}

extern UartReceive_TypeDef UartBLEReceive;
uint8_t BLE_Modify_Time(const char* commend,RTC_SetTimeStructure *SetTimeStructure)
{
	
	if (strncmp(commend, "set-time:", 9) != 0) 
	{
		printf("This set Day\r\n");
		return 0;
	}

	// ��ȡʱ�䲿��
	char timeStr[9];
	//command ������������ַ��������� "set-time:16:02:00"
	//command + 9 ��ָ�� command �ַ����ĵ�10���ַ���������0��ʼ����ʼ��λ�á�Ҳ����˵���� command �ĵ�10���ַ���ʼ���� "16:02:00"
	strncpy(timeStr, commend + 9, 8); // ����ʱ���ַ���������ð��
	timeStr[8] = '\0'; // ȷ���ַ�����null��β
	
	// �ָ�ʱ���ַ���
	char *token;
	token = strtok(timeStr, ":");
	if (token == NULL) return 0;
	SetTimeStructure->RTC_Hours = atoi(token);

	token = strtok(NULL, ":");
	if (token == NULL) return 0;
	SetTimeStructure->RTC_Minutes = atoi(token);

	token = strtok(NULL, ":");
	if (token == NULL) return 0;
	SetTimeStructure->RTC_Seconds = atoi(token); 
	
//	char *token;
//	token = strtok(timeStr, ":");
//	if (token == NULL) return 0;
//	SetTimeStructure->RTC_Hours = atoi(token)/10*16 + atoi(token)%10;

//	token = strtok(NULL, ":");
//	if (token == NULL) return 0;
//	SetTimeStructure->RTC_Minutes = atoi(token)/10*16 + atoi(token)%10;

//	token = strtok(NULL, ":");
//	if (token == NULL) return 0;
//	SetTimeStructure->RTC_Seconds = atoi(token)/10*16 + atoi(token)%10;
	
	return 1;
}

void SetRTCAlarm(RTC_AlarmTypeDef *alarmTime,RTC_SetTimeStructure *setTime) 
{
    RTC_AlarmCmd(RTC_Alarm_A, DISABLE);
    alarmTime->RTC_AlarmTime.RTC_Hours = setTime->RTC_Hours;
    alarmTime->RTC_AlarmTime.RTC_Minutes = setTime->RTC_Minutes;
    alarmTime->RTC_AlarmTime.RTC_Seconds = setTime->RTC_Seconds;
    alarmTime->RTC_AlarmMask = RTC_AlarmMask_DateWeekDay;
    RTC_SetAlarm(RTC_Format_BIN, RTC_Alarm_A, alarmTime);
    RTC_AlarmCmd(RTC_Alarm_A, ENABLE);
}
