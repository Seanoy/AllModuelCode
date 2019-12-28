#ifndef __DELAY_H
#define __DELAY_H 			   
#include "sys.h"

void delay_init(void); //延时初始化
void delay_ms(u16 nms);	//延时ms
extern void delay_us(u32 nus);//延时us

#endif





























