#include "bsp_spi.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//Mini STM32������
//SPI ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2010/6/13 
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2009-2019
//All rights reserved
////////////////////////////////////////////////////////////////////////////////// 	  
 

//������SPIģ��ĳ�ʼ�����룬���ó�����ģʽ������SD Card/W25X16/24L01/JF24C							  
//SPI�ڳ�ʼ��
//�������Ƕ�SPI1�ĳ�ʼ��
void SPIx_Init(void)
{	 
	RCC->APB2ENR|=1<<2;       //PORTAʱ��ʹ�� 	 
	RCC->APB2ENR|=1<<12;      //SPI1ʱ��ʹ�� 
		   
	//����ֻ���SPI�ڳ�ʼ��
	GPIOA->CRL&=0X000FFFFF; 
	GPIOA->CRL|=0XBBB00000;//PA5.6.7���� 	    
	GPIOA->ODR|=0X7<<5;    //PA5.6.7����
		
	SPI1->CR1|=0<<10;//ȫ˫��ģʽ	
	SPI1->CR1|=1<<9; //���nss����
	SPI1->CR1|=1<<8;  

	SPI1->CR1|=1<<2; //SPI����
	SPI1->CR1|=0<<11;//8bit���ݸ�ʽ	
	//��24L01Ҫ���� CPHA=0;CPOL=0;
	SPI1->CR1|=0<<1; //CPOL=0ʱ����ģʽ��SCKΪ1 
	//SPI1->CR1|=1<<1; //����ģʽ��SCKΪ1 CPOL=1			   
	SPI1->CR1|=0<<0; //��һ��ʱ�ӵ��½���,CPHA=1 CPOL=1	   
	SPI1->CR1|=7<<3; //Fsck=Fcpu/256
	SPI1->CR1|=0<<7; //MSBfirst   
	SPI1->CR1|=1<<6; //SPI�豸ʹ��
	SPIx_ReadWriteByte(0xff);//��������		 
}   
//SPI �ٶ����ú���
//SpeedSet:
//SPI_SPEED_2   2��Ƶ   (SPI 36M@sys 72M)
//SPI_SPEED_8   8��Ƶ   (SPI 9M@sys 72M)
//SPI_SPEED_16  16��Ƶ  (SPI 4.5M@sys 72M)
//SPI_SPEED_256 256��Ƶ (SPI 281.25K@sys 72M)
void SPIx_SetSpeed(u8 SpeedSet)
{
	SPI1->CR1&=0XFFC7;//Fsck=Fcpu/256
	if(SpeedSet==SPI_SPEED_2)//����Ƶ
	{
		SPI1->CR1|=0<<3;//Fsck=Fpclk/2=36Mhz	
	}else if(SpeedSet==SPI_SPEED_8)//�˷�Ƶ 
	{
		SPI1->CR1|=2<<3;//Fsck=Fpclk/8=9Mhz	
	}else if(SpeedSet==SPI_SPEED_16)//ʮ����Ƶ
	{
		SPI1->CR1|=3<<3;//Fsck=Fpclk/16=4.5Mhz
	}else			 	 //256��Ƶ
	{
		SPI1->CR1|=7<<3; //Fsck=Fpclk/256=281.25Khz ����ģʽ
	}
	SPI1->CR1|=1<<6; //SPI�豸ʹ��	  
} 
//SPIx ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
u8 SPIx_ReadWriteByte(u8 TxData)
{		
	u8 retry=0;				 
	while((SPI1->SR&1<<1)==0)//�ȴ���������	
	{
		retry++;
		if(retry>200)return 0;
	}			  
	SPI1->DR=TxData;	 	  //����һ��byte 
	retry=0;
	while((SPI1->SR&1<<0)==0) //�ȴ�������һ��byte  
	{
		retry++;
		if(retry>200)return 0;
	}	  						    
	return SPI1->DR;          //�����յ�������				    
}


//#include "spi.h"

////������SPIģ��ĳ�ʼ�����룬���ó�����ģʽ 						  
////SPI�ڳ�ʼ��
////�������Ƕ�SPI1�ĳ�ʼ��
//void SPI1_Init(void)
//{
//	GPIO_InitTypeDef  GPIO_InitStructure;
//	SPI_InitTypeDef  SPI_InitStructure;
//	
//	/* SPI��IO�ں�SPI�����ʱ�� */
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
//	
//	/* SPI��IO������ */
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);

//	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
//	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//����SPI����ģʽ:����Ϊ��SPI
//	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
//	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
//	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
//	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
//	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
//	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
//	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRCֵ����Ķ���ʽ
//	SPI_Init(SPI1, &SPI_InitStructure);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
//	
//	SPI_Cmd(SPI1, ENABLE); //ʹ��SPI����
//	
//	SPI1_ReadWriteByte(0xff);//��������	
//}

////SPI1�ٶ����ú���
////SPI�ٶ�=fAPB2/��Ƶϵ��
////@ref SPI_BaudRate_Prescaler:SPI_BaudRatePrescaler_2~SPI_BaudRatePrescaler_256  
////fAPB2ʱ��һ��Ϊ84Mhz��
//void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler)
//{
//	SPI1->CR1&=0XFFC7;//λ3-5���㣬�������ò�����
//	SPI1->CR1|=SPI_BaudRatePrescaler;	//����SPI1�ٶ� 
//	SPI_Cmd(SPI1,ENABLE); //ʹ��SPI1
//} 

////SPI1 ��дһ���ֽ�
////TxData:Ҫд����ֽ�
////����ֵ:��ȡ�����ֽ�
//u8 SPI1_ReadWriteByte(u8 TxData)
//{		 			 
// 
//	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);//�ȴ���������  
//	
//	SPI_I2S_SendData(SPI1, TxData); //ͨ������SPIx����һ��byte  ����
//		
//	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET); //�ȴ�������һ��byte  
// 
//	return SPI_I2S_ReceiveData(SPI1); //����ͨ��SPIx������յ�����	
// 		    
//}



////SPI�ڳ�ʼ��
////�������Ƕ�SPI2�ĳ�ʼ��
//void SPI2_Init(void)
//{
//	GPIO_InitTypeDef  GPIO_InitStructure;
//	SPI_InitTypeDef  SPI_InitStructure;
//	
//	/* SPI��IO�ں�SPI�����ʱ�� */
//    RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );
//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
//	
//	/* SPI��IO������ */
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //PB13/14/15����������� 
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);

//	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
//	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//����SPI����ģʽ:����Ϊ��SPI
//	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
//	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
//	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
//	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
//	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
//	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
//	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRCֵ����Ķ���ʽ
//	SPI_Init(SPI2, &SPI_InitStructure);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
//	
//	SPI_Cmd(SPI2, ENABLE); //ʹ��SPI����
//	
//	SPI2_ReadWriteByte(0xff);//��������	
//}

////SPI2�ٶ����ú���
////SPI�ٶ�=fAPB1/��Ƶϵ��
////@ref SPI_BaudRate_Prescaler:SPI_BaudRatePrescaler_2~SPI_BaudRatePrescaler_256  
////fAPB1ʱ��һ��Ϊ36Mhz��
//void SPI2_SetSpeed(u8 SPI_BaudRatePrescaler)
//{
//	SPI2->CR1&=0XFFC7;//λ3-5���㣬�������ò�����
//	SPI2->CR1|=SPI_BaudRatePrescaler;	//����SPI�ٶ� 
//	SPI_Cmd(SPI2,ENABLE); //ʹ��SPI2
//} 

////SPI2 ��дһ���ֽ�
////TxData:Ҫд����ֽ�
////����ֵ:��ȡ�����ֽ�
//u8 SPI2_ReadWriteByte(u8 TxData)
//{		 			 
// 
//	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);//�ȴ���������  
//	
//	SPI_I2S_SendData(SPI2, TxData); //ͨ������SPIx����һ��byte  ����
//		
//	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET); //�ȴ�������һ��byte  
// 
//	return SPI_I2S_ReceiveData(SPI2); //����ͨ��SPIx������յ�����	
// 		    
//}


