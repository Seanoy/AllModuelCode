#include <stdio.h>			/* ��Ϊ�õ���printf���������Ա����������ļ� */
#include <string.h>
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "ff.h"
#include "sdio_sd.h"
#include "exfuns.h"
#include "malloc.h"
//ALIENTEKս��STM32������ʵ��1
//�����ʵ��  
//����֧�֣�www.openedv.com
//������������ӿƼ����޹�˾
static void CreateNewFile(void);

char path0[512]="0:";
char buffer[4096];   // file copy buffer 
uint8_t textFileBuffer[] = "��Ӣ�Ĳ����ַ��� \r\nChinese and English test strings \r\n";

//������
int main(void)
{	
	u32 total,free;
	int j;
	delay_init();	    //��ʱ������ʼ��	  
	NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);    //���ڳ�ʼ����������115200
	
	mem_init(SRAMIN);	//��ʼ���ڲ��ڴ��	
	exfuns_init();		//Ϊfatfs��ر��������ڴ� 
	printf("************* SD test ****************\r\n");//
	
	SDIO_Interrupts_Config(); // ����SDIO�ж�
	
	res = f_mount(0, fs[0]); //����SD�� 0��ʾ��ֱ꣬�Ӵ��ݵ��ײ�diskio.c��
	if (res != FR_OK)
	{
		printf("mount filesystem 0 failed : %d\r\n",res);
	}

	//д�ļ�����
	printf("write file test......\r\n");
	
	res = f_open(file1, "0:/test.txt", FA_CREATE_ALWAYS | FA_WRITE);
	if(res != FR_OK)
	{
		printf("open file error : %d\r\n",res);
	}
	else
	{
	    res = f_write(file1, textFileBuffer, sizeof(textFileBuffer), &bw);               // Write it to the dst file 
		if(res == FR_OK)
		{
			printf("write data ok! %d\r\n",bw);
		}
		else
		{
			printf("write data error : %d\r\n",res);
		}		
		
		f_write(file1, "seanoy666\r\n",9, &bw);
		
		f_close(file1);//close file 
	}

//	//���ļ�����
//	printf("read file test......\r\n");
//    res = f_open(file2, "0:/seanoy123.txt", FA_OPEN_EXISTING | FA_READ);
//    if(res != FR_OK)
//	{
//		printf("open file error : %d\r\n",res);
//	}
//	else
//	{
//	    res = f_read(file2, buffer, (sizeof(textFileBuffer)+9), &br);     // Read a chunk of src file 
//		if(res==FR_OK)
//		{
//			printf("read data num : %d\r\n",br);
////			printf("%s\n\r",buffer);
//			for(j=0; j<br; j++)
//			{
//				printf("%c",buffer[j]);
//			}
//			printf("\n\r");
//		}
//		else
//		{
//			printf("read file error : %d\r\n",res);
//		}	
//		f_close(file2);	//close file 
//	}
//	
	//ɨ���Ѿ����ڵ��ļ�
	printf("\n\rbegin scan files path0......\r\n");
	f_scan_files(path0);
	
	//��ȡSD����
	exf_getfree(path0,&total,&free);
	printf("SD Total Size:%d KB\r\nSD Free Size: %d KB",total,free);
	while(1)
	{ 

	}
	
//	CreateNewFile();	
//	while(1);
	
}

/*
*********************************************************************************************************
*	�� �� ��: CreateNewFile
*	����˵��: ��SD������һ�����ļ����ļ�������д��www.armfly.com��
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void CreateNewFile(void)
{
	/* ������ʹ�õľֲ�����ռ�ý϶࣬���޸������ļ�����֤��ջ�ռ乻�� */
	FRESULT result;
	FATFS fs;
	FIL file;
//	DIR DirInf;  
	uint32_t bw;
	
 	/* �����ļ�ϵͳ */
	result = f_mount(0, &fs);			/* Mount a logical drive */
	if (result != FR_OK)
	{
		printf("�����ļ�ϵͳʧ�� (%d)\r\n", result);
	}
	
//	/* �򿪸��ļ��� */
//	result = f_opendir(&DirInf, "/"); /* ���������������ӵ�ǰĿ¼��ʼ */
//	if (result != FR_OK) 
//	{
//		printf("�򿪸�Ŀ¼ʧ�� (%d)\r\n", result);
//		return;
//	}

	/* ���ļ� */
	result = f_open(&file, "0:/armfly.txt", FA_CREATE_ALWAYS | FA_WRITE);
	
	/* дһ������ */
	result = f_write(&file, "FatFS Write Demo \r\n www.armfly.com \r\n", 34, &bw);	
	if (result == FR_OK)
	{
		printf("armfly.txt �ļ�д��ɹ�\r\n");
	}
	else
	{
		printf("armfly.txt �ļ�д��ʧ��\r\n");
	}
	
	/* �ر��ļ�*/
	f_close(&file);
 	
	/* ж���ļ�ϵͳ */
	f_mount(0, NULL);	
}
