/**
  ******************************************************************************
  * @file    sys_config.c
  * @author  zjj
  * @version V1.0
  * @date    2014-10-10
  * @brief   系统配置
  ******************************************************************************
  * @attention
  *
  * 实验平台:stm32f103单片机
  * 论坛    :
  * 淘宝    :
  *
  ******************************************************************************
  */
  
#include "sys_config.h"


void sys_Configuration(void)
{
    /* System Clocks Configuration */
  	RCC_Configuration();
    
    /* 设置NVIC中断分组2:2位抢占优先级，2位响应优先级 */
    NVIC_Configuration();
    
    /* USART1 配置模式为 115200 8-N-1，中断接收 */
	  USART1_Config();
//    USART2_Config();
    
    /*SysTick 配置*/
//    SysTick_Init(); //10us中断
    
    //禁用JTAG功能
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);    //开启AFIO时钟
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);      /*使能SWD 禁用JTAG*/
    
    /* GPIO 配置 */
    GPIO_Configuration();
}


//系统时钟配置
void RCC_Configuration(void)
{

    ErrorStatus HSEStartUpStatus;
    
    /* 这里是重置了RCC的设置，类似寄存器复位 */
    RCC_DeInit();
    
    /* 使能外部高速晶振 */
    RCC_HSEConfig(RCC_HSE_ON);
    
    /* 等待高速晶振稳定 */
    HSEStartUpStatus = RCC_WaitForHSEStartUp();
    
    if (HSEStartUpStatus == SUCCESS)
    {
        /* 使能flash预读取缓冲区 */
        FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
        
        /* 令Flash处于等待状态，2是针对高频时钟的，这两句跟RCC没直接关系，可以暂且略过 */
        FLASH_SetLatency(FLASH_Latency_2);
        
        /* HCLK = SYSCLK 设置高速总线AHB的时钟为系统时钟*/
        RCC_HCLKConfig(RCC_SYSCLK_Div1);                   
        
        /* PCLK2 = HCLK 设置低速总线2（APB2）时钟为高速总线时钟  注意这个值最大为72MHz*/ 
        RCC_PCLK2Config(RCC_HCLK_Div1);
        
        /* PCLK1 = HCLK/2 设置低速总线1（APB1）的时钟为高速时钟的二分频   注意这个值最大为36MHz*/
        RCC_PCLK1Config(RCC_HCLK_Div2);
        
        /* ADCCLK = PCLK2/6 设置ADC外设时钟=低速总线2时钟的六分频   注意这个值最大为14MHz*/
//        RCC_ADCCLKConfig(RCC_PCLK2_Div6);				  //这个是不需要单独使能的 。其他类似的还有几个，具体参见资料
        
        /* Set PLL clock output to 72MHz using HSE (8MHz) as entry clock */ 
        
        /* 利用锁相环讲外部8Mhz晶振9倍频到72Mhz */ 
        RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9); 
        /* Enable PLL 使能锁相环 以产生SYSCLK，也即启动PLL*/
        RCC_PLLCmd(ENABLE); 
        
        /* Wait till PLL is ready 等待锁相环输出稳定*/
        while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
        {}
        
        /* Select PLL as system clock source 将锁相环输出设置为系统时钟 此时会真正产生系统时钟*/
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
        
        /* Wait till PLL is used as system clock source 等待校验成功*/
        while (RCC_GetSYSCLKSource() != 0x08)
        {}
        
        /* Enable FSMC, GPIOD, GPIOE, GPIOF, GPIOG and AFIO clocks */
        //使能外围接口总线时钟，注意各外设的隶属情况，不同芯片的分配不同，到时候查手册就可以
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
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);   //抢占优先级2，响应优先级2    
}


/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : Configures the different GPIO ports. CAN TRX config； Test Pin；
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GPIO_Configuration(void)
{
  	GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_LED_GPIOx, ENABLE);   //GPIO时钟使能
    
    /* Configure PB0 as normal gpio push-pull */
    GPIO_InitStructure.GPIO_Pin = LED0_PINx;					//管脚选择，LED_PINx使用了宏定义
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	    //管脚速率选择
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		//管脚功能配置
    GPIO_Init(LED0_GPIOx, &GPIO_InitStructure);				//端口选择：如PA，PB口
    
    GPIO_ResetBits(LED0_GPIOx, LED0_PINx);                    //端口状态复位

}



void GPIO_PinReverse(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
  /* Check the parameters */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  assert_param(IS_GPIO_PIN(GPIO_Pin));
  
  GPIOx->ODR ^=  GPIO_Pin;
}


/*********************软件延时*******************/
//软件延时函数，us级别
void Soft_delay_us(u16 time)
{    
   u32 i;  
   while(time--)
   {
      i=8;  //自己定义  调试时间：20141116
      while(i--){};
        __nop();
        __nop();
        __nop();
        __nop();
   }
}

//软件延时函数，ms级别
void Soft_delay_ms(u16 time)
{    
   u16 i=0;  
   while(time--)
   {
      i=7950;  //自己定义  调试时间：20141116
      while(i--) ;    
   }
}


