#include "sht30.h"

float_data humidity;//�洢��ʪ������
float_data temperature;//�洢���¶�����
u8 status[3];//״̬�Ĵ���
u8 status_crc;//CRCУ�����
u8 crc_check;//crc���

//�����ȡ�������ݣ��п����Ƕ�ȡ��ʱ�����̫�̵��£��������ʱ������С��ʵ�ֶ�ȡ��ʱ�䣡����

//CRCУ��
static u8 CRC_Check(u8 *check_data, u8 num, u8 check_crc)
{
    uint8_t bit;        // bit mask
    uint8_t crc = 0xFF; // calculated checksum
    uint8_t byteCtr;    // byte counter
    
 // calculates 8-Bit checksum with given polynomial x8+x5+x4+1
    for(byteCtr = 0; byteCtr < num; byteCtr++)
    {
        crc ^= (*(check_data+byteCtr));
        //crcУ�飬���λ��1��^0x31
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

//������ѡ��ȡ���ݵķ�ʽ��ʼ��
void SHT30_Init(void)
{
    i2c_GPIO_Config();
    SHT30_General_RESET();//��λ�豸
    delay_ms(10);
}

//�������ֽ�ָ�stopȷ���Ƿ���ֹͣ�ź�
void SHT30_SendBytes(u16 cmd,u8 stop)
{
    i2c_Start();
    i2c_SendByte(SHT30_ADDR<<1 | I2C_WR);//д7λI2C�豸��ַ��0��Ϊдȡλ,1Ϊ��ȡλ
    i2c_WaitAck();
    i2c_SendByte(cmd>>8);
    i2c_WaitAck();
    i2c_SendByte(cmd&0xFF);
    i2c_WaitAck();
    if(stop)
        i2c_Stop();
}

//��λ
void SHT30_SOFTRESET(void)
{
    SHT30_SendBytes(0x30A2,1);
}

//ͨ�ø�λ
void SHT30_General_RESET(void)
{
    i2c_Start();
    i2c_SendByte(0x00);
    i2c_WaitAck();
    i2c_SendByte(0x06);
    i2c_WaitAck();
}

//��ȡ״̬�Ĵ�������
void SHT30_Get_Status(u8 *buffer)
{
    SHT30_SendBytes(0xF32D,0);
    delay_ms(3);
    i2c_Start();
    i2c_SendByte(SHT30_ADDR<<1 | I2C_RD);//д7λI2C�豸��ַ��0��Ϊдȡλ,1Ϊ��ȡλ
    if(i2c_WaitAck()==0)
    {
        buffer[0]=i2c_ReadByte(1);
        buffer[1]=i2c_ReadByte(1);
        buffer[2]=i2c_ReadByte(1);
        i2c_Stop();
    }
}

//���״̬�Ĵ���
void SHT30_Clear_Status(void)
{
    SHT30_SendBytes(0x3041,1);
}

//����0����״̬�Ĵ�����ȡ�ɹ���1�����ȡ����
u8 SHT30_Status(void)
{
    SHT30_Get_Status(status);
    
    if(CRC_Check(status,2,*(status+2)))
        return 0;//��ȷ
    else
        return 1;//����
}

//���ζ�ȡ����
void SHT30_Single_Shot(u8 *buffer)
{
    u8 try_time=100;
    
    SHT30_SendBytes(0x2C06,1);
    
    delay_ms(20);//����Ҫ����Ȼ����������
    i2c_Start();
    i2c_SendByte(SHT30_ADDR<<1 | I2C_RD);//д7λI2C�豸��ַ��0��Ϊдȡλ,1Ϊ��ȡλ

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

//����/�رռ�����
void SHT30_Heater(u8 enable)
{
    if(enable)
        SHT30_SendBytes(0x306D,1);
    else
        SHT30_SendBytes(0x3066,1);
}

//������Ӧָ��
void SHT30_ART(void)
{
    SHT30_SendBytes(0x2B32,0);
}

//�������ڶ�ȡ�ظ��ԺͲ�������
void SHT30_Periodic_SetRepeat(void)
{
    SHT30_SendBytes(0x2737,0);//���ظ��ȣ�mpsΪ10
}

//�������ڶ�ȡ������
void SHT30_Periodic_Config(void)
{
    //����
    SHT30_Periodic_SetRepeat();
    SHT30_ART();
}
//���ڶ�ȡ���� ���ʹ�þ�Ҫ�ڳ�ʼ��ʱ����SHT30_Periodic_Config()����
void SHT30_Periodic(u8 *buffer)
{
    u8 try_time=100;
    
    //��ȡ����
    SHT30_SendBytes(0xE000,0);
    delay_ms(3);//����Ҫ����Ȼ����������

    i2c_Start();
    i2c_SendByte(SHT30_ADDR<<1 | I2C_RD);//д7λI2C�豸��ַ��0��Ϊдȡλ,1Ϊ��ȡλ
    
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

//�ж�ָ��/ֹͣ���ڻ�ȡ���ݹ���
void SHT30_Stop_Break(void)
{
    SHT30_SendBytes(0x3093,1);
    delay_ms(15);
}

//��ȡSHT3x��ʪ��
void SHT30_Read(void)
{    
    u8 buff[6];//��ȡraw����
    u16 tem,hum;//ƴ����ʪ������
    u8 crcT,crcH;//�¶Ⱥ�ʪ�ȵ�CRCУ��

    float Temperature=0;//ת�������ʪ��
    float Humidity=0;

    SHT30_Single_Shot(buff);
    //SHT30_Periodic(buff);
    
    tem = ((buff[0]<<8) | buff[1]);//�¶�ƴ��
    hum = ((buff[3]<<8) | buff[4]);//ʪ��ƴ��

    //�����¶Ⱥ�ʪ��CRCУ����
    crcT = CRC_Check(buff,2,buff[2]);   //�¶�
    crcH = CRC_Check(buff+3,2,buff[5]); //ʪ��
    
    if(crcT&&crcH)//�ж�CRCУ���Ƿ��
    {
        //�����ֲ���㹫ʽ����
        Temperature= (175.0*(float)tem/65535.0-45.0) ;  // T = -45 + 175 * tem / (2^16-1)
        Humidity= (100.0*(float)hum/65535.0);           // RH = hum*100 / (2^16-1)
        if((Temperature>=-20)&&(Temperature<=125)&&(Humidity>=0)&&(Humidity<=100))//���˳���������Χ�Ĵ�������
        {
            temperature.fdata=Temperature;
            humidity.fdata=Humidity;
        }
    }
}
