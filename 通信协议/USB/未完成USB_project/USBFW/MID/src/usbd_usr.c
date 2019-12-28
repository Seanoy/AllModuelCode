/**
  ******************************************************************************
  * @file    usbd_usr.c
  * @author  MCD Application Team
  * @version V1.2.1
  * @date    17-March-2018
  * @brief   This file includes the user application layer
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

/* Includes ------------------------------------------------------------------ */
#include "usbd_usr.h"

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
* @{
*/

/** @defgroup USBD_USR
* @brief    This file includes the user application layer
* @{
*/

/** @defgroup USBD_USR_Private_TypesDefinitions
* @{
*/
/**
* @}
*/


/** @defgroup USBD_USR_Private_Defines
* @{
*/
/**
* @}
*/


/** @defgroup USBD_USR_Private_Macros
* @{
*/
/**
* @}
*/


/** @defgroup USBD_USR_Private_Variables
* @{
*/

USBD_Usr_cb_TypeDef USR_cb = {
  USBD_USR_Init,
  USBD_USR_DeviceReset,
  USBD_USR_DeviceConfigured,
  USBD_USR_DeviceSuspended,
  USBD_USR_DeviceResumed,

  USBD_USR_DeviceConnected,
  USBD_USR_DeviceDisconnected,
};

uint32_t ADC_ConvertedValueX = 0;
uint32_t ADC_ConvertedValueX_1 = 0;

/**
* @}
*/

/** @defgroup USBD_USR_Private_Constants
* @{
*/

/**
* @}
*/



/** @defgroup USBD_USR_Private_FunctionPrototypes
* @{
*/
static void ADC_Configuration(void);
/**
* @}
*/


/** @defgroup USBD_USR_Private_Functions
* @{
*/

#define USER_INFORMATION1      (uint8_t*)"[Key]:switch On/Off Button[ReportID 5] "
#define USER_INFORMATION2      (uint8_t*)"[Leds]:switch on/off Leds [ReportID 1:4]"
#define USER_INFORMATION3      (uint8_t*)"[Potentiometer]:[ReportID 7]"

/**
* @brief  USBD_USR_Init
*         Displays the message on LCD for host lib initialization
* @param  None
* @retval None
*/
void USBD_USR_Init(void)
{
  /* Initialize LEDs */
  STM_EVAL_LEDInit(LED1);
  STM_EVAL_LEDInit(LED2);
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);

  /* - Enable peripheral clocks */
  /* - ADCx_DMA_STREAM channel configuration */
  /* - Configure ADCx Channel pin as analog input */
  /* - Configure ADCx Channel */
  ADC_Configuration();

  /* Initialize the LCD */
#if defined (USE_STM322xG_EVAL)
  STM322xG_LCD_Init();
#elif defined(USE_STM324xG_EVAL)
  STM324xG_LCD_Init();
#elif defined(USE_STM324x9I_EVAL)

  LCD_Init();
  LCD_LayerInit();

  /* Enable The Display */
  LCD_DisplayOn();
  /* Connect the Output Buffer to LCD Background Layer */
  LCD_SetLayer(LCD_FOREGROUND_LAYER);

  /* Clear the Background Layer */
  LCD_Clear(LCD_COLOR_WHITE);

#elif defined (USE_STM3210C_EVAL)
  STM3210C_LCD_Init();
#else
#error "Missing define: Evaluation board (ie. USE_STM322xG_EVAL)"
#endif

  LCD_LOG_Init();

#ifdef USE_USB_OTG_HS
  #ifdef USE_EMBEDDED_PHY
  LCD_LOG_SetHeader((uint8_t *) "HS_IN_FS CUSTOMHID Device");
  #else
  LCD_LOG_SetHeader((uint8_t *) "HS CUSTOMHID Device");
  #endif
#else
  LCD_LOG_SetHeader((uint8_t *) "FS CUSTOMHID Device");
#endif
  LCD_UsrLog("> USB device library started.\n");
  LCD_LOG_SetFooter((uint8_t *) "     USB Device Library V1.2.1");

  /* Information panel */
  LCD_SetTextColor(Green);
  LCD_DisplayStringLine(LCD_PIXEL_HEIGHT - 30, USER_INFORMATION1);
  LCD_DisplayStringLine(LCD_PIXEL_HEIGHT - 42, USER_INFORMATION2);
  LCD_DisplayStringLine(LCD_PIXEL_HEIGHT - 54, USER_INFORMATION3);
  LCD_SetTextColor(LCD_LOG_DEFAULT_COLOR);

}

/**
* @brief  USBD_USR_DeviceReset
*         Displays the message on LCD on device Reset Event
* @param  speed : device speed
* @retval None
*/
void USBD_USR_DeviceReset(uint8_t speed)
{
  switch (speed)
  {
  case USB_OTG_SPEED_HIGH:
    LCD_LOG_SetFooter((uint8_t *) "     USB Device Library V1.2.1 [HS]");
    break;

  case USB_OTG_SPEED_FULL:
    LCD_LOG_SetFooter((uint8_t *) "     USB Device Library V1.2.1 [FS]");
    break;
  default:
    LCD_LOG_SetFooter((uint8_t *) "     USB Device Library V1.2.1 [??]");

  }
}


/**
* @brief  USBD_USR_DeviceConfigured
*         Displays the message on LCD on device configuration Event
* @param  None
* @retval Staus
*/
void USBD_USR_DeviceConfigured(void)
{
  LCD_UsrLog("> CUSTOMHID Interface started.\n");

}


/**
* @brief  USBD_USR_DeviceConnected
*         Displays the message on LCD on device connection Event
* @param  None
* @retval Status
*/
void USBD_USR_DeviceConnected(void)
{
  LCD_UsrLog("> USB Device Connected.\n");
}


/**
* @brief  USBD_USR_DeviceDisonnected
*         Displays the message on LCD on device disconnection Event
* @param  None
* @retval Status
*/
void USBD_USR_DeviceDisconnected(void)
{
  LCD_UsrLog("> USB Device Disconnected.\n");
}

/**
* @brief  USBD_USR_DeviceSuspended
*         Displays the message on LCD on device suspend Event
* @param  None
* @retval None
*/
void USBD_USR_DeviceSuspended(void)
{
  LCD_UsrLog("> USB Device in Suspend Mode.\n");
  /* Users can do their application actions here for the USB-Reset */
}


/**
* @brief  USBD_USR_DeviceResumed
*         Displays the message on LCD on device resume Event
* @param  None
* @retval None
*/
void USBD_USR_DeviceResumed(void)
{
  LCD_UsrLog("> USB Device in Idle Mode.\n");
  /* Users can do their application actions here for the USB-Reset */
}

/**
  * @brief  ADC3 channel07 with DMA configuration
  * @note   This function Configure the ADC peripheral
            1) Enable peripheral clocks
            2) DMA2_Stream0 channel2 configuration
            3) Configure ADC Channel7 pin as analog input
            4) Configure ADC3 Channel7
  * @param  None
  * @retval None
  */
static void ADC_Configuration(void)
{
  ADC_InitTypeDef ADC_InitStructure;
  DMA_InitTypeDef DMA_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

#if !defined (USE_STM3210C_EVAL)
  ADC_CommonInitTypeDef ADC_CommonInitStructure;
#endif

  /* Enable ADCx, DMA and GPIO clocks *************************************** */
#if !defined (USE_STM3210C_EVAL)
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
  RCC_AHB1PeriphClockCmd(ADCx_CHANNEL_GPIO_CLK, ENABLE);
#else
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
  /* Enable ADC1 and GPIOC clock */
  RCC_APB2PeriphClockCmd(ADCx_CHANNEL_GPIO_CLK, ENABLE);
#endif

  RCC_APB2PeriphClockCmd(ADCx_CLK, ENABLE);

  /* DMA2 Stream0 channel2 configuration ************************************* */
#if !defined (USE_STM3210C_EVAL)
  DMA_InitStructure.DMA_Channel = ADCx_DMA_CHANNEL;
#endif
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) ADCx_DR_ADDRESS;
#if !defined (USE_STM3210C_EVAL)
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t) & ADC_ConvertedValueX;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
#else
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) & ADC_ConvertedValueX;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
#endif
  DMA_InitStructure.DMA_BufferSize = 1;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
#if defined (USE_STM3210C_EVAL)
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);
#endif

#if defined (USE_STM3210C_EVAL)
  /* Enable DMA1 channel1 */
  DMA_Cmd(DMA1_Channel1, ENABLE);
#endif

#if !defined (USE_STM3210C_EVAL)
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(ADCx_DMA_STREAM, &DMA_InitStructure);
  DMA_Cmd(ADCx_DMA_STREAM, ENABLE);
#endif

#if !defined (USE_STM3210C_EVAL)
  DMA_ITConfig(ADCx_DMA_STREAM, DMA_IT_TC, ENABLE);
#else
  DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);
#endif

  /* Configure ADC3 Channel7 pin as analog input ***************************** */
  GPIO_InitStructure.GPIO_Pin = ADCx_CHANNEL_PIN;
#if !defined (USE_STM3210C_EVAL)
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
#else
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
#endif

  GPIO_Init(ADCx_CHANNEL_GPIO_PORT, &GPIO_InitStructure);
#if !defined (USE_STM3210C_EVAL)
  /* ADC Common Init ********************************************************* */
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
  ADC_CommonInit(&ADC_CommonInitStructure);

  /* ADCx Init *************************************************************** */
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfConversion = 1;
  ADC_Init(ADCx, &ADC_InitStructure);

  /* ADC3 regular channel7 configuration ************************************* */
  ADC_RegularChannelConfig(ADCx, ADC_CHANNEL, 1, ADC_SampleTime_480Cycles);

  /* Enable DMA request after last transfer (Single-ADC mode) */
  ADC_DMARequestAfterLastTransferCmd(ADCx, ENABLE);

  /* Enable ADC3 DMA */
  ADC_DMACmd(ADCx, ENABLE);

  /* Enable ADC3 */
  ADC_Cmd(ADCx, ENABLE);

  /* Start ADC Software Conversion */
  ADC_SoftwareStartConv(ADCx);

#else
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel = 1;
  ADC_Init(ADC1, &ADC_InitStructure);

  /* ADC1 regular channel14 configuration */
  ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 1, ADC_SampleTime_55Cycles5);

  /* Enable ADC1 DMA */
  ADC_DMACmd(ADC1, ENABLE);

  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);

  /* Enable ADC1 reset calibration register */
  ADC_ResetCalibration(ADC1);
  /* Check the end of ADC1 reset calibration register */
  while (ADC_GetResetCalibrationStatus(ADC1));

  /* Start ADC1 calibration */
  ADC_StartCalibration(ADC1);
  /* Check the end of ADC1 calibration */
  while (ADC_GetCalibrationStatus(ADC1));

  /* Start ADC1 Software Conversion */
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);
#endif
  ADC_ConvertedValueX = ADC_GetConversionValue(ADCx);

}

/**
* @}
*/

/**
* @}
*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
