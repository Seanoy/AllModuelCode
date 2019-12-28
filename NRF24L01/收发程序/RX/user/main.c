#include "bsp.h"

void twinkle()
{
	GPIO_SetBits(GPIOC,GPIO_Pin_13);
	delay_ms(500);
	GPIO_ResetBits(GPIOC,GPIO_Pin_13);
	delay_ms(500);
}
	 
int main(){		
	u8 rxbuf[33]="Oh,i got nothing...";
	BSP_init();

		while(NRF24L01_Check())//检测不到24L01
	{
		printf("Device Error!\r\n");
	}
	printf("Device Check!\r\n");
	
	RX_Mode();//接收模式	
while(1)	
{	
	if(NRF24L01_RxPacket(rxbuf)==0)//接收成功
	{
		rxbuf[32]='\0';
		printf("%s",rxbuf);
	}
}	

}
