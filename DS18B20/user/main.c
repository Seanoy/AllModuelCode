#include "bsp.h"
void init_ds18b20( void );
void check_ds18b20( void );
void ds18b20_self_check( void );
void ds18b20_write_TH_TL_CONF(u8 TH,u8 TL,u8 mode);
void ds18b20_return_TH_TL_CONF( void );
short get_temp( void );
void tem_chage( void );
void ds18b20_read_rom_number();

u8 data_buffer[6];
void twinkle()
{
	GPIO_SetBits(GPIOC,GPIO_Pin_13);
	delay_ms(500);
	GPIO_ResetBits(GPIOC,GPIO_Pin_13);
	delay_ms(500);
}
	 
int main(){	
	BSP_init();
    ds18b20_write_TH_TL_CONF(120,-20,3);
    ds18b20_return_TH_TL_CONF();
    ds18b20_self_check();
        tem_chage();

while(1){
    delay_ms(1000);
    get_temp();
    
    
}	

}
