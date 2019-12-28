//****************************************************************//
//DHT12 IIC数据读取程序
// 公司：    奥松电子
//****************************************************************//

#include "DHT12.h"

float Temprature,Humi;//定义温湿度变量 ，此变量为全局变量


/********************************************\
|* 功能： DHT12_IIC初始化函数	        *|
\********************************************/
void sensor_iic_init(void)
{
	IIC_Init();
}


/********************************************\
|* 功能：DHT12读取温湿度函数       *|
\********************************************/
//变量：Humi_H：湿度高位；Humi_L：湿度低位；Temp_H：温度高位；Temp_L：温度低位；Temp_CAL：校验位
//数据格式为：湿度高位（湿度整数）+湿度低位（湿度小数）+温度高位（温度整数）+温度低位（温度小数）+ 校验位
//校验：校验位=湿度高位+湿度低位+温度高位+温度低位
void sensor_read(void)
{	
	u16 i;
	u8 Humi_H,Humi_L,Temp_H,Temp_L,Temp_CAL,temp;

	IIC_Start();    //主机发送起始信号
	IIC_Send_Byte(0Xb8);    //发送IIC地址
	if(!IIC_Wait_Ack())  //等待从机应答信号（如无应答：考虑IIC通讯频率是否太快，或者传感器接线错误）
	{
		i=0;
		IIC_Send_Byte(0);
		while(IIC_Wait_Ack())//等待从机应答信号
		{
		 	if(++i>=500)
			{
			 	break;
			}		 
		} 
		i=0;
		IIC_Start();       //主机发送起始信号
		IIC_Send_Byte(0Xb9);     //发送读指令
		while(IIC_Wait_Ack())    //等待从机应答信号
		{
		 	if(++i>=500)
			{
			 	break;
			}			 
		}
	
		Humi_H=IIC_Read_Byte(1);   //读取湿度高位
		Humi_L=IIC_Read_Byte(1);   //读取湿度低位
		Temp_H=IIC_Read_Byte(1);   //读取温度高位
		Temp_L=IIC_Read_Byte(1);   //读取温度低位
		Temp_CAL=IIC_Read_Byte(0); //读取校验位
		IIC_Stop();	   //发送停止信号	
		temp = (u8)(Humi_H+Humi_L+Temp_H+Temp_L);//只取低8位
		if(Temp_CAL==temp)//如果校验成功，往下运行
		{
			Humi=Humi_H*10+Humi_L; //湿度
	
			if(Temp_L&0X80)	//为负温度
			{
				Temprature =0-(Temp_H*10+((Temp_L&0x7F)));
			}
			else   //为正温度
			{
				Temprature=Temp_H*10+Temp_L;//为正温度
			} 	
			//判断温湿度是否超过测量范围（温度：-20℃~60摄氏度；湿度：20%RH~95%RH）		
			if(Humi>950) 
			{
			  Humi=950;
			}
			if(Humi<200)
			{
				Humi =200;
			}
			if(Temprature>600)
			{
			  Temprature=600;
			}
			if(Temprature<-200)
			{
				Temprature = -200;
			}
			Temprature=Temprature/10;//计算为温度值
			Humi=Humi/10; //计算为湿度值
			printf("\r\n温度为:  %.1f  ℃\r\n",Temprature); //显示温度
			printf("湿度为:  %.1f  %%RH\r\n",Humi);//显示湿度		   
		}
		else //校验失败
		{
			printf("CRC Error !!\r\n");
		}
	}else
	{
	   printf("Sensor Error !!\r\n");
	}	

}



