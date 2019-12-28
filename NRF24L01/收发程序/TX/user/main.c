#include "bsp.h"

void twinkle()
{
	GPIO_SetBits(GPIOC,GPIO_Pin_13);
	delay_ms(500);
	GPIO_ResetBits(GPIOC,GPIO_Pin_13);
	delay_ms(500);
}
	 
int main(){		
	u8 txbuf[33]="Can you go out on a date with me?";
	BSP_init();

		while(NRF24L01_Check())//��ⲻ��24L01
	{
		printf("Device Error!\r\n");
	}
	printf("Device Check!\r\n");
	
	TX_Mode();//����ģʽ
while(1)	
{	
	if(NRF24L01_TxPacket(txbuf) == TX_OK)//���ͳɹ�
	{
		printf("Send %s OK!\r\n",txbuf);
		delay_ms(1000);			//���1s����һ��
	}
}	

}
