#include "bsp_led.h"

void LED_GPIO_Config(void){
		
		GPIO_InitTypeDef GPIO_InitStructure;    //PC13×÷Îª²âÊÔµÆ
		
		RCC_APB2PeriphClockCmd ( RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO , ENABLE ); 
																																		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 ;  						 
		
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    	 
		
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;      
		
		GPIO_Init( GPIOC , &GPIO_InitStructure );    
	
		GPIO_SetBits(GPIOC,GPIO_Pin_13);
	}


