//****************************************************************//
//��ʱ������ʱ����ʼ������
// ��˾��    ���ɵ���
//****************************************************************//

#include "delay.h"


static u8  fac_us=0;
static u16 fac_ms=0;

void delay_init()	 
{

	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//ϵͳʱ�ӳ�ʼ����ѡ���ⲿʱ��  HCLK/8
	fac_us=SystemCoreClock/8000000;	//Ϊϵͳʱ�ӵ�1/8  
	fac_ms=(u16)fac_us*1000;//����ÿ��ms��Ҫ��systickʱ����   
}								    
	    								   
void delay_us(u32 nus)//��ʱus
{		
	u32 temp;	    	 
	SysTick->LOAD=nus*fac_us; 	 
	SysTick->VAL=0x00;       
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;       
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;      
	SysTick->VAL =0X00;        
}

void delay_ms(u16 nms)//��ʱms
{	 		  	  
	u32 temp;		   
	SysTick->LOAD=(u32)nms*fac_ms;
	SysTick->VAL =0x00;           
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;        
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;      
	SysTick->VAL =0X00;     
} 

































