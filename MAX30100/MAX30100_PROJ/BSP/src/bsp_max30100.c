#include "bsp_max30100.h"

//写数据到MAX30100寄存器
void MAX30100_WriteReg(u8 reg_add,u8 reg_dat)
{
	i2c_Start();
	i2c_SendByte(MAX30100_SLAVE_ADDRESS);
	i2c_WaitAck();
	i2c_SendByte(reg_add);
	i2c_WaitAck();
	i2c_SendByte(reg_dat);
	i2c_WaitAck();
	i2c_Stop();
}

//从MAX30100寄存器读取数据
void MAX30100_ReadData(u8 reg_add,u8 *Read,u8 num)
{
	unsigned char i;
	
	i2c_Start();
	i2c_SendByte(MAX30100_SLAVE_ADDRESS);
	i2c_WaitAck();
	i2c_SendByte(reg_add);
	i2c_WaitAck();
	
	i2c_Start();
	i2c_SendByte(MAX30100_SLAVE_ADDRESS+1);
	i2c_WaitAck();
	
	for(i=0;i<(num-1);i++){
		*Read=i2c_ReadByte(1);
		Read++;
	}
	*Read=i2c_ReadByte(0);
	i2c_Stop();
}

//MAX30100的FIFO寄存器读取数据
void MAX30100_FIFO_ReadData(u8 *Read)
{
	i2c_Start();
	i2c_SendByte(MAX30100_SLAVE_ADDRESS);
	i2c_WaitAck();
	i2c_SendByte(FIFODataRegister);
	i2c_WaitAck();
	i2c_Start();
	i2c_SendByte(MAX30100_SLAVE_ADDRESS+1);
	i2c_WaitAck();
	
	Read[0]=i2c_ReadByte(1);
	Read[1]=i2c_ReadByte(1);	
	Read[2]=i2c_ReadByte(1);
	Read[3]=i2c_ReadByte(0);
	i2c_Stop();
}

//MAX30100配置寄存器初始化
void MAX30100_Init(void)
{
	MAX30100_WriteReg(ModeConfiguration,0x0b);  //复位芯片，设置模式
	MAX30100_WriteReg(InterruptEnable,0xf0);		//开启所有中断
	MAX30100_WriteReg(LEDConfiguration,0x33);		//Red_PA OR IR_PA 设置电流值为11mA
	MAX30100_WriteReg(SPO2Configuration,0x43);	//SPO2_SR[2:0]=000 -> [采样周期为 50Hz]  LED_PW[1:0]=11 -> [脉宽1600μs, ADC分辨率为16Bits]
	
	MAX30100_WriteReg(FIFOWritePointer,0x00); 	// set FIFO write Pointer reg = 0x00 for clear it
	MAX30100_WriteReg(OverFlowCounter,0x00);		// set Over Flow Counter  reg = 0x00 for clear it
	MAX30100_WriteReg(FIFOReadPointer,0x00);		// set FIFO Read Pointer  reg = 0x00 for clear it 
}


//读取ID寄存器数值
void ReadID(void)
{
	u8 ID,Revision;//分别对应硬件ID和修订ID
	MAX30100_ReadData(PartID,&ID,1); 
	MAX30100_ReadData(RevisionID,&Revision,1);
	printf("MAX30100 ID = %d\r\n",ID);  
	printf("MAX30100 RevisionID = %d\r\n",Revision);
}

//读取温度寄存器
void GetTemp(void)
{
	float temp;
	u8 tempInteger,tempFraction;  						 //分别对应温度整数部分和小数部分
	MAX30100_WriteReg(ModeConfiguration,0x0b); //复位芯片，设置模式 
	MAX30100_ReadData(TempInteger,&tempInteger,1);
	delay_ms(100);	
	printf("tempInteger = %d\r\n",tempInteger);                    
	MAX30100_ReadData(TempFraction,&tempFraction,1);            
	printf("tempFraction = %d\r\n",tempFraction);                   
	temp=tempInteger+(tempFraction*0.0625);    //计算小数部分，步距为0.0625
	printf("当前温度 = %.4f\r\n",temp);              
}

void GetHeartRateNSPO2(void)
{
	u8 status,data[4];
	u16 ir_value,red_value;
	MAX30100_WriteReg(InterruptEnable,0xf0);		//开启所有中断
//	MAX30100_ReadData(InterruptStatus,&status,1);
 //这里可以加中断标志位校验if(...)
	MAX30100_FIFO_ReadData(data);
	MAX30100_WriteReg(FIFOWritePointer,0x00); 	// set FIFO write Pointer reg = 0x00 for clear it
	MAX30100_WriteReg(OverFlowCounter,0x00);		// set Over Flow Counter  reg = 0x00 for clear it
	MAX30100_WriteReg(FIFOReadPointer,0x00);		// set FIFO Read Pointer  reg = 0x00 for clear it
	
	ir_value = (data[0]<<8) + data[1];
	red_value = (data[2]<<8) + data[3];
	printf("ir_value:%d, red_value:%d\r\n",ir_value,red_value);
}

void test(void)
{
	GetHeartRateNSPO2();	//获取心率和血氧raw值,可对raw值进行处理从而得到心率(设置threshold值然后计数etc.)
	delay_ms(100);
  GetTemp();						//获取温度
}
