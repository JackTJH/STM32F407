
1.�����������bug,���ڲ���Comparelֱ�Ӹ�ֵ�ǿ���ʹ�õģ����Ƕ���һ���������ݸ������ǲ���
void TIM_SetCompare1(TIM_TypeDef* TIMx, uint32_t Compare1);

���˺�����װ֮���ֵ����������
2.�����˺�����װ֮����PWM_TIM->CCR1 = led_pwm;ʹ�ò��ˣ�����void TIM_SetCompare1(TIM_TypeDef* TIMx, uint32_t Compare1);ȴ�ֿ�����

3.int main(int argc,char *argv[])
{
	__IO uint8_t LED_Pluse = 0;

	LED_Init(CORE_LED);				//��ʼ�����İ�LED
	Key_Init();						//������ʼ��
	System_Tick_Init();				//�δ�ʱ����ʼ��
	BASE_Timer_Init(84,1000);		//������ʱ����ʼ��,����������õ�1ms�ж�
	PWM_Config_Init(84,100);		//������Ƶ��Ϊ10k
	
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

�����LED_Pluse����ǰ������__IO�������ַ�ʽ��CCR1���ò��ˣ���֮����������

4.��ʹ��16���Ƶ�һ��λ����λ�ж�if((g_timxchy_cap_sta & 0x40) == 0x40)��if((g_timxchy_cap_sta & 0x40) == 1)���ǵȼ۵ġ�
����if((g_timxchy_cap_sta & 0x40) == 0x40)��if((g_timxchy_cap_sta & 0x40))�ǵȼ۵�

5.��������

��ѯ������ַ
AT+LADDR
+LADDR=90:E2:02:30:49:24

������������
AT+NAMEJackTJH
+NAME=JackTJH
OK

���������
AT+PIN080952
+PIN=080952
OK

�����λ
AT+RESET
OK

6.����str��һЩ����
StrTemp = strstr(ReceiveData,"Sonic"); ԭ�ַ�����TJH,Sonic=10      StrTemp-> Sonic=10
StrTemp = strtok(StrTemp,"=");		   ԭ�ַ�����Sonic=10          StrTemp-> Sonic
StrTemp = strtok(NULL,"=");		       ԭ�ַ�����Sonic=10          StrTemp-> 10

num = atoi(StrTemp);				   ���ַ���ת��Ϊ����

7.����malloc
void DHT11_Calculator(float *c_dat)
{
//	uint8_t dat[5];
	
	uint8_t *dat = (uint8_t *)malloc(5 * sizeof(uint8_t));  // ��̬����5���ֽڵ��ڴ�
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
	free(dat);  // �ͷŶ�̬������ڴ�
}
��mallocһ��Ҫ����free��Ȼ��ջ�ռ������
���о����������uint8_t *dat���������Ǵ���
��Ϊ�ڣ�DHT11_Calculator�������ж�����һ��δ��ʼ����ָ�� uint8_t *dat��Ȼ��ֱ�Ӵ������� DHT11_ReadData(dat) ���������� dat δָ���κ���Ч���ڴ��ַ������� DHT11_ReadData �д洢����ʱ��ʵ���ϲ�û��д����Ч���ڴ��ַ���⵼�� dat �е����ݶ�ȡʱΪ 0��
����취��
һ��һ��������������
����uint8_t dat[5];  // ����һ��ʵ�ʵ��������ڴ洢��DHT11��ȡ������
����uint8_t local_dat[5];  // ����һ���ֲ�����
uint8_t *dat = local_dat;  // �� dat ָ�� local_dat ���׵�ַ

8.���ڿ��Ź���bug���ڶ�ʱ������ֻ��������ι����������⣬һ����������������������⣨��ʱδ�����
WWDG_SetPrescaler(WWDG_Prescaler_8);//һ�ζ೤ʱ��(4096*8)/42M = 780us
WWDG_SetWindowValue(90);��9��Ƶ�������ֻ�ܡ�100��Ȼι��ʧ��
��������������Լ����
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
ֱ���ڶ�ʱ���жϺ�������ι�������
if(TimeBaseGetFlag(BASE_40MS))
{
	TimeBaseClearFlag(BASE_40MS);
	WWDG_SetCounter(127);	
}
���ﻹҪע��������Ź��������21.06��������Ҫ����������
����ʵʱ�Բ���,����ι���˹�����˵�������Ҫ����ʵʱ��Ҫ��ߵ�ʱ��ֱ������Ӧ���жϺ������洦��
������ﴮ��ʹ�ó����������Ͼ�����Ҫʹ����ͬ�Ĵ��ڣ���Ȼ�������,��������ʹ��115200���ϵĲ�����