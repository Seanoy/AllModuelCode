#include "stm32f10x.h"
#include "key.h"
#include "sys.h"
#include "delay.h"

//������ʼ������
void key_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG,ENABLE);//ʹ��ʱ��

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0���ó����룬Ĭ������	  
	GPIO_Init(GPIOG, &GPIO_InitStructure);//��ʼ��GPIOG.8
}

unsigned char KEY_Scan(void)
{	 	  
	if(USER_KEY==0)
	{
		delay_ms(15);//ȥ���� 
		if(USER_KEY==0)return DOWN_KEY;		
	}    
	return 0;// �ް�������
}
