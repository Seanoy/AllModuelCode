#ifndef _KEY_H_
#define _KEY_H_
#include "sys.h"

//#define KEY0 PEin(4)   	//PE4
//#define KEY1 PEin(3)	//PE3 
//#define KEY2 PEin(2)	//PE2
//#define KEY3 PAin(0)	//PA0  WK_UP

#define USER_KEY GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_8)//��ȡ����0  PG8

#define DOWN_KEY	1

void key_init(void);//IO��ʼ��
unsigned char KEY_Scan(void);  	//����ɨ�躯��
#endif
