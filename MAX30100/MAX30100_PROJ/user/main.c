//Author:SeanOY 
//Date:2019.4.25 20£º25
//Email:oyzeyuan@163.com

#include "bsp.h"

void twinkle()
{
	GPIO_SetBits(GPIOC,GPIO_Pin_13);
	delay_ms(500);
	GPIO_ResetBits(GPIOC,GPIO_Pin_13);
	delay_ms(500);
}
	 
int main(){	
	BSP_init();
	ReadID();
while(1){
	test();
	//twinkle();
}	

}
