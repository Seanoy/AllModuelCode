#ifndef __SPI_H
#define __SPI_H
#include "gd32e23x.h"
#include "systick.h"

#define SPI_CLOCK       RCU_GPIOB
#define SPI_GPIO        GPIOB

#define SPI_MOSI_PIN    GPIO_PIN_5
#define SPI_MISO_PIN    GPIO_PIN_4
#define SPI_SCK_PIN     GPIO_PIN_3


#define SPI_CS_CLOCK    RCU_GPIOA

#define SPI_CS_GPIO     GPIOA
#define SPI_CS_PIN      GPIO_PIN_4

#define MOSI_1()    (*(volatile uint32_t *)0x48000418U) = 0x00000020
#define MOSI_0()    (*(volatile uint32_t *)0x48000428U) = 0x00000020
    
//#define MISO_1()    (*(volatile uint32_t *)0x48000418U) = 0x00000040
//#define MISO_0()    (*(volatile uint32_t *)0x48000428U) = 0x00000040
    
#define MISO_RD()   ((*(volatile uint32_t *)0x48000410U)&(0x00000010))

#define SCK_1()		(*(volatile uint32_t *)0x48000418U) = 0x00000008
#define SCK_0()     (*(volatile uint32_t *)0x48000428U) = 0x00000008
    
#define CS_1()		(*(volatile uint32_t *)0x48000018U) = 0x00000010
#define CS_0()      (*(volatile uint32_t *)0x48000028U) = 0x00000010

#define SPI_Delay()  delay_us(20)

uint8_t SPI_Init(void);
void SPI_WriteByte(uint8_t byte);
uint8_t	SPI_ReadByte(void);
uint8_t SPI_ReadID(void);

void SPI_Transmit(u8 *data, int length);

void SPI_W_3BYTE(uint8_t bPage, uint8_t bAddr, uint8_t bData);
void SPI_W_NBYTE(uint8_t bPage, uint8_t bStart_Addr, uint8_t blength);

#endif
