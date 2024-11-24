#include "button.h"

static void button_cycle_process(BTN_HandleTypedef *btn);
static void button_add(BTN_HandleTypedef *btn);
static void button_info_print(BTN_HandleTypedef *btn);

static struct button *button_head = NULL;

void button_create(const char *name, BTN_HandleTypedef *btn, 
        uint8_t (*read_btn_level)(void), uint8_t btn_trigger_level)
{
    if (btn == NULL)
    {
        PRINT_DBG("button is null\r\n");
        return;
    }

    memset(btn, 0, sizeof(struct button));
    strncpy(btn->name, name, BTN_NAME_MAX);

    btn->button_state = BTN_NONE;                  
    btn->button_last_state = BTN_NONE;             
    btn->event = BTN_EV_NULL;
    btn->read_btn_level = read_btn_level;           
    btn->button_trigger_level = btn_trigger_level;     
    btn->button_last_level = btn->read_btn_level(); 
    btn->debounce_count = 0;
    btn->timer_count = 0;
    btn->button_trigger_cnt = 0;

    PRINT_DBG("button create success!\r\n");
    button_info_print(btn);

    button_add(btn);
}


void button_attach(BTN_HandleTypedef *btn, BTN_EVENT btn_event, BTN_CALLBACK btn_callback)
{
    if (btn == NULL)
    {
        PRINT_DBG("button is null\r\n");
        return;
    }

    btn->button_callback[btn_event] = btn_callback;
}


void button_delete(BTN_HandleTypedef *btn)
{
    struct button **curr;
    struct button *entry;
    
    if(btn == NULL)
        return;

    curr = &button_head;
    while(*curr != NULL)
    {
        entry = *curr;
        if (entry == btn)
            *curr = entry->next;
        else
            curr = &entry->next;
    }
}


void button_cycle_process(BTN_HandleTypedef *btn)
{
    uint8_t current_level = (uint8_t)btn->read_btn_level();

    if ((current_level != btn->button_last_level) &&
            (++btn->debounce_count >= BTN_DEBOUNCE_TIME))
    {
        btn->button_last_level = current_level;
        btn->debounce_count = 0;

        if (btn->button_trigger_level == current_level)
        {
            btn->button_state = BTN_DOWM;
            btn->timer_count = 0;
            PRINT_DBG("button %s is down\r\n", btn->name);
        }
        else
        {
            btn->button_state = BTN_UP;
            PRINT_DBG("button %s is up\r\n", btn->name);
        }
    }

    btn->timer_count++;
    switch (btn->button_state)
    {
    case BTN_DOWM:
        if (btn->timer_count > BTN_LONG_TIME && btn->button_last_level == current_level)
        {
            if (btn->timer_count % 4 == 0)
                btn->event = BTN_EV_LONG_CLICK;
        }
        break;

    case BTN_UP:
        if (btn->event == BTN_EV_LONG_CLICK)
        {
            btn->event = BTN_EV_NULL;
        }

        if (btn->timer_count < BTN_DOUBLE_TIME)
        {
            if (btn->button_last_state == BTN_DOWM)
            {
                btn->button_trigger_cnt++;
                btn->timer_count = 0;
            }
        }

        if (btn->timer_count > BTN_DOUBLE_TIME)
        {
            btn->timer_count = 0;

            if (btn->button_trigger_cnt == 1)
            {
                btn->event = BTN_EV_CLICK;
            }
            else if (btn->button_trigger_cnt > 1)
            {
                btn->event = BTN_EV_DOUBLE_CLICK;
            }
            btn->button_trigger_cnt = 0;
            btn->timer_count = 0;
        }
        break;
    }

    btn->button_last_state = btn->button_state;
    if (btn->event < BTN_EV_CNT && btn->button_callback[btn->event] != NULL)
    {
        btn->button_callback[btn->event](btn);
        btn->event = BTN_EV_NULL;
    }
}


void button_process(void)
{
    struct button *pass_btn;
    for (pass_btn = button_head; pass_btn != NULL; pass_btn = pass_btn->next)
    {
        button_cycle_process(pass_btn);
    }
}

static void button_add(BTN_HandleTypedef *btn)
{
    btn->next = button_head;
    button_head = btn;
}


static void button_info_print(BTN_HandleTypedef *btn)
{
    PRINT_DBG("button struct information:\r\n");
    PRINT_DBG("btn->name:%s\r\n", btn->name);
    PRINT_DBG("btn->button_state:%d\r\n", btn->button_state);
    PRINT_DBG("btn->button_last_state:%d\r\n", btn->button_last_state);
    PRINT_DBG("btn->button_trigger_level:%d\r\n", btn->button_trigger_level);
    PRINT_DBG("btn->button_last_level:%d\r\n", btn->button_last_level);
    PRINT_DBG("btn->event:%d\r\n", btn->event);
}

