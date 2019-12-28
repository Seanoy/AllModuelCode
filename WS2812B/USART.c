#include "stm32f10x.h"
#include "stdio.h"


/*initialize NVIC*/
static void NVIC_Config(void){
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/*嵌套中断向量控制组的选择*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	/*配置USART为中断源*/
	NVIC_InitStructure.NVIC_IRQChannel = 	USART1_IRQn ;
	/*抢占优先级为1*/
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	/*子优先级为1*/
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	/*使能中断*/
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	/*initializatio*/
	NVIC_Init(&NVIC_InitStructure);
}

/*initialize USART*/
void USART_Config(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	//打开串口GPIO和USART的时钟和功能复用 IO 时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1 | RCC_APB2Periph_AFIO,ENABLE);
	
	//将USART Tx的GPIO配置为推挽复用模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//将USART Rx的GPIO配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	/*配置串口的工作参数*/
	//配置波特率
	USART_InitStructure.USART_BaudRate = 115200;
	//配置帧数据字长
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	//配置停止位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	//配置校验位
	USART_InitStructure.USART_Parity = USART_Parity_No;
	//配置硬件流控制
	USART_InitStructure.USART_HardwareFlowControl =USART_HardwareFlowControl_None;
	//配置工作模式，收发
	USART_InitStructure.USART_Mode =  USART_Mode_Rx|USART_Mode_Tx;
	//完成串口的初始化配置
	USART_Init(USART1,&USART_InitStructure);
	
	//串口中断优先级配置
	NVIC_Config();
	
	//使能串口接收中断
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	
	//使能串口
	USART_Cmd(USART1,ENABLE);
}


// 重定向c库函数printf到USART1
int fputc(int ch, FILE *f)
{
                /* 发送一个字节数据到USART1 */
                USART_SendData(USART1, (uint8_t) ch);
                
                /* 等待发送完毕 */
                while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);                
        
                return (ch);
}
