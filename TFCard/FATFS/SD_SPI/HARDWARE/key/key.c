#include "stm32f10x.h"
#include "key.h"
#include "sys.h"
#include "delay.h"

//按键初始化函数
void key_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG,ENABLE);//使能时钟

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0设置成输入，默认下拉	  
	GPIO_Init(GPIOG, &GPIO_InitStructure);//初始化GPIOG.8
}

unsigned char KEY_Scan(void)
{	 	  
	if(USER_KEY==0)
	{
		delay_ms(15);//去抖动 
		if(USER_KEY==0)return DOWN_KEY;		
	}    
	return 0;// 无按键按下
}
