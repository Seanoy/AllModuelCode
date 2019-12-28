#ifndef __IIC_H
#define __IIC_H

#include "stm32f10x.h"
#include <stdbool.h>
#include <stdlib.h>
#include <inttypes.h>
#include "bsp_systick.h"
#include "bsp_sys.h"

#define IIC_CLOCK           RCC_APB2Periph_GPIOA

#define IIC_GPIO            GPIOA

#define IIC_SCL_PIN         GPIO_Pin_6
#define IIC_SDA_PIN         GPIO_Pin_7
    

/* ����I2C�������ӵ�GPIO�˿�, �û�ֻ��Ҫ�޸�����4�д��뼴������ı�SCL��SDA������ */

#define I2C_SCL_1()     PAout(6)=1
#define I2C_SCL_0()     PAout(6)=0
#define I2C_SDA_1()		PAout(7)=1
#define I2C_SDA_0()     PAout(7)=0
#define I2C_SDA_READ()  GPIO_ReadInputDataBit(IIC_GPIO,IIC_SDA_PIN)


#define I2C_WR	0		/* д����bit */
#define I2C_RD	1		/* ������bit */

void i2c_Start(void);
void i2c_Stop(void);
void i2c_SendByte(uint8_t _ucByte);
uint8_t i2c_ReadByte(uint8_t ack);
uint8_t i2c_WaitAck(void);
void i2c_Ack(void);
void i2c_NAck(void);
uint8_t i2c_CheckDevice(uint8_t _Address);
void i2c_GPIO_Config(void);


#endif
