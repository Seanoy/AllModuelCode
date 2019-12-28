//****************************************************************//
//模拟IIC配置程序
//注意：DHT12的IIC的通讯频率不得超过50KHZ
// 公司：    奥松电子
//****************************************************************//

#include "myiic.h"

//初始化IIC
void IIC_Init(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );//使能GPIOB	
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14;//PB13,PB14
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD ;   //开漏输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_14); 	//PB13,PB14 输出高
}

//产生IIC起始信号
void IIC_Start(void)
{
	SDA_OUT();     //sda线输出
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	delay_us(14);
 	IIC_SDA=0;//开始：当SCL为高时，数据从高到低变化
	delay_us(14);
	IIC_SCL=0;//钳住I2C总线，准备发送或接收数据 
}	  
//产生IIC停止信号
void IIC_Stop(void)
{
	SDA_OUT();//sda线输出
	IIC_SCL=0;
	IIC_SDA=0;//停止:当SCL为高时，数据从低到高变化
 	delay_us(14);
	IIC_SCL=1;
	delay_us(14);	 
	IIC_SDA=1;//发送I2C总线结束信号
							   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	IIC_SCL=0;
	IIC_SDA=1;
	SDA_IN();      //SDA设置为输入  
	delay_us(14);	   
	IIC_SCL=1;
	delay_us(14);	 
	while(READ_SDA)//等到SDA变低
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
		delay_us(1);
	}
	IIC_SCL=0;//时钟输出0
	delay_us(10); 
	return 0;  
} 
//产生ACK应答
void IIC_Ack(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=0;
	delay_us(15);
	IIC_SCL=1;
	delay_us(15);
	IIC_SCL=0;
}
//产生非ACK应答		    
void IIC_NAck(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=1;
	delay_us(15);
	IIC_SCL=1;
	delay_us(15);
	IIC_SCL=0;
}					 				     
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    IIC_SCL=0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
		IIC_SCL=0;
		if((txd&0x80)>>7)
			IIC_SDA=1;
		else
			IIC_SDA=0;
		txd<<=1; 	  
		delay_us(15);   //延时
		IIC_SCL=1;
		delay_us(15); 	
    }
	IIC_SCL=0;	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        IIC_SCL=0; 
        delay_us(50);
		IIC_SCL=1;
		delay_us(50);
        receive<<=1;
        if(READ_SDA)receive++;   
    }
        IIC_SCL=0;						 
    if (!ack)
        IIC_NAck();//发送nACK
    else
        IIC_Ack(); //发送ACK   
    return receive;
}



























