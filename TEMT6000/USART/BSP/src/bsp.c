#include "bsp.h"

void USART_Config(void);

void BSP_init(void)
{	
	USART_Config();
	LED_GPIO_Config();
	delay_init();
	ADC_Configuration();
}


