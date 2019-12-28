/**
  ******************************************************************************
  * @file    bsp_SysTick.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   SysTick ϵͳ�δ�ʱ��10us�жϺ�����,�ж�ʱ����������ã�
  *          ���õ��� 1us 10us 1ms �жϡ�     
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� ISO-MINI STM32 ������ 
  * ��̳    :http://www.chuxue123.com
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
//SysTick_Config��Ҫ���������ж�����������STK_VAL�Ĵ���������SysTickʱ��ΪAHB 
#include "bsp_SysTick.h"

static __IO u32 TimingDelay;

uint8_t SYSTICK_Delay_Flag = 0;

/**
  * @brief  ����ϵͳ�δ�ʱ�� SysTick
  * @param  ��
  * @retval ��
  */
void SysTick_Init(void)
{
// 	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//ѡ���ⲿʱ��  HCLK/8
//     SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);	//ѡ���ⲿʱ��  HCLK
    
    /* SystemFrequency / 1000    1ms�ж�һ��
	 * SystemFrequency / 100000	 10us�ж�һ��
	 * SystemFrequency / 1000000 1us�ж�һ��
	 */
//	if (SysTick_Config(SystemFrequency / 100000))	// ST3.0.0��汾
	if (SysTick_Config(SystemCoreClock / 100000))	// ST3.5.0��汾  ʹ���ں�ʱ�ӣ�����Ƶ
	{ 
		/* Capture error */ 
		while (1);
	}
    // �رյδ�ʱ��  
    SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;
		//�����δ�ʱ��
//		SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;
}


/**
  * @brief   us��ʱ����,10usΪһ����λ
  * @param  
  *		@arg nTime: Delay_us( 1 ) ��ʵ�ֵ���ʱΪ 1 * 10us = 10us
  * @retval  ��
  */
void Delay_us(__IO u32 nTime)
{ 
	SYSTICK_Delay_Flag = 1;
	
	TimingDelay = nTime;

    //��ռ����� 
	SysTick->VAL &= 0;
	// ʹ�ܵδ�ʱ����ʹ�ܵδ��ж� 
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk ;

	while(TimingDelay != 0);
    
    // �رյδ�ʱ�����رյδ��ж�
	SysTick->CTRL &= (~ SysTick_CTRL_TICKINT_Msk) & (~ SysTick_CTRL_ENABLE_Msk);
    //��ռ����� 
	SysTick->VAL &= 0;
	
	SYSTICK_Delay_Flag = 0;
}

/**
  * @brief  ��ȡ���ĳ���
  * @param  ��
  * @retval ��
  * @attention  �� SysTick �жϺ��� SysTick_Handler()����
  */
void TimingDelay_Decrement(void)
{
	if (TimingDelay != 0x00)
	{ 
		TimingDelay--;
	}
}
/*********************************************END OF FILE**********************/
