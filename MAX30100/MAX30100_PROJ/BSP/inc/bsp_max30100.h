#ifndef __BSP_MAX30100_H
#define __BSP_MAX30100_H

#include "bsp_iic.h"
#include <stdio.h>
#include "bsp_systick.h"

#define MAX30100_SLAVE_ADDRESS    0xAE

//STATUS
#define InterruptStatus    0x00		
#define InterruptEnable    0x01		

#define INTERRUPT_REG_A_FULL  			(0X01<<7)		//FIFO Almost Full Flag
#define INTERRUPT_REG_TEMP_RDY  		(0X01<<6)		//Temperature Ready Flag
#define INTERRUPT_REG_HR_RDY  			(0X01<<5)		//Heart Rate Data Ready
#define INTERRUPT_REG_SPO2_RDY  		(0X01<<4)		//SpO 2 Data Ready
#define INTERRUPT_REG_PWR_RDY  			(0X01<<0)		//Power Ready Flag

//FIFO
#define FIFOWritePointer    0x02	
#define OverFlowCounter    0x03		
#define FIFOReadPointer    0x04		
#define FIFODataRegister    0x05	

//CONFIGURATION
#define ModeConfiguration    0x06	
#define SPO2Configuration    0x07
#define LEDConfiguration    0x09	

//TEMPERATURE
#define TempInteger    0x16
#define TempFraction    0x17

//PART ID
#define RevisionID    0xFE		//return 0xXX(0x00~0xFF)
#define PartID    0xFF		//return 0x11

void MAX30100_ReadData(u8 reg_add,u8 *Read,u8 num);
void MAX30100_WriteReg(u8 reg_add,u8 reg_dat);

void MAX30100_Init(void);
void ReadID(void);
void test(void);
void GetTemp(void);
void GetHeartRateNSPO2(void);
#endif
