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

/* 按键循环10ms */
#define BTN_DEBOUNCE_TIME 5 			// 消抖周期
#define BTN_DOUBLE_TIME 	35   		// 双击间隔周期
#define BTN_LONG_TIME 		45     	// 长按检测周期

typedef void (*BTN_CALLBACK)(void *);

typedef enum
{
    BTN_LEVEL_LOW = 0,
    BTN_LEVEL_HIGH,
} BTN_LEVEL;

typedef enum
{
    BTN_DOWM = 0, // 按下
    BTN_UP,       // 松开
    BTN_NONE,
} BTN_STATE;

typedef enum
{
    BTN_EV_CLICK = 0,    // 单击
    BTN_EV_DOUBLE_CLICK, // 双击
    BTN_EV_LONG_CLICK,   // 长按
    BTN_EV_CNT,          // 辅助用于统计事件个数
    BTN_EV_NULL,         // 无事件发生
} BTN_EVENT;

typedef struct button
{
    uint8_t (*read_btn_level)(void);  /* 读取按键电平函数 */
    char name[BTN_NAME_MAX];          /* 按键名称 */
    uint8_t button_state : 2;         /* 按键状态 */
    uint8_t button_last_state : 2;    /* 上一次的按键状态，判断双击 */
    uint8_t button_trigger_level : 2; /* 按键触发电平 */
    uint8_t button_last_level : 2;    /* 上一次按键电平 */

    BTN_EVENT event;                          /* 按键触发事件，单击，双击，长按 */
    BTN_CALLBACK button_callback[BTN_EV_CNT]; /* 按键函数指针 */

    uint8_t button_trigger_cnt; /* 双击检测周期内按键触发次数 */
    uint16_t timer_count;       /* 计数值 */
    uint16_t debounce_count;    /* 消抖计数值 */
    struct button *next;        /* 链表指针 */

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