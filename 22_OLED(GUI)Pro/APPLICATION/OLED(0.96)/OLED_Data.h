#ifndef __OLED_DATA_H
#define __OLED_DATA_H

#include <stdint.h>

/*�ַ�������*/
/*���������궨��ֻ�ɽ������һ����ע��*/
//#define OLED_CHARSET_UTF8			//�����ַ���ΪUTF8
#define OLED_CHARSET_GB2312		//�����ַ���ΪGB2312

/*��ģ������Ԫ*/
typedef struct 
{
	
#ifdef OLED_CHARSET_UTF8			//�����ַ���ΪUTF8
	char Index[5];					//�����������ռ�Ϊ5�ֽ�
#endif
	
#ifdef OLED_CHARSET_GB2312			//�����ַ���ΪGB2312
	char Index[3];					//�����������ռ�Ϊ3�ֽ�
#endif
	
	uint8_t Data[32];				//��ģ����
} ChineseCell_t;

/*ASCII��ģ��������*/
extern const uint8_t OLED_F8x16[][16];
extern const uint8_t OLED_F6x8[][6];

/*������ģ��������*/
extern const ChineseCell_t OLED_CF16x16[];

/*ͼ����������*/
extern const uint8_t Diode[];
extern const uint8_t book[];
extern const uint8_t chess[];
extern const uint8_t tool[];
extern const uint8_t ear[];
extern const uint8_t badminton[];
extern const uint8_t brush[];
extern const uint8_t caravan[];
extern const uint8_t fish[];
extern const uint8_t game[];
extern const uint8_t music[];
extern const uint8_t shopping[];
extern const uint8_t telephone[];
extern const uint8_t telescope[];
extern const uint8_t left[];
extern const uint8_t right[];
/*��������ĸ�ʽ�������λ�ü����µ�ͼ����������*/
//...

#endif


/*****************��Э�Ƽ�|��Ȩ����****************/
/*****************jiangxiekeji.com*****************/
