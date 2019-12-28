#include "bsp_iic.h"

void IIC_Delay(void)
{
    delay_us(1);
}

void IIC_Init(void)
{
    //个人觉得程序初始化就是要通俗易懂，所以习惯用库函数进行
    //但是像改变IIC引脚电平状态，由于需要频繁操作，所以最好还是使用寄存器操作
    GPIO_InitTypeDef GPIO_InitStrcuture;
    
    RCC_APB2PeriphClockCmd(IIC_CLOCK,ENABLE);

    GPIO_InitStrcuture.GPIO_Mode = GPIO_Mode_Out_OD;//开漏输出
    GPIO_InitStrcuture.GPIO_Pin = IIC_SDA_PIN|IIC_SCL_PIN;
    GPIO_InitStrcuture.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(IIC_GPIO,&GPIO_InitStrcuture);
//    printf("SHT30_Init OK！n");

//寄存器操作
//    /*1.开时钟*/
//    RCC->APB2ENR |= 1<<2;

//    /*2.设置GPIO模式*/
//    GPIOA->CRL &= 0x0000FFFF;
//    GPIOA->CRL |= 0x33330000;

//    /*3.设置GPIO空闲电平*/
//    GPIOA->ODR |= 0xF<<4;
}


//发送ACK
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

//发送NACK
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

//等待ACK
u8 IIC_Wait_ACK(void)
{
    u8 t = 200;//连接超时次数
    IIC_OUTPUT_MODE_SET();

    SDA_OUT=1;//8位发送完后释放数据线，准备接收应答位 
    IIC_Delay();
    SCL=0;
    IIC_Delay(); 
    IIC_INPUT_MODE_SET();
    IIC_Delay(); 

    while(SDA_IN)//等待SHT30应答
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

//启动IIC
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

//结束IIC
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

//发送一字节地址/数据
void  IIC_TxByte(u8 byte)
{
    u8  BitCnt;
    IIC_OUTPUT_MODE_SET();

    SCL=0;
    for(BitCnt=0;BitCnt<8;BitCnt++)//要传送的数据长度为8位
    {
        if(byte&0x80) 
            SDA_OUT=1;//判断发送位
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

//接收一字节
u8 IIC_RxByte(void)
{
    u8 ch=0;

    u8 BitCnt;

    IIC_INPUT_MODE_SET();//置数据线为输入方式
    
    IIC_Delay();                    
    for(BitCnt=0;BitCnt<8;BitCnt++)
    {  
        SCL=0;//置时钟线为低，准备接收数据位
        IIC_Delay();               
        SCL=1;//置时钟线为高使数据线上数据有效                
        ch=ch<<1;

        if(SDA_IN) 
            ch |=1;//读数据位,接收的数据位放入retc中 

        IIC_Delay();
    }
    SCL=0;    
    return(ch);
}
