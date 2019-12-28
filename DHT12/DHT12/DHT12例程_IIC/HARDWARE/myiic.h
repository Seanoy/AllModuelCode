#ifndef __MYIIC_H
#define __MYIIC_H
#include "sys.h"
#include "DHT12.h"
#include "delay.h"

//IO方向设置
#define SDA_IN()  {GPIOB->CRH&=0XF0FFFFFF;GPIOB->CRH|=0x08000000;}//上下拉输入
#define SDA_OUT() {GPIOB->CRH&=0XF0FFFFFF;GPIOB->CRH|=0x03000000;}//通用推挽输出


//IO操作函数	 
#define IIC_SCL    PBout(13) //SCL输出
#define IIC_SDA    PBout(14) //SDA输出	 
#define READ_SDA   PBin(14)  //SDA输入 

//IIC所有操作函数
void IIC_Init(void);                //初始化IIC的IO口				 
void IIC_Start(void);				//发送IIC开始信号
void IIC_Stop(void);	  			//发送IIC停止信号
void IIC_Send_Byte(u8 txd);			//IIC发送一个字节
u8 IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 IIC_Wait_Ack(void); 				//IIC等待ACK信号
void IIC_Ack(void);					//IIC发送ACK信号
void IIC_NAck(void);				//IIC不发送ACK信号


#endif
















