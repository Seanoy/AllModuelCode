#include "bsp_max30100.h"

//д���ݵ�MAX30100�Ĵ���
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

//��MAX30100�Ĵ�����ȡ����
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

//MAX30100��FIFO�Ĵ�����ȡ����
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

//MAX30100���üĴ�����ʼ��
void MAX30100_Init(void)
{
	MAX30100_WriteReg(ModeConfiguration,0x0b);  //��λоƬ������ģʽ
	MAX30100_WriteReg(InterruptEnable,0xf0);		//���������ж�
	MAX30100_WriteReg(LEDConfiguration,0x33);		//Red_PA OR IR_PA ���õ���ֵΪ11mA
	MAX30100_WriteReg(SPO2Configuration,0x43);	//SPO2_SR[2:0]=000 -> [��������Ϊ 50Hz]  LED_PW[1:0]=11 -> [����1600��s, ADC�ֱ���Ϊ16Bits]
	
	MAX30100_WriteReg(FIFOWritePointer,0x00); 	// set FIFO write Pointer reg = 0x00 for clear it
	MAX30100_WriteReg(OverFlowCounter,0x00);		// set Over Flow Counter  reg = 0x00 for clear it
	MAX30100_WriteReg(FIFOReadPointer,0x00);		// set FIFO Read Pointer  reg = 0x00 for clear it 
}


//��ȡID�Ĵ�����ֵ
void ReadID(void)
{
	u8 ID,Revision;//�ֱ��ӦӲ��ID���޶�ID
	MAX30100_ReadData(PartID,&ID,1); 
	MAX30100_ReadData(RevisionID,&Revision,1);
	printf("MAX30100 ID = %d\r\n",ID);  
	printf("MAX30100 RevisionID = %d\r\n",Revision);
}

//��ȡ�¶ȼĴ���
void GetTemp(void)
{
	float temp;
	u8 tempInteger,tempFraction;  						 //�ֱ��Ӧ�¶��������ֺ�С������
	MAX30100_WriteReg(ModeConfiguration,0x0b); //��λоƬ������ģʽ 
	MAX30100_ReadData(TempInteger,&tempInteger,1);
	delay_ms(100);	
	printf("tempInteger = %d\r\n",tempInteger);                    
	MAX30100_ReadData(TempFraction,&tempFraction,1);            
	printf("tempFraction = %d\r\n",tempFraction);                   
	temp=tempInteger+(tempFraction*0.0625);    //����С�����֣�����Ϊ0.0625
	printf("��ǰ�¶� = %.4f\r\n",temp);              
}

void GetHeartRateNSPO2(void)
{
	u8 status,data[4];
	u16 ir_value,red_value;
	MAX30100_WriteReg(InterruptEnable,0xf0);		//���������ж�
//	MAX30100_ReadData(InterruptStatus,&status,1);
 //������Լ��жϱ�־λУ��if(...)
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
	GetHeartRateNSPO2();	//��ȡ���ʺ�Ѫ��rawֵ,�ɶ�rawֵ���д���Ӷ��õ�����(����thresholdֵȻ�����etc.)
	delay_ms(100);
  GetTemp();						//��ȡ�¶�
}
