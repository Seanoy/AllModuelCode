#include "bsp.h"

void USART_Config(void);

void BSP_init(void)
{	
	USART_Config();
	LED_GPIO_Config();
	delay_init();
	i2c_CheckDevice(MAX30100_SLAVE_ADDRESS);	//���ӻ���ַ�Ƿ���ȷ
	MAX30100_Init();
}


