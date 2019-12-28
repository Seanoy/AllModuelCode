/**
  ******************************************************************************
  * @file    bsp_config.h
  * @author  MCD Application Team
  * @version V1.2.1
  * @date    17-March-2018
  * @brief   Header for usbd_bsp.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2015 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                      <http://www.st.com/SLA0044>
  *
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_CONFIG_H
#define __BSP_CONFIG_H

/* Includes ------------------------------------------------------------------*/
#if defined (USE_STM322xG_EVAL)
 #include "stm32f2xx.h"
#elif defined(USE_STM324xG_EVAL)
 #include "stm32f4xx.h"
#elif defined (USE_STM3210C_EVAL)
 #include "stm32f10x.h"
#endif
#include <stdio.h>

/* Private define ------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#if defined (USE_STM324xG_EVAL) || defined(USE_STM322xG_EVAL)
  #define ADCx                     ADC3
  #define ADC_CHANNEL              ADC_Channel_7
  #define ADCx_CLK                 RCC_APB2Periph_ADC3
  #define ADCx_CHANNEL_GPIO_CLK    RCC_AHB1Periph_GPIOF
  #define ADCx_CHANNEL_PIN         GPIO_Pin_9
  #define ADCx_CHANNEL_GPIO_PORT   GPIOF
  #define ADCx_DMA_CHANNEL         DMA_Channel_2
  #define ADCx_DMA_STREAM          DMA2_Stream0
  #define ADCx_DMA_IRQHandler      DMA2_Stream0_IRQHandler
  #define ADCx_DMA_IRQn            DMA2_Stream0_IRQn
  #define ADCx_DR_ADDRESS          ((uint32_t)0x4001224C)

#elif defined(USE_STM324x9I_EVAL)
  #define ADCx                     ADC3
  #define ADC_CHANNEL              ADC_Channel_8
  #define ADCx_CLK                 RCC_APB2Periph_ADC3
  #define ADCx_CHANNEL_GPIO_CLK    RCC_AHB1Periph_GPIOF
  #define ADCx_CHANNEL_PIN         GPIO_Pin_10
  #define ADCx_CHANNEL_GPIO_PORT   GPIOF
  #define ADCx_DMA_CHANNEL         DMA_Channel_2
  #define ADCx_DMA_STREAM          DMA2_Stream0
  #define ADCx_DMA_IRQHandler      DMA2_Stream0_IRQHandler
  #define ADCx_DMA_IRQn            DMA2_Stream0_IRQn
  #define ADCx_DR_ADDRESS          ((uint32_t)0x4001224C)

#else /* USE_STM3210C_EVAL*/
  #define ADCx                     ADC1
  #define ADC_CHANNEL              ADC_Channel_4
  #define ADCx_CLK                 RCC_APB2Periph_ADC1
  #define ADCx_CHANNEL_GPIO_CLK    RCC_APB2Periph_GPIOC
  #define ADCx_CHANNEL_PIN         GPIO_Pin_4
  #define ADCx_CHANNEL_GPIO_PORT   GPIOC
  #define DMA_CHANNELx             DMA_Channel_1
  #define ADCx_DMA_IRQHandler      DMA1_Channel1_IRQHandler
  #define ADCx_DMA_IRQn            DMA1_Channel1_IRQn
  #define ADCx_DR_ADDRESS          ((uint32_t)0x4001244C)

#endif
/* Exported functions ------------------------------------------------------- */

#endif /* __BSP_CONFIG_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
