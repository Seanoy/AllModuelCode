#ifndef __WS2812B_H
#define	__WS2812B_H

#include "stm32f10x.h"

#define TIM3_CCR3_Address 0x4000043c 	// physical memory address of Timer 3 CCR1 register
	
#define TIMING_ONE  50 //1Âë£¬0.7us/1.25us=56%=50%
#define TIMING_ZERO 25 //0Âë£¬0.35us/1.25us=28%=25%


typedef enum
{
    WHITE=0,
    RED,
    ORANGE,
    YELLOW,
    GREEN,
    CYAN,
    BLUE,
    PURPLE   
}Color_Type;
extern Color_Type rainbow[30];
void WS2812B_Init(void);
void WS2812_Display(Color_Type color[],uint16_t len, uint8_t display_all);

#endif /* __LED_H */
