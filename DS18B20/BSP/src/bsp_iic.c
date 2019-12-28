#include "bsp_systick.h"
#include "bsp_sys.h"
#include <stdio.h>
void init_onewire_out( void );
void init_onewire_in( void );
#define DQ_Out PAout(1)
#define DQ_In GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)

//ds18b20��ʼ��
void  init_ds18b20( void )
{
	init_onewire_out();
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);
	delay_us(480);
	init_onewire_in();
	delay_us(60);
	if( !DQ_In)
	{
		delay_us(120);
	}
}
//ds18b20 ���
void  check_ds18b20( void )
{
	init_onewire_out();
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);
	delay_us(240);
	init_onewire_in();
	delay_us(60);
	if( !DQ_In)
	{
		delay_us(80);
		if( !DQ_In )
			printf("��⵽DS18B20��\r\n");
	}
 
}
//����Ϊ���豸д���ߣ����豸������
void init_onewire_out( void )
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//ʹ��GPIOAʱ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��
}
//����Ϊ���豸��ȡ���ߣ����豸д����
void init_onewire_in( void )
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//ʹ��GPIOAʱ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��ͨ����ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��
}
 
void ds18b20_write_byte( u8 data )
{
	u8 i;
	u8 j=0;
	init_onewire_out();
	for(i=0;i<8;i++)
		{
			j=data & 0x01;
			if(j)
			{
				DQ_Out=0;		//д1
				delay_us(15);
				DQ_Out=1;
				delay_us(60);
				
			}
			else
			{
				DQ_Out=0;		//д0
				delay_us(60);
				DQ_Out=1;
				delay_us(1);
			}
			data = data>>1;
		}	
}
//��ȡDS18B20 ��һλ
u8 ds18b20_read_bit( void )
{	
	u8 bit;
	init_onewire_out();
	DQ_Out=0;
	delay_us(2);
	DQ_Out=1;
	init_onewire_in();
	delay_us(12);
	if(DQ_In)
		bit=1;
	else
		bit=0;
	delay_us(50);
	return bit;
}
//��ds18b20���ֽ�
u8 ds18b20_read_byte ( void )
{
	u8 data=0;
	u8 i;
	u8 j=0;
	for(i=0;i<8;i++)
	{
		j=ds18b20_read_bit();
		if(j)			//ע��˳�򼴿ɣ�ds18b20�ȷ��͵�λ��������
			j=j<<i;
		data |=j;
 
	}
	return data;
}
//��ȡds18b20��ϵ�����48λΨһ���к�

void ds18b20_read_rom_number()
{
	u32 number=0;
	u8 data,i,serial_num,ds18b20_crc;
	init_ds18b20();
	ds18b20_write_byte(0x33);
	serial_num = ds18b20_read_byte();
	for(i=0;i<6;i++)
	{
		data = ds18b20_read_byte();
		number |= data;
		number = number<<8;
	}
	ds18b20_crc = ds18b20_read_byte();
	
	printf("ϵ�к��ǣ�%d\r\n",serial_num);
	printf("���к��ǣ�0x%x\r\n",number);
	printf("CRCУ��Ϊ��0x%x\r\n",ds18b20_crc);
	
}
//����ds18b20�¶�ת��

void tem_chage( void )
{
	init_ds18b20();
	ds18b20_write_byte(0xcc);		//����romָ��
	ds18b20_write_byte(0x44);	//����ת��
}
 
short  get_temp( void )
{
	int temp=0;
	u8 TH,TL;
	short tem;
 
	tem_chage();
	delay_us(10);
	init_ds18b20();
	ds18b20_write_byte(0xcc);		//����romָ��
	ds18b20_write_byte(0xbe);	//��ȡ�¶�ת��ֵ
	TL=ds18b20_read_byte();
	TH=ds18b20_read_byte();
	
	if(TH > 7) 		//ͨ���ж��洢���ĸ���λ��0��1���ж��¶ȵ�������
	{
		temp = 0;	//Ϊ��
		TH =~TH;
		TL =~TL; 
	}
	else 
		temp = 1;	//Ϊ��
	tem = TH;
	tem =tem<<8;
	tem =tem+TL;
	tem = (double)tem * 0.625;
	if(temp)
    {        
        printf("temp:%d\r\n",tem);
		return tem;
    }
	else
    {
        printf("temp:%d\r\n",tem);
		return -tem;
    }
	
}
void ds18b20_return_TH_TL_CONF( void )
{
	char data,data_TH,data_TL,CONF;
	init_ds18b20();
	ds18b20_write_byte(0xcc);   //����romָ��
	ds18b20_write_byte(0xbe);	//��ȡ�¶�ת��ֵ
	data = ds18b20_read_byte();
	data = ds18b20_read_byte();
	data_TH = ds18b20_read_byte();
	data_TL = ds18b20_read_byte();
	CONF =ds18b20_read_byte();
	printf("���±������¶�Ϊ��%d��\r\n",data_TH);
	printf("���±������¶�Ϊ��%d��\r\n",-(data_TL-128));
	CONF &=0x60 ;
	CONF =CONF>>5;
	switch (CONF) {
		case 0:
			printf("ds18b20�Ĳ�������Ϊ9λ������Ϊ0.5��\r\n");
			break;
		case 1:
			printf("ds18b20�Ĳ�������Ϊ10λ������Ϊ0.25��\r\n");
			break;
		case 2:
			printf("ds18b20�Ĳ�������Ϊ11λ������Ϊ0.125��\r\n");
			break;
		case 3:
			printf("ds18b20�Ĳ�������Ϊ12λ������Ϊ0.0625��\r\n");
			break;
		default:
			printf("error!!\r\n");
			break;
	}
}
//�����¶ȱ���ֵ�����þ��ȣ�TH���±���ֵ(TH>0)��TL���±���ֵ(TLΪ���� )��mode����ģʽ0,1,2,3
//mode=0 ����Ϊ9λ  00011111    dat=31
//mode=1 ����Ϊ10λ 00111111	dat=63
//mode=2 ����Ϊ11λ	01011111	dat=95
//mode=3 ����Ϊ12λ 01111111	dat =127

void ds18b20_write_TH_TL_CONF(u8 TH,u8 TL,u8 mode)
{
	u8 dat;
	switch (mode){
		case 0:
			dat=31;
			break;
		case 1:
			dat=63;
			break;
		case 2:
			dat=95;
			break;
		case 3:
			dat=127;
			break;
		default:
			printf("mode error!!\r\n");
			dat=127;
			break;
	}
	TL=TL+128;
	init_ds18b20();
	ds18b20_write_byte(0xcc);   //����romָ��
	ds18b20_write_byte(0x4e);	//д���ݴ�Ĵ��� �����º͵��±���ֵ
	ds18b20_write_byte(TH);	    //д��20��Ϊ���±���ֵ
	ds18b20_write_byte(TL);	    //д��-20��Ϊ���±���ֵ
	ds18b20_write_byte(dat);	//д�뾫��
	init_ds18b20();
	ds18b20_write_byte(0xcc);   //����romָ��
	ds18b20_write_byte(0x48);	//��д����ݴ�Ĵ�������EEPROM
}
void ds18b20_self_check( void )
{
	check_ds18b20();
	ds18b20_read_rom_number();
	ds18b20_return_TH_TL_CONF();
}
