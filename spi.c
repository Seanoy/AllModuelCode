#include "spi.h"
#include "LED_RamSetting.h"
#include "systick.h"
#include "matrixscreen.h"

void spi_delay(void)
{
    //不需要延时，按最快速度
}

uint8_t SPI_Init(void)
{
    uint8_t id;

	/* enable the LED1 GPIO clock */
	rcu_periph_clock_enable(RCU_GPIOA);
	rcu_periph_clock_enable(RCU_GPIOB);

	//MOSI
	gpio_mode_set(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_5);
	gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_5);
    MOSI_0();

    //MISO
	gpio_mode_set(GPIOB, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO_PIN_4);

    //SCK
    gpio_mode_set(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_3);
    gpio_output_options_set(GPIOB,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ, GPIO_PIN_3);
    SCK_0();

    //CS
    gpio_mode_set(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_4);
    gpio_output_options_set(GPIOA,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ, GPIO_PIN_4);
    CS_1();

#ifdef SLED17341X
//使用点阵屏
    id = SPI_ReadID();

    if(id != 0x73)
        return 1;
    else
        return 0;
#endif
}

inline void SPI_Transmit(u8 *data, int length)
{
    int i;
    for(i=0;i<length;i++)
    {
        SPI_WriteByte(*(data+i));
    }
}

inline void SPI_WriteByte(uint8_t byte)
{
	uint8_t i,j;
	for(i=0;i<8;i++)
	{
		if((byte &0x80)==0x80)
		{
			MOSI_1();
		}
		else
		{
			MOSI_0();
		}
		byte <<=1;
		SCK_1();
		spi_delay();
		SCK_0();
	}
}

inline uint8_t	SPI_ReadByte(void)
{
	uint8_t i,j;
	uint8_t byte=0;
	
	for(i=0;i<8;i++)
	{
		SCK_1();
		spi_delay();	
		
		byte <<=1;
		if(MISO_RD())			//read miso
		{
			byte |=1;
		}
		SCK_0();
		spi_delay();
	}
	return byte;
}


//使用点阵屏
#ifdef SLED17341X
uint8_t SPI_ReadID(void)
{
    uint8_t ID;
    CS_0();

    SPI_WriteByte(0xAB);
    SPI_WriteByte(0x1B);
    ID =SPI_ReadByte();
	
    CS_1();
    return ID;
}

/*****************************************************************************
* Function		: SPI_W_3BYTE
* Description	: Write 3 Byte Data by SPI
* Input			: None
* Output		: None
* Return		: None
* Note			: None
*****************************************************************************/
void SPI_W_3BYTE(uint8_t bPage, uint8_t bAddr, uint8_t bData)
{
    u16 n;
    CS_0();
	wSPI_NBytes = 3; 
	hwSPI_Tx_Fifo[0] = (0x20|bPage);
	hwSPI_Tx_Fifo[1] = bAddr;
	hwSPI_Tx_Fifo[2] = bData;	
    SPI_Transmit(hwSPI_Tx_Fifo,wSPI_NBytes);
    CS_1();
}
/*****************************************************************************
* Function		: SPI_W_NBYTE
* Description	: SPI Write N bytes hwSPI_Rx_Fifo[] FIFO Data to SLED1735
* Input			: the start Address of the Ram of SLED1735 , and the Data Length
* Output		: SPI Data 
* Return		: None
* Note			: None
*****************************************************************************/
void SPI_W_NBYTE(uint8_t bPage, uint8_t bStart_Addr, uint8_t blength)
{
    u16 n;
    CS_0();
	hwSPI_Tx_Fifo[0] = (0x20|bPage);
	hwSPI_Tx_Fifo[1] = bStart_Addr;
	wSPI_NBytes =	blength+2;
    SPI_Transmit(hwSPI_Tx_Fifo,wSPI_NBytes);
    CS_1();
}
#endif