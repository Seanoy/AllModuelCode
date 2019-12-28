#include "bsp.h"
#include "ws2812b.h"

void twinkle()
{
	GPIO_SetBits(GPIOC,GPIO_Pin_13);
	delay_ms(500);
	GPIO_ResetBits(GPIOC,GPIO_Pin_13);
	delay_ms(500);
}

int main(){
    
	BSP_init();
    WS2812B_Init();
    WS2812_Display(rainbow,30,0);
    while(1){
        twinkle();
        //do something
    }
}
