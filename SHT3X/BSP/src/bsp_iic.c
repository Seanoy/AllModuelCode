#include "bsp_iic.h"

void IIC_Delay(void)
{
    delay_us(1);
}

void IIC_Init(void)
{
    //���˾��ó����ʼ������Ҫͨ���׶�������ϰ���ÿ⺯������
    //������ı�IIC���ŵ�ƽ״̬��������ҪƵ��������������û���ʹ�üĴ�������
    GPIO_InitTypeDef GPIO_InitStrcuture;
    
    RCC_APB2PeriphClockCmd(IIC_CLOCK,ENABLE);

    GPIO_InitStrcuture.GPIO_Mode = GPIO_Mode_Out_OD;//��©���
    GPIO_InitStrcuture.GPIO_Pin = IIC_SDA_PIN|IIC_SCL_PIN;
    GPIO_InitStrcuture.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(IIC_GPIO,&GPIO_InitStrcuture);
//    printf("SHT30_Init OK��n");

//�Ĵ�������
//    /*1.��ʱ��*/
//    RCC->APB2ENR |= 1<<2;

//    /*2.����GPIOģʽ*/
//    GPIOA->CRL &= 0x0000FFFF;
//    GPIOA->CRL |= 0x33330000;

//    /*3.����GPIO���е�ƽ*/
//    GPIOA->ODR |= 0xF<<4;
}


//����ACK
void IIC_ACK(void)
{
    IIC_OUTPUT_MODE_SET();
    
    SCL=0;
    IIC_Delay(); 
    SDA_OUT=0;
    IIC_Delay(); 
    SCL=1;
    IIC_Delay();                  
    SCL=0;                     
    IIC_Delay();    
}

//����NACK
void IIC_NACK(void)
{
    IIC_OUTPUT_MODE_SET();

    SCL=0;
    IIC_Delay();
    SDA_OUT=1;
    IIC_Delay();
    SCL=1;
    IIC_Delay();
    SCL=0;                     
    IIC_Delay();
}

//�ȴ�ACK
u8 IIC_Wait_ACK(void)
{
    u8 t = 200;//���ӳ�ʱ����
    IIC_OUTPUT_MODE_SET();

    SDA_OUT=1;//8λ��������ͷ������ߣ�׼������Ӧ��λ 
    IIC_Delay();
    SCL=0;
    IIC_Delay(); 
    IIC_INPUT_MODE_SET();
    IIC_Delay(); 

    while(SDA_IN)//�ȴ�SHT30Ӧ��
    {
        t--;
        IIC_Delay(); 
        if(t==0)
        {
            SCL=0;
            return 1;
        }
        IIC_Delay(); 
    }

    IIC_Delay();      
    SCL=1;
    IIC_Delay();
    SCL=0;             
    IIC_Delay();    
    return 0;
}

//����IIC
void IIC_Start(void)
{
    IIC_OUTPUT_MODE_SET();

    SDA_OUT=1;
    SCL=1;
    IIC_Delay();
    SDA_OUT=0;
    IIC_Delay(); 
    SCL=0;
}

//����IIC
void IIC_Stop(void)
{
    IIC_OUTPUT_MODE_SET();
    
    SCL=0;
    SDA_OUT=0;  
    IIC_Delay();
    SCL=1;
    IIC_Delay();
    SDA_OUT=1;
    IIC_Delay();
}

//����һ�ֽڵ�ַ/����
void  IIC_TxByte(u8 byte)
{
    u8  BitCnt;
    IIC_OUTPUT_MODE_SET();

    SCL=0;
    for(BitCnt=0;BitCnt<8;BitCnt++)//Ҫ���͵����ݳ���Ϊ8λ
    {
        if(byte&0x80) 
            SDA_OUT=1;//�жϷ���λ
        else 
            SDA_OUT=0; 

        byte<<=1;
        IIC_Delay(); 
        SCL=1;
        IIC_Delay();
        SCL=0;
        IIC_Delay();
    }
}

//����һ�ֽ�
u8 IIC_RxByte(void)
{
    u8 ch=0;

    u8 BitCnt;

    IIC_INPUT_MODE_SET();//��������Ϊ���뷽ʽ
    
    IIC_Delay();                    
    for(BitCnt=0;BitCnt<8;BitCnt++)
    {  
        SCL=0;//��ʱ����Ϊ�ͣ�׼����������λ
        IIC_Delay();               
        SCL=1;//��ʱ����Ϊ��ʹ��������������Ч                
        ch=ch<<1;

        if(SDA_IN) 
            ch |=1;//������λ,���յ�����λ����retc�� 

        IIC_Delay();
    }
    SCL=0;    
    return(ch);
}
