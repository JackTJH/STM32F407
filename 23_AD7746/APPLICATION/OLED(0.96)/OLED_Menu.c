#include "OLED_Menu.h"
#include "delay.h"
#include "uart.h"
#include <stdlib.h>

MENU_t menu;
MENU_t *prev_menu[MENU_LEVEL];

MENU_Item_t menu_item[] = 
{
	{"Item1",Item1_Callback},
	{"Item2",Item2_Callback},
	{"Item3",Item3_Callback},
	{"Item4",Item4_Callback},
	{"Item5",Item5_Callback},
};

MENU_Item_t second_menu_item[] = 
{
	{"Item2-1",Sub1_Callback},
	{"Item2-2",Sub2_Callback},
	{"Item2-3",Sub3_Callback},
	{"Item2-4",Sub4_Callback},
	{"Item2-5",Sub5_Callback},
};

MENU_Item_t third_menu_item[] = 
{
	{"Item3-1",Third1_Callback},
	{"Item3-2",Third2_Callback},
	{"Item3-3",Third3_Callback},
	{"Item3-4",Third4_Callback},
	{"Item3-5",Third5_Callback},
};

void Item1_Callback(I2C_TYPE i2c_type,I2CX_Typedef i2cx)
{
//	Menu_Input(0,menu,menu.sub2_menu,0,1);
	menu.cur_item = 1;
	OLED_Clear();
	OLED_Printf(0,0,OLED_8X16,"CAP");
	OLED_Update(i2c_type,i2cx);
}

void Item2_Callback(I2C_TYPE i2c_type,I2CX_Typedef i2cx)
{
	menu.cur_item = 1;
	OLED_Clear();
	OLED_Update(i2c_type,i2cx);
}

void Item3_Callback(I2C_TYPE i2c_type,I2CX_Typedef i2cx)
{
	
}

void Item4_Callback(I2C_TYPE i2c_type,I2CX_Typedef i2cx)
{
	
}

void Item5_Callback(I2C_TYPE i2c_type,I2CX_Typedef i2cx)
{
	
}

void Sub1_Callback(I2C_TYPE i2c_type,I2CX_Typedef i2cx)
{
	
}

void Sub2_Callback(I2C_TYPE i2c_type,I2CX_Typedef i2cx)
{
	
}

void Sub3_Callback(I2C_TYPE i2c_type,I2CX_Typedef i2cx)
{
	
}

void Sub4_Callback(I2C_TYPE i2c_type,I2CX_Typedef i2cx)
{
	
}

void Sub5_Callback(I2C_TYPE i2c_type,I2CX_Typedef i2cx)
{
	
}

void Third1_Callback(I2C_TYPE i2c_type,I2CX_Typedef i2cx)
{
	
}

void Third2_Callback(I2C_TYPE i2c_type,I2CX_Typedef i2cx)
{
	
}

void Third3_Callback(I2C_TYPE i2c_type,I2CX_Typedef i2cx)
{
	
}

void Third4_Callback(I2C_TYPE i2c_type,I2CX_Typedef i2cx)
{
	
}

void Third5_Callback(I2C_TYPE i2c_type,I2CX_Typedef i2cx)
{
	
}

void Menu_Switch(Button_Operation oper,I2C_TYPE i2c_type,I2CX_Typedef i2cx)
{
	switch(oper)
	{
		case BUTTON_Default:
			Menu_Mode_Change(i2c_type,i2cx);
			break;
		case BUTTON_Next_page:
			if(menu.cur_item <menu.item_num - 1)
			{
				menu.cur_item++;
				Menu_Mode_Change(i2c_type,i2cx);
			}
			break;
		case BUTTON_Pre_page:
			if(menu.cur_item > 0)
			{
				menu.cur_item--;
				Menu_Mode_Change(i2c_type,i2cx);
			}
			break;
		case BUTTON_UNCONDITION_INSERT:
			if(menu.menu_flag < MENU_LEVEL)
			{
				if(menu.menu_flag == 0)
				{
					Menu_Input(0,menu,menu.sub2_menu,0,1,i2c_type,i2cx);
				}
				else if(menu.menu_flag == 1)
				{
					Menu_Input(0,menu,menu.sub3_menu,0,2,i2c_type,i2cx);
				}
			}
			break;
		case BUTTON_INSERT:
			menu.item[menu.cur_item].func(i2c_type,i2cx);
			break;
		case BUTTON_EXTI:
			Menu_Return(i2c_type,i2cx);
			break;
		default :
			break;
	}
}

void MY_Menu_Init(void)
{
	menu.item = menu_item;
	menu.item_num = sizeof(menu_item)/sizeof(menu_item[0]);
	menu.cur_item = 0;
	menu.menu_flag = 0;

#if MENU_LEVEL >= 2
	menu.sub2_menu = malloc(sizeof(MENU_t));
	if(menu.sub2_menu != NULL)
	{
		menu.sub2_menu->item = second_menu_item;
		menu.sub2_menu->item_num = sizeof(second_menu_item)/sizeof(second_menu_item[0]);
		menu.sub2_menu->cur_item = 0;
#if MENU_LEVEL >= 3
		menu.sub2_menu->sub3_menu = malloc(sizeof(MENU_t));
		if(menu.sub2_menu->sub3_menu != NULL)
		{
			menu.sub2_menu->sub3_menu->item = third_menu_item;
			menu.sub2_menu->sub3_menu->item_num = sizeof(third_menu_item)/sizeof(third_menu_item[0]);
			menu.sub2_menu->sub3_menu->cur_item = 0;
		}
#endif		
	}
	else
	{
		free(menu.sub2_menu);
	}
#endif	
}

void Menu_Input(uint8_t cur_item_ok,MENU_t menu_ok,MENU_t *sub_menu_ok,\
								uint8_t cur_item_next_ok,uint8_t menu_flag_ok,I2C_TYPE i2c_type,I2CX_Typedef i2cx)
{	
	menu.cur_item = cur_item_ok;	
	if(menu_flag_ok < MENU_LEVEL)
	{
		prev_menu[menu_flag_ok] = malloc(sizeof(MENU_t));
		if(prev_menu[menu_flag_ok] != NULL)
		{
			*prev_menu[menu_flag_ok] = menu_ok;
		}
	}
	menu = *sub_menu_ok;									//切换到二级菜单
	menu.cur_item = cur_item_next_ok;			//设置当前项为0
	menu.menu_flag = menu_flag_ok;				//设置为二级菜单
	Menu_Mode_Change(i2c_type,i2cx);
}

void Menu_Return(I2C_TYPE i2c_type,I2CX_Typedef i2cx)
{
	OLED_Clear();
	if(menu.menu_flag == 1)
	{
		menu = *prev_menu[menu.menu_flag];
		menu.cur_item = 0;
		menu.menu_flag = 0;
	}
	else if(menu.menu_flag == 2)
	{
		menu = *prev_menu[menu.menu_flag];
		menu.cur_item = 0;
		menu.menu_flag =1;
	}
	Menu_Mode_Change(i2c_type,i2cx);
}

void Menu_Mode_Change(I2C_TYPE i2c_type,I2CX_Typedef i2cx)
{
#if MENU_MODE == 0
	Menu_Philology_Display(i2c_type,i2cx);
#endif
	
#if MENU_MODE == 1
	Menu_Image_Display(menu.cur_item,i2c_type,i2cx);
#endif	
	
}
	

void Menu_Philology_Display(I2C_TYPE i2c_type,I2CX_Typedef i2cx)
{
	OLED_Clear();
	uint8_t i;
	uint8_t x = 0, y = 0;
	uint8_t menu_display;																					//显示标志位
	uint16_t pos;																									//定义像素变量
	char str[20];	
	static uint8_t old_item = 0;																	//旧选框参数
	uint8_t old_y = y + old_item * OLED_Rectangle_Hight;					//就选框坐标
	uint8_t new_y = y + menu.cur_item * OLED_Rectangle_Hight;			//新选框坐标
	uint8_t visiable_items = (menu.item_num<3)?menu.item_num:3;		//动态设置屏幕可见菜单子项数量 
	uint8_t start_index = (menu.cur_item<2)?0:menu.cur_item - 2;	//根据当前选项计算起始项
	
	if(start_index + visiable_items > menu.item_num)							//防止越界
	{
		start_index = menu.item_num - visiable_items;
	}
	for(i = 0; i < visiable_items; i++)
	{
		if(start_index + i < menu.item_num)
		{
			sprintf((char*)&str,"%s",menu.item[start_index+i].name);
			OLED_ShowString(x+5,y+i* OLED_Rectangle_Hight+2,str,OLED_8X16);
		}
	}
	OLED_DrawRectangle(x,new_y,OLED_Width,OLED_Rectangle_Hight,OLED_UNFILLED);
	
	if(menu.cur_item >= 3)
	{
		pos = 26;																										//选项固定
		menu_display = 1;																						//显示大于屏幕项标志位
	}
	
	if(old_item != menu.cur_item)
	{
		for(pos = old_y; pos != new_y; pos += (new_y>old_y)?2:-2)
		{
			OLED_Clear();
			for(i = 0; i < visiable_items; i++)
			{
				if(start_index + i < menu.item_num)
				{
					sprintf((char*)&str,"%s",menu.item[start_index+i].name);
					OLED_ShowString(x+5,y+i* OLED_Rectangle_Hight+2,str,OLED_8X16);
				}
			}	
			if(menu_display == 1)
			{
				OLED_DrawRectangle(x,pos-(OLED_Rectangle_Hight*(menu.cur_item-2)),OLED_Width,OLED_Rectangle_Hight,OLED_UNFILLED);
			}
			else
			{
				OLED_DrawRectangle(x,pos,OLED_Width,20,OLED_UNFILLED);		//动态显示移动框
				OLED_Update(i2c_type,i2cx);
			}
		}
		old_item = menu.cur_item;
	}
	OLED_Update(i2c_type,i2cx);
}

void Menu_Image_Display(uint8_t image_index,I2C_TYPE i2c_type,I2CX_Typedef i2cx)
{
	static uint8_t old_item = 0;
	const uint8_t *image_data_level_1[] = {cap,ear,game,chess,fish,music,badminton};			//一级菜单图片
	const uint8_t *image_data_level_2[] = {book,tool,brush};													//二级菜单图片
	const uint8_t *image_data_level_3[] = {caravan,shopping,telephone,telescope};			//三级菜单图片
	const uint8_t **image_data;																												//指向图片数据数组的指针
	
	if(menu.menu_flag == 0)
	{
		image_data = image_data_level_1;
	}
	else if(menu.menu_flag == 1)
	{
		image_data = image_data_level_2;
	}
	else if(menu.menu_flag == 2)
	{
		image_data = image_data_level_3;
	}
	
	size_t image_data_size;
	if(menu.menu_flag == 0)
	{
		image_data_size = sizeof(image_data_level_1)/sizeof(image_data_level_1[0]);
	}
	else if(menu.menu_flag == 1)
	{
		image_data_size = sizeof(image_data_level_2)/sizeof(image_data_level_2[0]);
	}
	else if(menu.menu_flag == 2)
	{
		image_data_size = sizeof(image_data_level_3)/sizeof(image_data_level_3[0]);
	}
	
	if(image_index < image_data_size)
	{
		OLED_ShowImage(32,0,64,64,image_data[image_index]);
		OLED_ShowImage(0,16,32,32,left);
		OLED_ShowImage(96,16,32,32,right);
		OLED_Update(i2c_type,i2cx);
		
		if(old_item != menu.cur_item)
		{
			if(menu.cur_item - old_item < 0)
			{
				OLED_ClearArea(0,16,32,32);
				OLED_Update(i2c_type,i2cx);
				Delay_ms(100);
				OLED_ShowImage(0,16,32,32,left);
			}
			else if(menu.cur_item - old_item > 0)
			{
				OLED_ClearArea(96,16,32,32);
				OLED_Update(i2c_type,i2cx);
				Delay_ms(100);
				OLED_ShowImage(96,16,32,32,right);
			}
			OLED_Update(i2c_type,i2cx);
		}
		old_item = menu.cur_item;
		OLED_Update(i2c_type,i2cx);
	}
	else
	{
		return;
	}
}