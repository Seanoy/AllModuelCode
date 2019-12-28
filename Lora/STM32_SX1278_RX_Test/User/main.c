/****************************************************************************
* Copyright (C), 2016 莱米物联研发中心
*
* 本例程在 STM32开发板上调试通过           
*
* 文件名: main.c
* 内容简述:	*       
*	演示的是安信可LoRa无线通信的功能。
*    
*	定义：	
*	LED1-LED3 ---V1--V3  注意郭版的开发板上LED灯为高电平点亮！
*		V1----- PB5-LED1
*		V2----- PD6-LED2
*		V3----- PD3-LED3			
*		Key1----PC5
*		Key2----PC2
*		Key3----PC3
*		Key4----PE6
* 硬件连接：
* 
* LoRa模块引脚连接
*  1--VDD--3.3V
*  3--GND--GND
*  5--PE1--RESET
*  7--PD4--DIO0
*  9--PD5--DIO1
* 11--PD7--DIO2
* 13--PD11--DIO3
* 
* 2--PA5--SCK
* 4--PB7--NSS
* 6--PA7--MOSI
* 8--PA6--MISO
* -----
*20--PD8--DIO5
* 22--PE15--DIO4
* 24--GND--GND
*  ------------------------
* | PA9  - USART1(Tx)      |
* | PA10 - USART1(Rx)      |
*  ------------------------    
*	基于MDK版本：        3.8
*	基于官方外设库版本： 3.5
*
* 文件历史:
* 版本号  日期       作者    说明
* v0.1   2017-2-11 	Duke 创建该文件
*
*******************************************************************************/
 
#include "sys_config.h"

#define BUFFER_SIZE     10                          // Define the payload size here

static uint16_t BufferSize = BUFFER_SIZE;			// RF buffer size
static uint8_t  Buffer[BUFFER_SIZE];				// RF buffer

static uint8_t EnableMaster = false; 				// Master/Slave selection

tRadioDriver *Radio = NULL;

const uint8_t PingMsg[] = "PING";
const uint8_t PongMsg[] = "PONG";
const uint8_t MY_TEST_Msg[] = "seanOY_LoraTest";


void OnMaster( void );
void OnSlave( void );
/*
 * Manages the master operation
 */
void OnMaster( void )
{
    uint8_t i;
    
    switch( Radio->Process( ) )
    {
    case RF_RX_TIMEOUT:
        // Send the next PING frame
        Buffer[0] = 'P';
        Buffer[1] = 'I';
        Buffer[2] = 'N';
        Buffer[3] = 'G';
        for( i = 4; i < BufferSize; i++ )
        {
            Buffer[i] = i - 4;
        }
        Radio->SetTxPacket( Buffer, BufferSize );   //RFLR_STATE_TX_INIT
        break;
    case RF_RX_DONE:
        Radio->GetRxPacket( Buffer, ( uint16_t* )&BufferSize );

		for(i=0;i<17;i++)
		{
			USART_putchar(USART1,Buffer[i]);
		}
//		printf("\n");
    
		if( BufferSize > 0 )
        {
            if( strncmp( ( const char* )Buffer, ( const char* )PongMsg, 4 ) == 0 )
            {
                // Indicates on a LED that the received frame is a PONG
//                LedToggle( LED_GREEN );
				LED0_TOGGLE;

                // Send the next PING frame            
                Buffer[0] = 'P';
                Buffer[1] = 'I';
                Buffer[2] = 'N';
                Buffer[3] = 'G';
                // We fill the buffer with numbers for the payload 
                for( i = 4; i < BufferSize; i++ )
                {
                    Buffer[i] = i - 4;
                }
                Radio->SetTxPacket( Buffer, BufferSize );    //RFLR_STATE_TX_INIT
            }
            else if( strncmp( ( const char* )Buffer, ( const char* )PingMsg, 4 ) == 0 )
            { // A master already exists then become a slave
                EnableMaster = false;
//                LedOff( LED_RED );
				LED0_OFF;
            }
        }            
        break;
    case RF_TX_DONE:
        // Indicates on a LED that we have sent a PING
//        LedToggle( LED_RED );
		LED0_TOGGLE;
        Radio->StartRx( );   //RFLR_STATE_RX_INIT
        break;
    default:
        break;
    }
}

/*
 * Manages the slave operation
 */
void OnSlave( void )
{
    uint8_t i;

    switch( Radio->Process( ) )
    {
    case RF_RX_DONE:
        Radio->GetRxPacket( Buffer, ( uint16_t* )&BufferSize );
    
        if( BufferSize > 0 )
        {
            if( strncmp( ( const char* )Buffer, ( const char* )PingMsg, 4 ) == 0 )
            {
                // Indicates on a LED that the received frame is a PING
//                LedToggle( LED_GREEN );
				LED0_TOGGLE;

               // Send the reply to the PONG string
                Buffer[0] = 'P';
                Buffer[1] = 'O';
                Buffer[2] = 'N';
                Buffer[3] = 'G';
                // We fill the buffer with numbers for the payload 
                for( i = 4; i < BufferSize; i++ )
                {
                    Buffer[i] = i - 4;
                }

                Radio->SetTxPacket( Buffer, BufferSize );      //RFLR_STATE_TX_INIT
            }
        }
        break;
    case RF_TX_DONE:
        // Indicates on a LED that we have sent a PONG
//        LedToggle( LED_RED );
		LED0_TOGGLE;
        Radio->StartRx( );   //RFLR_STATE_RX_INIT
        break;
    default:
        break;
    }
}




#define SX1278_RX
//#define SX1278_TX

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
	uint8_t i;
	//stm32 config
	sys_Configuration();
	
	BoardInit( );
    
    Radio = RadioDriverInit( );
    
    Radio->Init( );
		
#if defined (SX1278_RX)
    Radio->StartRx( );   //RFLR_STATE_RX_INIT
#elif defined (SX1278_TX)
	Radio->SetTxPacket( MY_TEST_Msg, 18 );
#endif
	
	while(1)
	{
#if defined (SX1278_RX)
		while( Radio->Process( ) == RF_RX_DONE)
		{
			Radio->GetRxPacket( Buffer, ( uint16_t* )&BufferSize );
//			if( strncmp( ( const char* )Buffer, ( const char* )MY_TEST_Msg, 18 ) == 0 )
            //这里可以通过判断数据头部和CRC校验来判断数据是否是所需要的数据
//			{
				for(i=0;i < BufferSize;i++)
				{
					USART_putchar(USART1,Buffer[i]);
				}
//				printf("\n");
				LED0_TOGGLE;
				for(i=0;i<BUFFER_SIZE;i++)//这里可以通过发送过来的包头来确定数据大小
				{
						Buffer[i] = 0;
				}
				Radio->StartRx( );
		}
#elif defined (SX1278_TX)
//		printf("运行在RF_LoRa_TX_OK？ \n");
		while(Radio->Process( ) == RF_TX_DONE)
		{
			printf("RF_LoRa_TX_OK! \n");
			LED0_TOGGLE;
			Soft_delay_ms(500);
		Radio->SetTxPacket( MY_TEST_Msg, strlen(MY_TEST_Msg) );   //RFLR_STATE_TX_INIT
		}		
#endif
		
	}
}



/*********************************************END OF FILE**********************/
