/**
  ******************************************************************************
  * @file    sys_config.h
  * @author  zjj
  * @version V1.0
  * @date    2014-10-10
  * @brief   ϵͳ����ͷ�ļ�
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:stm32f103��Ƭ��
  * ��̳    :
  * �Ա�    :
  *
  ******************************************************************************
  */
  
#ifndef __SYS_CONFIG_H
#define __SYS_CONFIG_H

/********************************************************************************/
//stm32ͷ�ļ�
#include "stm32f10x.h"
#include "stm32f10x_it.h"
//C��ͷ�ļ�
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h> 
#include <string.h>

//����ͷ�ļ�
#include "bsp_usart.h"
#include "bsp_SysTick.h"

//SX1278
#include "platform.h"
#include "radio.h"

/********************************************************************************/


//GPIO�����������ú궨��--����ԭ��
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO�ڵ�ַӳ��
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C 
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C 
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C 
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C    
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C    

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008 
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //0x40011408 
#define GPIOE_IDR_Addr    (GPIOE_BASE+8) //0x40011808 
#define GPIOF_IDR_Addr    (GPIOF_BASE+8) //0x40011A08 
#define GPIOG_IDR_Addr    (GPIOG_BASE+8) //0x40011E08 
 
//IO�ڲ���,ֻ�Ե�һ��IO��!
//ȷ��n��ֵС��16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //��� 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //���� 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //��� 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //���� 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //��� 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //���� 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //��� 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //���� 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //��� 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //����

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //��� 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //����

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //��� 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //����


/*******************************************************************************/
/********************** System_Parameter_Config ********************************/
/*******************************************************************************/

//USART_BUAD_CONFIG
#define BAUD    115200
//USART_PRINTF_CONFIG
#define USED_COM_NUMBER     USART1

//GPIO CONFIG
#define RCC_LED_GPIOx   RCC_APB2Periph_GPIOB
#define LED0_PINx       GPIO_Pin_5
#define LED0_GPIOx      GPIOB
#define LED0_ON         GPIO_ResetBits(LED0_GPIOx, LED0_PINx)
#define LED0_OFF        GPIO_SetBits(LED0_GPIOx, LED0_PINx)
#define LED0_TOGGLE     GPIO_PinReverse(LED0_GPIOx, LED0_PINx)
/*******************************************************************************/
/******************** End of System_Parameter_Config ***************************/
/*******************************************************************************/


//��������
void sys_Configuration(void);
void RCC_Configuration(void);
void NVIC_Configuration(void);
void GPIO_Configuration(void);

void GPIO_PinReverse(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

void Soft_delay_us(u16 time);
void Soft_delay_ms(u16 time);


#endif
