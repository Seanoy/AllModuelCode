/**
  ******************************************************************************
  * @file    bsp_SysTick.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   SysTick 系统滴答时钟10us中断函数库,中断时间可自由配置，
  *          常用的有 1us 10us 1ms 中断。     
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 ISO-MINI STM32 开发板 
  * 论坛    :http://www.chuxue123.com
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
//SysTick_Config主要用来配置中端向量，重置STK_VAL寄存器，配置SysTick时钟为AHB 
#include "bsp_SysTick.h"

static __IO u32 TimingDelay;

uint8_t SYSTICK_Delay_Flag = 0;

/**
  * @brief  启动系统滴答定时器 SysTick
  * @param  无
  * @retval 无
  */
void SysTick_Init(void)
{
// 	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//选择外部时钟  HCLK/8
//     SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);	//选择外部时钟  HCLK
    
    /* SystemFrequency / 1000    1ms中断一次
	 * SystemFrequency / 100000	 10us中断一次
	 * SystemFrequency / 1000000 1us中断一次
	 */
//	if (SysTick_Config(SystemFrequency / 100000))	// ST3.0.0库版本
	if (SysTick_Config(SystemCoreClock / 100000))	// ST3.5.0库版本  使用内核时钟，不分频
	{ 
		/* Capture error */ 
		while (1);
	}
    // 关闭滴答定时器  
    SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;
		//开启滴答定时器
//		SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;
}


/**
  * @brief   us延时程序,10us为一个单位
  * @param  
  *		@arg nTime: Delay_us( 1 ) 则实现的延时为 1 * 10us = 10us
  * @retval  无
  */
void Delay_us(__IO u32 nTime)
{ 
	SYSTICK_Delay_Flag = 1;
	
	TimingDelay = nTime;

    //清空计数器 
	SysTick->VAL &= 0;
	// 使能滴答定时器，使能滴答中断 
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk ;

	while(TimingDelay != 0);
    
    // 关闭滴答定时器，关闭滴答中断
	SysTick->CTRL &= (~ SysTick_CTRL_TICKINT_Msk) & (~ SysTick_CTRL_ENABLE_Msk);
    //清空计数器 
	SysTick->VAL &= 0;
	
	SYSTICK_Delay_Flag = 0;
}

/**
  * @brief  获取节拍程序
  * @param  无
  * @retval 无
  * @attention  在 SysTick 中断函数 SysTick_Handler()调用
  */
void TimingDelay_Decrement(void)
{
	if (TimingDelay != 0x00)
	{ 
		TimingDelay--;
	}
}
/*********************************************END OF FILE**********************/
