#ifndef __BSP_SYSTICK_H 
#define __BSP_SYSTICK_H 			   
#include "bsp_sys.h"  
 
void delay_init(void);
void delay_ms(u16 nms);
void delay_us(u32 nus);
void SysTick_Handler(void);
#endif





