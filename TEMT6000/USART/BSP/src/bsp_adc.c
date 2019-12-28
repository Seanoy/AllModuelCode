#include "bsp_adc.h"

float AD_value;
vu16 ADC_ConvertedValue;

#define ADC1_DR_Address    ((u32)0x4001244C)

void ADC_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
	
	/* Configure PC4 (ADC Channel14) as analog input -------------------------*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//ģ������
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
  /* DMA channel1 configuration ----------------------------------------------*/
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;				//�����ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC_ConvertedValue;	//DMAͨ���洢��ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;								//DMA_data_transfer_direction
	DMA_InitStructure.DMA_BufferSize = 1;															//��������С
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;					//��ַ�Ĵ����Ƿ�����
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//�����ַ���� ���ֽ�4λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//Specifies the Memory data width.
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;										//ָ������ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;								//Specifies the software priority for the DMAy Channelx
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;											//Specifies if the DMAy Channelx will be used in memory-to-memory transfer
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	
  /* Enable DMA1 channel1 */
	DMA_Cmd(DMA1_Channel1, ENABLE);
	
  /* ADC1 configuration ------------------------------------------------------*/
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	/* ����ģʽ */
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;				/* ������ͨ��ģʽ */
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	/* ����ת�� */
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;/* ת������������ */
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;		/* �Ҷ��� */
	ADC_InitStructure.ADC_NbrOfChannel = 1;							/* ɨ��ͨ���� */
	ADC_Init(ADC1, &ADC_InitStructure);
	
	/* ADC1 regular channel14 configuration */ 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 1, ADC_SampleTime_239Cycles5);
  //����ʱ��Խ������Խ׼ȷ
	
  /* Enable ADC1 DMA */	
	ADC_DMACmd(ADC1, ENABLE);
	
	/* Enable ADC1 */    
	ADC_Cmd(ADC1, ENABLE);												

	/* Enable ADC1 reset calibration register */ 
	ADC_ResetCalibration(ADC1);
	/* Check the end of ADC1 reset calibration register */
  while(ADC_GetResetCalibrationStatus(ADC1));
	
  /* Start ADC1 calibaration */
  ADC_StartCalibration(ADC1);
  /* Check the end of ADC1 calibration */
  while(ADC_GetCalibrationStatus(ADC1));
     
		 
  /* Start ADC1 Software Conversion */ 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);				
}

float get_val(void)
{
//	if(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)==SET)
//	  {
//			AD_value=ADC_GetConversionValue(ADC1);
//			/* Printf message with AD value to serial port every 1 second */
//			printf("The current AD value = %4.2fV \r\n", (AD_value/4096)*3.3);
//	  }
	      /* Printf message with AD value to serial port every 1 second */
	  AD_value = ADC_ConvertedValue;
	  AD_value = (AD_value/4096)*3.3;
    printf("The current AD value = %f \r\n", AD_value); 
}






