/**
  ******************************************************************************
  * @file    sys_config.c
  * @author  zjj
  * @version V1.0
  * @date    2014-10-10
  * @brief   ϵͳ����
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:stm32f103��Ƭ��
  * ��̳    :
  * �Ա�    :
  *
  ******************************************************************************
  */
  
#include "sys_config.h"


void sys_Configuration(void)
{
    /* System Clocks Configuration */
  	RCC_Configuration();
    
    /* ����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ� */
    NVIC_Configuration();
    
    /* USART1 ����ģʽΪ 115200 8-N-1���жϽ��� */
	  USART1_Config();
//    USART2_Config();
    
    /*SysTick ����*/
//    SysTick_Init(); //10us�ж�
    
    //����JTAG����
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);    //����AFIOʱ��
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);      /*ʹ��SWD ����JTAG*/
    
    /* GPIO ���� */
    GPIO_Configuration();
}


//ϵͳʱ������
void RCC_Configuration(void)
{

    ErrorStatus HSEStartUpStatus;
    
    /* ������������RCC�����ã����ƼĴ�����λ */
    RCC_DeInit();
    
    /* ʹ���ⲿ���پ��� */
    RCC_HSEConfig(RCC_HSE_ON);
    
    /* �ȴ����پ����ȶ� */
    HSEStartUpStatus = RCC_WaitForHSEStartUp();
    
    if (HSEStartUpStatus == SUCCESS)
    {
        /* ʹ��flashԤ��ȡ������ */
        FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
        
        /* ��Flash���ڵȴ�״̬��2����Ը�Ƶʱ�ӵģ��������RCCûֱ�ӹ�ϵ�����������Թ� */
        FLASH_SetLatency(FLASH_Latency_2);
        
        /* HCLK = SYSCLK ���ø�������AHB��ʱ��Ϊϵͳʱ��*/
        RCC_HCLKConfig(RCC_SYSCLK_Div1);                   
        
        /* PCLK2 = HCLK ���õ�������2��APB2��ʱ��Ϊ��������ʱ��  ע�����ֵ���Ϊ72MHz*/ 
        RCC_PCLK2Config(RCC_HCLK_Div1);
        
        /* PCLK1 = HCLK/2 ���õ�������1��APB1����ʱ��Ϊ����ʱ�ӵĶ���Ƶ   ע�����ֵ���Ϊ36MHz*/
        RCC_PCLK1Config(RCC_HCLK_Div2);
        
        /* ADCCLK = PCLK2/6 ����ADC����ʱ��=��������2ʱ�ӵ�����Ƶ   ע�����ֵ���Ϊ14MHz*/
//        RCC_ADCCLKConfig(RCC_PCLK2_Div6);				  //����ǲ���Ҫ����ʹ�ܵ� ���������ƵĻ��м���������μ�����
        
        /* Set PLL clock output to 72MHz using HSE (8MHz) as entry clock */ 
        
        /* �������໷���ⲿ8Mhz����9��Ƶ��72Mhz */ 
        RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9); 
        /* Enable PLL ʹ�����໷ �Բ���SYSCLK��Ҳ������PLL*/
        RCC_PLLCmd(ENABLE); 
        
        /* Wait till PLL is ready �ȴ����໷����ȶ�*/
        while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
        {}
        
        /* Select PLL as system clock source �����໷�������Ϊϵͳʱ�� ��ʱ����������ϵͳʱ��*/
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
        
        /* Wait till PLL is used as system clock source �ȴ�У��ɹ�*/
        while (RCC_GetSYSCLKSource() != 0x08)
        {}
        
        /* Enable FSMC, GPIOD, GPIOE, GPIOF, GPIOG and AFIO clocks */
        //ʹ����Χ�ӿ�����ʱ�ӣ�ע�������������������ͬоƬ�ķ��䲻ͬ����ʱ����ֲ�Ϳ���
        //RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
        
        //RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE |
        //                         RCC_APB2Periph_GPIOF | RCC_APB2Periph_GPIOG |
        //                         RCC_APB2Periph_AFIO, ENABLE);
        //RCC_APB1PeriphClockCmd
    }
}



/******************NVIC_Configuration*****************************/
void NVIC_Configuration(void)
{ 
	/* Configure the NVIC Preemption Priority Bits */  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);   //��ռ���ȼ�2����Ӧ���ȼ�2    
}


/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : Configures the different GPIO ports. CAN TRX config�� Test Pin��
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GPIO_Configuration(void)
{
  	GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_LED_GPIOx, ENABLE);   //GPIOʱ��ʹ��
    
    /* Configure PB0 as normal gpio push-pull */
    GPIO_InitStructure.GPIO_Pin = LED0_PINx;					//�ܽ�ѡ��LED_PINxʹ���˺궨��
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	    //�ܽ�����ѡ��
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		//�ܽŹ�������
    GPIO_Init(LED0_GPIOx, &GPIO_InitStructure);				//�˿�ѡ����PA��PB��
    
    GPIO_ResetBits(LED0_GPIOx, LED0_PINx);                    //�˿�״̬��λ

}



void GPIO_PinReverse(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
  /* Check the parameters */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  assert_param(IS_GPIO_PIN(GPIO_Pin));
  
  GPIOx->ODR ^=  GPIO_Pin;
}


/*********************�����ʱ*******************/
//�����ʱ������us����
void Soft_delay_us(u16 time)
{    
   u32 i;  
   while(time--)
   {
      i=8;  //�Լ�����  ����ʱ�䣺20141116
      while(i--){};
        __nop();
        __nop();
        __nop();
        __nop();
   }
}

//�����ʱ������ms����
void Soft_delay_ms(u16 time)
{    
   u16 i=0;  
   while(time--)
   {
      i=7950;  //�Լ�����  ����ʱ�䣺20141116
      while(i--) ;    
   }
}


