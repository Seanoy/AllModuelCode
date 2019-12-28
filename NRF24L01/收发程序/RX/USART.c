#include "stm32f10x.h"
#include "stdio.h"


/*initialize NVIC*/
static void NVIC_Config(void){
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/*Ƕ���ж������������ѡ��*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	/*����USARTΪ�ж�Դ*/
	NVIC_InitStructure.NVIC_IRQChannel = 	USART1_IRQn ;
	/*��ռ���ȼ�Ϊ1*/
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	/*�����ȼ�Ϊ1*/
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	/*ʹ���ж�*/
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	/*initializatio*/
	NVIC_Init(&NVIC_InitStructure);
}

/*initialize USART*/
void USART_Config(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	//�򿪴���GPIO��USART��ʱ�Ӻ͹��ܸ��� IO ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1 | RCC_APB2Periph_AFIO,ENABLE);
	
	//��USART Tx��GPIO����Ϊ���츴��ģʽ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//��USART Rx��GPIO����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	/*���ô��ڵĹ�������*/
	//���ò�����
	USART_InitStructure.USART_BaudRate = 115200;
	//����֡�����ֳ�
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	//����ֹͣλ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	//����У��λ
	USART_InitStructure.USART_Parity = USART_Parity_No;
	//����Ӳ��������
	USART_InitStructure.USART_HardwareFlowControl =USART_HardwareFlowControl_None;
	//���ù���ģʽ���շ�
	USART_InitStructure.USART_Mode =  USART_Mode_Rx|USART_Mode_Tx;
	//��ɴ��ڵĳ�ʼ������
	USART_Init(USART1,&USART_InitStructure);
	
	//�����ж����ȼ�����
	NVIC_Config();
	
	//ʹ�ܴ��ڽ����ж�
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	
	//ʹ�ܴ���
	USART_Cmd(USART1,ENABLE);
}


// �ض���c�⺯��printf��USART1
int fputc(int ch, FILE *f)
{
                /* ����һ���ֽ����ݵ�USART1 */
                USART_SendData(USART1, (uint8_t) ch);
                
                /* �ȴ�������� */
                while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);                
        
                return (ch);
}
