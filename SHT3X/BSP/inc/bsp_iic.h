#ifndef SHT30_H
#define SHT30_H

#include "bsp.h"//Î»´ø²Ù×÷
#include "string.h"

#define WR  0
#define RD  1

#define IIC_CLOCK       RCC_APB2Periph_GPIOA
#define IIC_GPIO        GPIOA
//#define IIC_SCL_PIN     GPIO_Pin_4
//#define IIC_SDA_PIN     GPIO_Pin_5

#define SCL PAout(4)
#define SDA_OUT PAout(5)
#define SDA_IN PAin(5)
#define IIC_INPUT_MODE_SET()  GPIOA->CRL&=0xFF0FFFFF;GPIOA->CRL|=0x00800000;
#define IIC_OUTPUT_MODE_SET() GPIOA->CRL&=0xFF0FFFFF;GPIOA->CRL|=0x00300000;

void IIC_Init(void);
void IIC_ACK(void);
void IIC_NACK(void);
u8 IIC_Wait_ACK(void);
void IIC_Start(void);
void IIC_Stop(void);
void IIC_TxByte(u8 byte);
u8 IIC_RxByte(void);

#endif
