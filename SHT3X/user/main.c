#include "bsp.h"

int main(){
	BSP_init();
    SHT30_Init();
    while(1){
        SHT30_Read();
//        delay_ms(1000);
    }
}
