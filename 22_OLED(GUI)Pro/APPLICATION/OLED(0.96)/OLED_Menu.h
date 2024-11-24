#ifndef __OLED_MENU_H
#define __OLED_MENU_H


#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f4xx.h"
#include "OLED.h"

#define OLED_Hight						64
#define OLED_Width						128
#define OLED_Rectangle_Hight	20
#define MENU_LEVEL						2
#define MENU_MODE							1	//0是文本1是图片

typedef enum
{
	BUTTON_Default,
	BUTTON_Next_page,
	BUTTON_Pre_page,
	BUTTON_INSERT,
	BUTTON_UNCONDITION_INSERT,
	BUTTON_EXTI
}Button_Operation;

typedef struct
{
	const char *name;
	void (*func)(void);
}MENU_Item_t;

typedef struct MENU_t
{
	MENU_Item_t *item;
	uint8_t item_num;
	uint8_t cur_item;
	uint8_t menu_flag;
	struct MENU_t *sub2_menu;
	struct MENU_t *sub3_menu;
}MENU_t;

extern MENU_t menu;

void Item1_Callback(void);
void Item2_Callback(void);
void Item3_Callback(void);
void Item4_Callback(void);
void Item5_Callback(void);

void Sub1_Callback(void);
void Sub2_Callback(void);
void Sub3_Callback(void);
void Sub4_Callback(void);
void Sub5_Callback(void);

void Third1_Callback(void);
void Third2_Callback(void);
void Third3_Callback(void);
void Third4_Callback(void);
void Third5_Callback(void);

void Menu_Input(uint8_t cur_item_ok,MENU_t menu_ok,\
								MENU_t *sub_menu_ok,uint8_t cur_item_next_ok,uint8_t menu_flag_ok);
void Menu_Return(void);

void MY_Menu_Init(void);
void Menu_Switch(Button_Operation oper);
void Menu_Philology_Display(void);
void Menu_Image_Display(uint8_t image_index);
void Menu_Mode_Change(void);




#ifdef __cplusplus
}
#endif

#endif