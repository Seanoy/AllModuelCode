#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "MAX30100.h"
#include "MAX30100_PulseOximeter.h"
#include "MAX30100_SpO2Calculator.h"
#include "MAX30100_Filters.h"
#include "timer3.h"
#include "myiic.h"
/************************************************
  ALIENTEK��ӢSTM32������  
 ���ߣ�Ψ������ 
 CSDN���ͣ�https://blog.csdn.net/tichimi3375 
 SCL-PB12
 SDA-PB11
************************************************/
 int main(void)
 {		
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 //���ڳ�ʼ��Ϊ115200
 	//LED_Init();			     //LED�˿ڳ�ʼ��
	//KEY_Init();          //��ʼ���밴�����ӵ�Ӳ���ӿ�
	TIM3_Int_Init(100-1,720-1);//??1ms??
	IIC_Init();	
	SPO2_Init();
 	while(1)
	{
		POupdate();//����FIFO���� Ѫ������ �������� 
		delay_ms(10);
	}	 
 }

