
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