#ifndef __SHT30_H
#define __SHT30_H

//#include "bsp_iic.h"
#include "iic.h"
#define SHT30_ADDR      0x44

typedef union 
{
    float fdata;
    unsigned char cdata[4];
}float_data;//����������洢float���ݣ�float���͵Ĵ洢����IEEE��׼�������ڴ�������

void SHT30_Init(void);
void SHT30_SOFTRESET(void);
void SHT30_General_RESET(void);
void SHT30_Stop_Break(void);

void SHT30_Read(void);
void SHT30_SendBytes(u16 cmd,u8 stop);
void SHT30_ART(void);
void SHT30_Single_Shot(u8 *buffer);
void SHT30_Periodic(u8 *buffer);
void SHT30_Heater(u8 enable);



#endif
