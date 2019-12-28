#ifndef __USART1_H
#define	__USART1_H

#include "sys_config.h"


void USART1_Config(void);
void USART2_Config(void);

void UsartSend(u16 ch);
void UART1_Send_char(u8* c, u8 len);
void USART_putchar(USART_TypeDef* USART_x, unsigned char ch);

int fputc(int ch, FILE *f);
int fgetc(FILE *f);

#endif /* __USART1_H */
