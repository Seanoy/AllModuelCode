#ifndef __MYIIC_H
#define __MYIIC_H
#include "sys.h"
#include "DHT12.h"
#include "delay.h"

//IO��������
#define SDA_IN()  {GPIOB->CRH&=0XF0FFFFFF;GPIOB->CRH|=0x08000000;}//����������
#define SDA_OUT() {GPIOB->CRH&=0XF0FFFFFF;GPIOB->CRH|=0x03000000;}//ͨ���������


//IO��������	 
#define IIC_SCL    PBout(13) //SCL���
#define IIC_SDA    PBout(14) //SDA���	 
#define READ_SDA   PBin(14)  //SDA���� 

//IIC���в�������
void IIC_Init(void);                //��ʼ��IIC��IO��				 
void IIC_Start(void);				//����IIC��ʼ�ź�
void IIC_Stop(void);	  			//����IICֹͣ�ź�
void IIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void IIC_Ack(void);					//IIC����ACK�ź�
void IIC_NAck(void);				//IIC������ACK�ź�


#endif
















