#include "sht30.h"

float_data humidity;//存储的湿度数据
float_data temperature;//存储的温度数据
u8 status[3];//状态寄存器
u8 status_crc;//CRC校验计算
u8 crc_check;//crc结果

//如果读取不到数据，有可能是读取的时间设得太短导致，我这里的时间是最小可实现读取的时间！！！

//CRC校验
static u8 CRC_Check(u8 *check_data, u8 num, u8 check_crc)
{
    uint8_t bit;        // bit mask
    uint8_t crc = 0xFF; // calculated checksum
    uint8_t byteCtr;    // byte counter
    
 // calculates 8-Bit checksum with given polynomial x8+x5+x4+1
    for(byteCtr = 0; byteCtr < num; byteCtr++)
    {
        crc ^= (*(check_data+byteCtr));
        //crc校验，最高位是1就^0x31
        for(bit = 8; bit > 0; --bit)
        {
            if(crc & 0x80)
                crc = (crc << 1) ^ 0x31;
            else
                crc = (crc << 1);
        }
    }
    if(crc==check_crc)
        return 1;
    else 
        return 0;
}

//根据所选获取数据的方式初始化
void SHT30_Init(void)
{
    i2c_GPIO_Config();
    SHT30_General_RESET();//软复位设备
    delay_ms(10);
}

//发送两字节指令，stop确定是否发送停止信号
void SHT30_SendBytes(u16 cmd,u8 stop)
{
    i2c_Start();
    i2c_SendByte(SHT30_ADDR<<1 | I2C_WR);//写7位I2C设备地址加0作为写取位,1为读取位
    i2c_WaitAck();
    i2c_SendByte(cmd>>8);
    i2c_WaitAck();
    i2c_SendByte(cmd&0xFF);
    i2c_WaitAck();
    if(stop)
        i2c_Stop();
}

//软复位
void SHT30_SOFTRESET(void)
{
    SHT30_SendBytes(0x30A2,1);
}

//通用复位
void SHT30_General_RESET(void)
{
    i2c_Start();
    i2c_SendByte(0x00);
    i2c_WaitAck();
    i2c_SendByte(0x06);
    i2c_WaitAck();
}

//获取状态寄存器数据
void SHT30_Get_Status(u8 *buffer)
{
    SHT30_SendBytes(0xF32D,0);
    delay_ms(3);
    i2c_Start();
    i2c_SendByte(SHT30_ADDR<<1 | I2C_RD);//写7位I2C设备地址加0作为写取位,1为读取位
    if(i2c_WaitAck()==0)
    {
        buffer[0]=i2c_ReadByte(1);
        buffer[1]=i2c_ReadByte(1);
        buffer[2]=i2c_ReadByte(1);
        i2c_Stop();
    }
}

//清空状态寄存器
void SHT30_Clear_Status(void)
{
    SHT30_SendBytes(0x3041,1);
}

//返回0代表状态寄存器读取成功，1代表读取错误
u8 SHT30_Status(void)
{
    SHT30_Get_Status(status);
    
    if(CRC_Check(status,2,*(status+2)))
        return 0;//正确
    else
        return 1;//错误
}

//单次读取数据
void SHT30_Single_Shot(u8 *buffer)
{
    u8 try_time=100;
    
    SHT30_SendBytes(0x2C06,1);
    
    delay_ms(20);//很重要！不然读不出数据
    i2c_Start();
    i2c_SendByte(SHT30_ADDR<<1 | I2C_RD);//写7位I2C设备地址加0作为写取位,1为读取位

    while(i2c_WaitAck())
    {
        try_time--;
        delay_us(50);
        if(try_time==0)
            return;
    }
    buffer[0]=i2c_ReadByte(1);
    buffer[1]=i2c_ReadByte(1);
    buffer[2]=i2c_ReadByte(1);
    buffer[3]=i2c_ReadByte(1);
    buffer[4]=i2c_ReadByte(1);
    buffer[5]=i2c_ReadByte(0);
    i2c_Stop();
}

//开启/关闭加热器
void SHT30_Heater(u8 enable)
{
    if(enable)
        SHT30_SendBytes(0x306D,1);
    else
        SHT30_SendBytes(0x3066,1);
}

//加速响应指令
void SHT30_ART(void)
{
    SHT30_SendBytes(0x2B32,0);
}

//配置周期读取重复性和采样速率
void SHT30_Periodic_SetRepeat(void)
{
    SHT30_SendBytes(0x2737,0);//高重复度，mps为10
}

//配置周期读取总配置
void SHT30_Periodic_Config(void)
{
    //配置
    SHT30_Periodic_SetRepeat();
    SHT30_ART();
}
//周期读取数据 如果使用就要在初始化时加上SHT30_Periodic_Config()函数
void SHT30_Periodic(u8 *buffer)
{
    u8 try_time=100;
    
    //获取数据
    SHT30_SendBytes(0xE000,0);
    delay_ms(3);//很重要！不然读不出数据

    i2c_Start();
    i2c_SendByte(SHT30_ADDR<<1 | I2C_RD);//写7位I2C设备地址加0作为写取位,1为读取位
    
    while(i2c_WaitAck())
    {
        try_time--;
        delay_us(50);
        if(try_time==0)
            return;
    }
    buffer[0]=i2c_ReadByte(1);
    buffer[1]=i2c_ReadByte(1);
    buffer[2]=i2c_ReadByte(1);
    buffer[3]=i2c_ReadByte(1);
    buffer[4]=i2c_ReadByte(1);
    buffer[5]=i2c_ReadByte(0);
    i2c_Stop();
}

//中断指令/停止周期获取数据功能
void SHT30_Stop_Break(void)
{
    SHT30_SendBytes(0x3093,1);
    delay_ms(15);
}

//获取SHT3x温湿度
void SHT30_Read(void)
{    
    u8 buff[6];//获取raw数据
    u16 tem,hum;//拼接温湿度数据
    u8 crcT,crcH;//温度和湿度的CRC校验

    float Temperature=0;//转换后的温湿度
    float Humidity=0;

    SHT30_Single_Shot(buff);
    //SHT30_Periodic(buff);
    
    tem = ((buff[0]<<8) | buff[1]);//温度拼接
    hum = ((buff[3]<<8) | buff[4]);//湿度拼接

    //计算温度和湿度CRC校验码
    crcT = CRC_Check(buff,2,buff[2]);   //温度
    crcH = CRC_Check(buff+3,2,buff[5]); //湿度
    
    if(crcT&&crcH)//判断CRC校验是否对
    {
        //根据手册计算公式计算
        Temperature= (175.0*(float)tem/65535.0-45.0) ;  // T = -45 + 175 * tem / (2^16-1)
        Humidity= (100.0*(float)hum/65535.0);           // RH = hum*100 / (2^16-1)
        if((Temperature>=-20)&&(Temperature<=125)&&(Humidity>=0)&&(Humidity<=100))//过滤超出测量范围的错误数据
        {
            temperature.fdata=Temperature;
            humidity.fdata=Humidity;
        }
    }
}
