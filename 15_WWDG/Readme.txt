
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

6.关于str的一些函数
StrTemp = strstr(ReceiveData,"Sonic"); 原字符串：TJH,Sonic=10      StrTemp-> Sonic=10
StrTemp = strtok(StrTemp,"=");		   原字符串：Sonic=10          StrTemp-> Sonic
StrTemp = strtok(NULL,"=");		       原字符串：Sonic=10          StrTemp-> 10

num = atoi(StrTemp);				   将字符串转化为整数

7.关于malloc
void DHT11_Calculator(float *c_dat)
{
//	uint8_t dat[5];
	
	uint8_t *dat = (uint8_t *)malloc(5 * sizeof(uint8_t));  // 动态分配5个字节的内存
	if (dat == NULL)
	{
		printf("Memory allocation failed\r\n");
		return;
	}
	
//	uint8_t *dat = NULL;
	DHT11_ReadData(dat);
	if(dat[0] + dat[1] + dat[2] + dat[3] == dat[4])
	{
		c_dat[0] = dat[0] + dat[1] / 10.0;
		c_dat[1] = dat[2] + dat[3] / 10.0;
	}
	free(dat);  // 释放动态分配的内存
}
有malloc一定要配套free不然会栈空间溢出。
还有就是如果定义uint8_t *dat这种做法是错误。
因为在（DHT11_Calculator）函数中定义了一个未初始化的指针 uint8_t *dat，然后直接传递它给 DHT11_ReadData(dat) 函数。由于 dat 未指向任何有效的内存地址，因此在 DHT11_ReadData 中存储数据时，实际上并没有写入有效的内存地址，这导致 dat 中的数据读取时为 0。
解决办法：
一、一个是上面程序给出
二、uint8_t dat[5];  // 定义一个实际的数组用于存储从DHT11读取的数据
三、uint8_t local_dat[5];  // 定义一个局部数组
uint8_t *dat = local_dat;  // 让 dat 指向 local_dat 的首地址

8.窗口看门狗有bug，在定时器里面只处理它的喂狗不会出问题，一旦加上其他处理函数会出问题（暂时未解决）
WWDG_SetPrescaler(WWDG_Prescaler_8);//一次多长时间(4096*8)/42M = 780us
WWDG_SetWindowValue(90);在9分频的情况下只能》100不然喂狗失败
对于上面的问题以及解决
volatile uint8_t Cnt40ms = 0;
void TIM6_DAC_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM6,TIM_IT_Update) ==SET)
	{		
		TimeBaseUpdata();
		Cnt40ms++;
		if(Cnt40ms >= 40)
		{
			Cnt40ms = 0;
			WWDG_SetCounter(127);
		}
		TIM_ClearITPendingBit(TIM6,TIM_IT_Update);
	}
}
直接在定时器中断函数里面喂狗如果用
if(TimeBaseGetFlag(BASE_40MS))
{
	TimeBaseClearFlag(BASE_40MS);
	WWDG_SetCounter(127);	
}
这里还要注意如果看门狗算出来是21.06但你至少要大于它三秒
处理实时性不够,导致喂不了狗。因此当我们需要处理，实时性要求高的时候直接在相应的中断函数里面处理
如果这里串口使用超过两个以上尽量不要使用相同的串口，不然会出问题,而且至少使用115200以上的波特率