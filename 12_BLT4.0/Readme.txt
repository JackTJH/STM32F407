
1.这个给函数有bug,对于参数Comparel直接赋值是可以使用的，但是定义一个变量传递给他就是不行
void TIM_SetCompare1(TIM_TypeDef* TIMx, uint32_t Compare1);

做了函数封装之后发现的问题和现象
2.我做了函数封装之后发现PWM_TIM->CCR1 = led_pwm;使用不了，但是void TIM_SetCompare1(TIM_TypeDef* TIMx, uint32_t Compare1);却又可以了

3.int main(int argc,char *argv[])
{
	__IO uint8_t LED_Pluse = 0;

	LED_Init(CORE_LED);				//初始化核心板LED
	Key_Init();						//按键初始化
	System_Tick_Init();				//滴答定时器初始化
	BASE_Timer_Init(84,1000);		//基本定时器初始化,这里参数配置的1ms中断
	PWM_Config_Init(84,100);		//方波的频率为10k
	
	while(1)
	{
		
		for(LED_Pluse = 0; LED_Pluse<100; LED_Pluse += 10)
		{
			PWM_TIM->CCR1 = LED_Pluse;
//			TIM_SetCompare1(PWM_TIM,LED_Pluse);
			Delay_ms(300);
		}
		
		for(LED_Pluse = 100; LED_Pluse>0; LED_Pluse -= 10)
		{
			PWM_TIM->CCR1 = LED_Pluse;
//			TIM_SetCompare1(PWM_TIM,LED_Pluse);
			Delay_ms(300);
		}
	}
}

这里的LED_Pluse变量前面必须加__IO不加两种方式改CCR1都用不了，反之都可以用了

4.在使用16进制的一个位作按位判断if((g_timxchy_cap_sta & 0x40) == 0x40)和if((g_timxchy_cap_sta & 0x40) == 1)不是等价的。
反而if((g_timxchy_cap_sta & 0x40) == 0x40)和if((g_timxchy_cap_sta & 0x40))是等价的

5.关于蓝牙

查询蓝牙地址
AT+LADDR
+LADDR=90:E2:02:30:49:24

设置蓝牙名字
AT+NAMEJackTJH
+NAME=JackTJH
OK

设置配对码
AT+PIN080952
+PIN=080952
OK

软件复位
AT+RESET
OK