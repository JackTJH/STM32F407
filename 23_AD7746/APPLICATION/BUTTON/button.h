#ifndef __BUTTON_H
#define __BUTTON_H


#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f4xx.h"

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "key.h"

//#define DEBUG

#ifdef DEBUG
#define PRINT_DBG(fmt, args...)                            \
    do                                                     \
    {                                                      \
        printf("[DEBUG] >> FILE:%s, LINE:%d, FUNC:%s\r\n", \
               __FILE__, __LINE__, __FUNCTION__);          \
        printf(fmt, ##args);                               \
    } while (0)
#else
#define PRINT_DBG(fmt, args...)
#endif

#define BTN_NAME_MAX 32

/* ����ѭ��10ms */
#define BTN_DEBOUNCE_TIME 5 			// ��������
#define BTN_DOUBLE_TIME 	35   		// ˫���������
#define BTN_LONG_TIME 		45     	// �����������

typedef void (*BTN_CALLBACK)(void *);

typedef enum
{
    BTN_LEVEL_LOW = 0,
    BTN_LEVEL_HIGH,
} BTN_LEVEL;

typedef enum
{
    BTN_DOWM = 0, // ����
    BTN_UP,       // �ɿ�
    BTN_NONE,
} BTN_STATE;

typedef enum
{
    BTN_EV_CLICK = 0,    // ����
    BTN_EV_DOUBLE_CLICK, // ˫��
    BTN_EV_LONG_CLICK,   // ����
    BTN_EV_CNT,          // ��������ͳ���¼�����
    BTN_EV_NULL,         // ���¼�����
} BTN_EVENT;

typedef struct button
{
    uint8_t (*read_btn_level)(void);  /* ��ȡ������ƽ���� */
    char name[BTN_NAME_MAX];          /* �������� */
    uint8_t button_state : 2;         /* ����״̬ */
    uint8_t button_last_state : 2;    /* ��һ�εİ���״̬���ж�˫�� */
    uint8_t button_trigger_level : 2; /* ����������ƽ */
    uint8_t button_last_level : 2;    /* ��һ�ΰ�����ƽ */

    BTN_EVENT event;                          /* ���������¼���������˫�������� */
    BTN_CALLBACK button_callback[BTN_EV_CNT]; /* ��������ָ�� */

    uint8_t button_trigger_cnt; /* ˫����������ڰ����������� */
    uint16_t timer_count;       /* ����ֵ */
    uint16_t debounce_count;    /* ��������ֵ */
    struct button *next;        /* ����ָ�� */

} BTN_HandleTypedef;

void button_create(const char *name, BTN_HandleTypedef *btn, 
        uint8_t (*read_btn_level)(void), uint8_t btn_trigger_level);
void button_attach(BTN_HandleTypedef *btn, BTN_EVENT btn_event, BTN_CALLBACK btn_callback);
void button_process(void);
void button_delete(BTN_HandleTypedef *btn);
	 
	 
#ifdef __cplusplus
}
#endif

#endif	 