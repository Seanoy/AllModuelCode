#include "bsp_systick.h"
#include "bsp_sys.h"
#include <stdio.h>
void init_onewire_out( void );
void init_onewire_in( void );
#define DQ_Out PAout(1)
#define DQ_In GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)

//ds18b20初始化
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
//ds18b20 检测
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
			printf("检测到DS18B20！\r\n");
	}
 
}
//设置为主设备写总线，从设备读总线
void init_onewire_out( void )
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//使能GPIOA时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//普通输出模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化
}
//设置为主设备读取总线，从设备写总线
void init_onewire_in( void )
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//使能GPIOA时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//普通输入模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化
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
				DQ_Out=0;		//写1
				delay_us(15);
				DQ_Out=1;
				delay_us(60);
				
			}
			else
			{
				DQ_Out=0;		//写0
				delay_us(60);
				DQ_Out=1;
				delay_us(1);
			}
			data = data>>1;
		}	
}
//读取DS18B20 的一位
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
//读ds18b20的字节
u8 ds18b20_read_byte ( void )
{
	u8 data=0;
	u8 i;
	u8 j=0;
	for(i=0;i<8;i++)
	{
		j=ds18b20_read_bit();
		if(j)			//注意顺序即可，ds18b20先发送地位到总线上
			j=j<<i;
		data |=j;
 
	}
	return data;
}
//获取ds18b20的系列码和48位唯一序列号

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
	
	printf("系列号是：%d\r\n",serial_num);
	printf("序列号是：0x%x\r\n",number);
	printf("CRC校验为：0x%x\r\n",ds18b20_crc);
	
}
//开启ds18b20温度转换

void tem_chage( void )
{
	init_ds18b20();
	ds18b20_write_byte(0xcc);		//忽略rom指令
	ds18b20_write_byte(0x44);	//开启转换
}
 
short  get_temp( void )
{
	int temp=0;
	u8 TH,TL;
	short tem;
 
	tem_chage();
	delay_us(10);
	init_ds18b20();
	ds18b20_write_byte(0xcc);		//忽略rom指令
	ds18b20_write_byte(0xbe);	//读取温度转换值
	TL=ds18b20_read_byte();
	TH=ds18b20_read_byte();
	
	if(TH > 7) 		//通过判读存储器的高五位的0，1来判断温度的正负，
	{
		temp = 0;	//为负
		TH =~TH;
		TL =~TL; 
	}
	else 
		temp = 1;	//为正
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
	ds18b20_write_byte(0xcc);   //忽略rom指令
	ds18b20_write_byte(0xbe);	//读取温度转换值
	data = ds18b20_read_byte();
	data = ds18b20_read_byte();
	data_TH = ds18b20_read_byte();
	data_TL = ds18b20_read_byte();
	CONF =ds18b20_read_byte();
	printf("过温报警的温度为：%d℃\r\n",data_TH);
	printf("低温报警的温度为：%d℃\r\n",-(data_TL-128));
	CONF &=0x60 ;
	CONF =CONF>>5;
	switch (CONF) {
		case 0:
			printf("ds18b20的测量精度为9位，精度为0.5℃\r\n");
			break;
		case 1:
			printf("ds18b20的测量精度为10位，精度为0.25℃\r\n");
			break;
		case 2:
			printf("ds18b20的测量精度为11位，精度为0.125℃\r\n");
			break;
		case 3:
			printf("ds18b20的测量精度为12位，精度为0.0625℃\r\n");
			break;
		default:
			printf("error!!\r\n");
			break;
	}
}
//设置温度报警值和配置精度，TH过温报警值(TH>0)，TL低温报警值(TL为负数 )，mode配置模式0,1,2,3
//mode=0 精度为9位  00011111    dat=31
//mode=1 精度为10位 00111111	dat=63
//mode=2 精度为11位	01011111	dat=95
//mode=3 精度为12位 01111111	dat =127

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
	ds18b20_write_byte(0xcc);   //忽略rom指令
	ds18b20_write_byte(0x4e);	//写入暂存寄存器 ，过温和低温报警值
	ds18b20_write_byte(TH);	    //写入20°为过温报警值
	ds18b20_write_byte(TL);	    //写入-20°为低温报警值
	ds18b20_write_byte(dat);	//写入精度
	init_ds18b20();
	ds18b20_write_byte(0xcc);   //忽略rom指令
	ds18b20_write_byte(0x48);	//将写入的暂存寄存器拷入EEPROM
}
void ds18b20_self_check( void )
{
	check_ds18b20();
	ds18b20_read_rom_number();
	ds18b20_return_TH_TL_CONF();
}
