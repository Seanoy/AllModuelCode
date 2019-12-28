#include "bsp.h"

void USART_Config(void);

void BSP_init(void)
{		
    delay_init();
	USART_Config();
	LED_GPIO_Config();
}


