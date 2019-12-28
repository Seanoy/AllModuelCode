//****************************************************************//
//DHT12 IIC���ݶ�ȡ����
// ��˾��    ���ɵ���
//****************************************************************//

#include "DHT12.h"

float Temprature,Humi;//������ʪ�ȱ��� ���˱���Ϊȫ�ֱ���


/********************************************\
|* ���ܣ� DHT12_IIC��ʼ������	        *|
\********************************************/
void sensor_iic_init(void)
{
	IIC_Init();
}


/********************************************\
|* ���ܣ�DHT12��ȡ��ʪ�Ⱥ���       *|
\********************************************/
//������Humi_H��ʪ�ȸ�λ��Humi_L��ʪ�ȵ�λ��Temp_H���¶ȸ�λ��Temp_L���¶ȵ�λ��Temp_CAL��У��λ
//���ݸ�ʽΪ��ʪ�ȸ�λ��ʪ��������+ʪ�ȵ�λ��ʪ��С����+�¶ȸ�λ���¶�������+�¶ȵ�λ���¶�С����+ У��λ
//У�飺У��λ=ʪ�ȸ�λ+ʪ�ȵ�λ+�¶ȸ�λ+�¶ȵ�λ
void sensor_read(void)
{	
	u16 i;
	u8 Humi_H,Humi_L,Temp_H,Temp_L,Temp_CAL,temp;

	IIC_Start();    //����������ʼ�ź�
	IIC_Send_Byte(0Xb8);    //����IIC��ַ
	if(!IIC_Wait_Ack())  //�ȴ��ӻ�Ӧ���źţ�����Ӧ�𣺿���IICͨѶƵ���Ƿ�̫�죬���ߴ��������ߴ���
	{
		i=0;
		IIC_Send_Byte(0);
		while(IIC_Wait_Ack())//�ȴ��ӻ�Ӧ���ź�
		{
		 	if(++i>=500)
			{
			 	break;
			}		 
		} 
		i=0;
		IIC_Start();       //����������ʼ�ź�
		IIC_Send_Byte(0Xb9);     //���Ͷ�ָ��
		while(IIC_Wait_Ack())    //�ȴ��ӻ�Ӧ���ź�
		{
		 	if(++i>=500)
			{
			 	break;
			}			 
		}
	
		Humi_H=IIC_Read_Byte(1);   //��ȡʪ�ȸ�λ
		Humi_L=IIC_Read_Byte(1);   //��ȡʪ�ȵ�λ
		Temp_H=IIC_Read_Byte(1);   //��ȡ�¶ȸ�λ
		Temp_L=IIC_Read_Byte(1);   //��ȡ�¶ȵ�λ
		Temp_CAL=IIC_Read_Byte(0); //��ȡУ��λ
		IIC_Stop();	   //����ֹͣ�ź�	
		temp = (u8)(Humi_H+Humi_L+Temp_H+Temp_L);//ֻȡ��8λ
		if(Temp_CAL==temp)//���У��ɹ�����������
		{
			Humi=Humi_H*10+Humi_L; //ʪ��
	
			if(Temp_L&0X80)	//Ϊ���¶�
			{
				Temprature =0-(Temp_H*10+((Temp_L&0x7F)));
			}
			else   //Ϊ���¶�
			{
				Temprature=Temp_H*10+Temp_L;//Ϊ���¶�
			} 	
			//�ж���ʪ���Ƿ񳬹�������Χ���¶ȣ�-20��~60���϶ȣ�ʪ�ȣ�20%RH~95%RH��		
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
			Temprature=Temprature/10;//����Ϊ�¶�ֵ
			Humi=Humi/10; //����Ϊʪ��ֵ
			printf("\r\n�¶�Ϊ:  %.1f  ��\r\n",Temprature); //��ʾ�¶�
			printf("ʪ��Ϊ:  %.1f  %%RH\r\n",Humi);//��ʾʪ��		   
		}
		else //У��ʧ��
		{
			printf("CRC Error !!\r\n");
		}
	}else
	{
	   printf("Sensor Error !!\r\n");
	}	

}



