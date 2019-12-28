#include "ws2812b.h"
/* Buffer that holds one complete DMA transmission
 * 
 * Ensure that this buffer is big enough to hold
 * all data bytes that need to be sent
 * 
 * The buffer size can be calculated as follows:
 * number of LEDs * 24 bytes + 42 bytes
 * 
 * This leaves us with a maximum string length of
 * (2^16 bytes per DMA stream - 42 bytes)/24 bytes per LED = 2728 LEDs
 */

uint16_t LED_BYTE_Buffer[1024];
u8 default_alpha = 100;//0~255

Color_Type rainbow[30] = {RED,ORANGE,YELLOW,GREEN,CYAN,BLUE,PURPLE,RED,ORANGE,YELLOW,GREEN,CYAN,BLUE,PURPLE,RED,ORANGE,YELLOW,GREEN,CYAN,BLUE,PURPLE,RED,ORANGE,YELLOW,GREEN,CYAN,BLUE,PURPLE,RED,ORANGE};

float color_table[][3] = //颜色的比值
{
    1,1,1,//白
    1,0,0,//红
    1,0.2,0,//橙
    1,1,0,//黄
    0,1,0,//绿
    0,1,1,//青
    0,0,1,//蓝
    1,0,1,//紫
};

void WS2812B_Init(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    DMA_InitTypeDef DMA_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	/* GPIOA Configuration: TIM3 Channel 1 as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	/* Compute the prescaler value */
	//PrescalerValue = (uint16_t) (SystemCoreClock / 24000000) - 1;
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 90-1; // 72M/90=800kHz 
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	/* PWM1 Mode configuration: Channel3 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);

//没有启动预装载寄存器ARR，以及没有启动TIM3（Send函数里启动）
	
	/* configure DMA */
	/* DMA clock enable */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	/* DMA1 Channel3 Config */
	DMA_DeInit(DMA1_Channel3);

	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)TIM3_CCR3_Address;	// physical address of Timer 3 CCR3
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)LED_BYTE_Buffer;		// this is the buffer memory 
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;						// data shifted from memory to peripheral
	DMA_InitStructure.DMA_BufferSize = 42;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;					// automatically increase buffer index
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;							// stop DMA feed after buffer size is reached
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	
	DMA_Init(DMA1_Channel3, &DMA_InitStructure);

    /* TIM3 DMA Request enable */
	TIM_DMACmd(TIM3, TIM_DMA_Update, ENABLE);
}

//DMA启动发送
void DMA_Transmit(u16 buffersize)
{
    DMA_SetCurrDataCounter(DMA1_Channel3, buffersize); 	// load number of bytes to be transferred
    DMA_Cmd(DMA1_Channel3, ENABLE); 			// enable DMA channel 6
    TIM_Cmd(TIM3, ENABLE); 						// enable Timer 3
    while(!DMA_GetFlagStatus(DMA1_FLAG_TC3)) ; 	// wait until transfer complete
    TIM_Cmd(TIM3, DISABLE); 	                // disable Timer 3
    DMA_Cmd(DMA1_Channel3, DISABLE); 			// disable DMA channel 6
    DMA_ClearFlag(DMA1_FLAG_TC3); 				// clear DMA1 Channel 6 transfer complete flag
}

/* This function sends data bytes out to a string of WS2812s
 * The first argument is a pointer to the first RGB triplet to be sent
 * The seconds argument is the number of LEDs in the chain
 * 
 * This will result in the RGB triplet passed by argument 1 being sent to 
 * the LED that is the furthest away from the controller (the point where
 * data is injected into the chain)
 *///
//如果display_all等于1 全部灯都显示color数组第一个颜色

void WS2812_Display(Color_Type color[],uint16_t len, uint8_t display_all)
{
	uint8_t num;//灯的序号
    uint8_t i;//用于循环发送原色的每一位数据
	uint16_t memaddr=0;//缓存区的地址
	uint16_t buffersize;//缓存区的大小
	u8 red;//红色色值
    u8 green;//绿色色值
    u8 blue;//蓝色色值
	buffersize = (len*24)+43;	// number of bytes needed is #LEDs * 24 bytes + 43 trailing bytes
	
	for(num=0;num<len;num++)
	{	
		//Ordered in GRB,not RGB
        if(!display_all)
            red = color_table[color[num]][1]*default_alpha;
        else
            red = color_table[color[0]][1]*default_alpha;
        for(i=0; i<8; i++) // GREEN data
        {
            LED_BYTE_Buffer[memaddr++] = (red<<i & 0x0080) ? TIMING_ONE:TIMING_ZERO;
        }
        if(!display_all)
            green = color_table[color[num]][0]*default_alpha;
        else
            green = color_table[color[0]][0]*default_alpha;
        for(i=0; i<8; i++) // RED
        {
            LED_BYTE_Buffer[memaddr++] = (green<<i & 0x0080) ? TIMING_ONE:TIMING_ZERO;
        }
        if(!display_all)
            blue = color_table[color[num]][2]*default_alpha;
        else
            blue = color_table[color[0]][2]*default_alpha;
        for(i=0; i<8; i++) // BLUE
        {
            LED_BYTE_Buffer[memaddr++] = (blue<<i & 0x0080) ? TIMING_ONE:TIMING_ZERO;
        }
	}
    DMA_Transmit(buffersize);
}
