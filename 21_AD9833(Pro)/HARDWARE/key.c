#include "key.h"
#include "led.h"
#include "ad9833.h"

GPIO_TypeDef* BUTTON_GPIO_PORT[Button_n] = {CORE_BUTTON_PORT};
const uint16_t BUTTON_GPIO_PIN[Button_n] = {CORE_BUTTON_PIN};

void Button_Init(Button_TypeDef Button)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	BUTTONx_GPIO_CLK_ENABLE(Button);

	GPIO_InitStructure.GPIO_Pin = BUTTON_GPIO_PIN[Button];
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init(BUTTON_GPIO_PORT[Button], &GPIO_InitStructure);
	
}

uint8_t Get_Button_Status(Button_TypeDef Button)
{
	return GPIO_ReadInputDataBit(BUTTON_GPIO_PORT[Button],BUTTON_GPIO_PIN[Button]);
}

static BTN_HandleTypedef key1, key2, key3, key4;

static uint8_t get_key_level(Button_TypeDef key)
{
    return Get_Button_Status(key);
}

static uint8_t get_key1()
{
    return get_key_level(CORE_BUTTON);
}

//static uint8_t get_key2()
//{
//    return get_key_level(KEY2);
//}

//static uint8_t get_key3()
//{
//    return get_key_level(KEY3);
//}

//static uint8_t get_key4()
//{
//    return get_key_level(KEY4);
//}

static void key_callback(void *btn)
{
    BTN_HandleTypedef *button = (BTN_HandleTypedef *)btn;
    if (button->event == BTN_EV_CLICK)
    {
        if (strncmp(button->name, "KEY1", 4) == 0)
        {
             printf("KEY1 clicked\r\n");
						 LED_Toggle(CORE_LED);
						 AD9833_1.frequent += 1000;
						 AD9833_1.Phase += 30;
        }
        else if (strncmp(button->name, "KEY2", 4) == 0)
        {
            printf("KEY2 clicked\r\n");
        }
        else if (strncmp(button->name, "KEY3", 4) == 0)
        {
            printf("KEY3 clicked\r\n");
        }
        else if (strncmp(button->name, "KEY4", 4) == 0)
        {
            printf("KEY4 clicked\r\n");
        }
    }
    else if (button->event == BTN_EV_DOUBLE_CLICK)
    {
        if (strncmp(button->name, "KEY1", 4) == 0)
        {
            printf("KEY1 double clicked\r\n");
        }
        else if (strncmp(button->name, "KEY2", 4) == 0)
        {
            printf("KEY2 double clicked\r\n");
        }
        else if (strncmp(button->name, "KEY3", 4) == 0)
        {
            printf("KEY3 double clicked\r\n");
        }
        else if (strncmp(button->name, "KEY4", 4) == 0)
        {
            printf("KEY4 double clicked\r\n");
        }
    }
    else if (button->event == BTN_EV_LONG_CLICK)
    {
        if (strncmp(button->name, "KEY1", 4) == 0)
        {
            printf("KEY1 long clicked\r\n");
        }
        else if (strncmp(button->name, "KEY2", 4) == 0)
        {
            printf("KEY2 long clicked\r\n");
        }
        else if (strncmp(button->name, "KEY3", 4) == 0)
        {
            printf("KEY3 long clicked\r\n");
        }
        else if (strncmp(button->name, "KEY4", 4) == 0)
        {
            printf("KEY4 long clicked\r\n");
        }
    }
}

void key_setup(void)
{
    button_create("KEY1", &key1, get_key1, BTN_LEVEL_HIGH);
    button_attach(&key1, BTN_EV_CLICK, key_callback);
    button_attach(&key1, BTN_EV_DOUBLE_CLICK, key_callback);
    button_attach(&key1, BTN_EV_LONG_CLICK, key_callback);

//    button_create("KEY2", &key2, get_key2, BTN_LEVEL_HIGH);
//    button_attach(&key2, BTN_EV_CLICK, key_callback);
//    button_attach(&key2, BTN_EV_DOUBLE_CLICK, key_callback);
//    button_attach(&key2, BTN_EV_LONG_CLICK, key_callback);

//    button_create("KEY3", &key3, get_key3, BTN_LEVEL_HIGH);
//    button_attach(&key3, BTN_EV_CLICK, key_callback);
//    button_attach(&key3, BTN_EV_DOUBLE_CLICK, key_callback);
//    button_attach(&key3, BTN_EV_LONG_CLICK, key_callback);

//    button_create("KEY4", &key4, get_key4, BTN_LEVEL_LOW);
//    button_attach(&key4, BTN_EV_CLICK, key_callback);
//    button_attach(&key4, BTN_EV_DOUBLE_CLICK, key_callback);
//    button_attach(&key4, BTN_EV_LONG_CLICK, key_callback);
}


