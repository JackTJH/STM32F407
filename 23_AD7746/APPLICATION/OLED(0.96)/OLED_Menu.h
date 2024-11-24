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
	void (*func)(I2C_TYPE i2c_type,I2CX_Typedef i2cx);
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

void Item1_Callback(I2C_TYPE i2c_type,I2CX_Typedef i2cx);
void Item2_Callback(I2C_TYPE i2c_type,I2CX_Typedef i2cx);
void Item3_Callback(I2C_TYPE i2c_type,I2CX_Typedef i2cx);
void Item4_Callback(I2C_TYPE i2c_type,I2CX_Typedef i2cx);
void Item5_Callback(I2C_TYPE i2c_type,I2CX_Typedef i2cx);

void Sub1_Callback(I2C_TYPE i2c_type,I2CX_Typedef i2cx);
void Sub2_Callback(I2C_TYPE i2c_type,I2CX_Typedef i2cx);
void Sub3_Callback(I2C_TYPE i2c_type,I2CX_Typedef i2cx);
void Sub4_Callback(I2C_TYPE i2c_type,I2CX_Typedef i2cx);
void Sub5_Callback(I2C_TYPE i2c_type,I2CX_Typedef i2cx);

void Third1_Callback(I2C_TYPE i2c_type,I2CX_Typedef i2cx);
void Third2_Callback(I2C_TYPE i2c_type,I2CX_Typedef i2cx);
void Third3_Callback(I2C_TYPE i2c_type,I2CX_Typedef i2cx);
void Third4_Callback(I2C_TYPE i2c_type,I2CX_Typedef i2cx);
void Third5_Callback(I2C_TYPE i2c_type,I2CX_Typedef i2cx);

void Menu_Input(uint8_t cur_item_ok,MENU_t menu_ok,MENU_t *sub_menu_ok,\
								uint8_t cur_item_next_ok,uint8_t menu_flag_ok,I2C_TYPE i2c_type,I2CX_Typedef i2cx);
void Menu_Return(I2C_TYPE i2c_type,I2CX_Typedef i2cx);

void MY_Menu_Init(void);
void Menu_Switch(Button_Operation oper,I2C_TYPE i2c_type,I2CX_Typedef i2cx);
void Menu_Philology_Display(I2C_TYPE i2c_type,I2CX_Typedef i2cx);
void Menu_Image_Display(uint8_t image_index,I2C_TYPE i2c_type,I2CX_Typedef i2cx);
void Menu_Mode_Change(I2C_TYPE i2c_type,I2CX_Typedef i2cx);




#ifdef __cplusplus
}
#endif

#endif