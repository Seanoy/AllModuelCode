#include "bsp.h"
extern void  LD3320_main(void);
void twinkle()
{
	GPIO_SetBits(GPIOC,GPIO_Pin_13);
	delay_ms(500);
	GPIO_ResetBits(GPIOC,GPIO_Pin_13);
	delay_ms(500);
}
	 
int main(){	
	BSP_init();
while(1){
LD3320_main();
//	printf("hello!");
//	twinkle();
}	

}
