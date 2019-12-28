#include <stdio.h>			/* 因为用到了printf函数，所以必须包含这个文件 */
#include <string.h>
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "ff.h"
#include "sdio_sd.h"
#include "exfuns.h"
#include "malloc.h"
//ALIENTEK战舰STM32开发板实验1
//跑马灯实验  
//技术支持：www.openedv.com
//广州市星翼电子科技有限公司
static void CreateNewFile(void);

char path0[512]="0:";
char buffer[4096];   // file copy buffer 
uint8_t textFileBuffer[] = "中英文测试字符串 \r\nChinese and English test strings \r\n";

//主函数
int main(void)
{	
	u32 total,free;
	int j;
	delay_init();	    //延时函数初始化	  
	NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);    //串口初始化，波特率115200
	
	mem_init(SRAMIN);	//初始化内部内存池	
	exfuns_init();		//为fatfs相关变量申请内存 
	printf("************* SD test ****************\r\n");//
	
	SDIO_Interrupts_Config(); // 配置SDIO中断
	
	res = f_mount(0, fs[0]); //挂载SD卡 0表示卷标，直接传递到底层diskio.c中
	if (res != FR_OK)
	{
		printf("mount filesystem 0 failed : %d\r\n",res);
	}

	//写文件测试
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

//	//读文件测试
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
	//扫描已经存在的文件
	printf("\n\rbegin scan files path0......\r\n");
	f_scan_files(path0);
	
	//获取SD容量
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
*	函 数 名: CreateNewFile
*	功能说明: 在SD卡创建一个新文件，文件内容填写“www.armfly.com”
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void CreateNewFile(void)
{
	/* 本函数使用的局部变量占用较多，请修改启动文件，保证堆栈空间够用 */
	FRESULT result;
	FATFS fs;
	FIL file;
//	DIR DirInf;  
	uint32_t bw;
	
 	/* 挂载文件系统 */
	result = f_mount(0, &fs);			/* Mount a logical drive */
	if (result != FR_OK)
	{
		printf("挂载文件系统失败 (%d)\r\n", result);
	}
	
//	/* 打开根文件夹 */
//	result = f_opendir(&DirInf, "/"); /* 如果不带参数，则从当前目录开始 */
//	if (result != FR_OK) 
//	{
//		printf("打开根目录失败 (%d)\r\n", result);
//		return;
//	}

	/* 打开文件 */
	result = f_open(&file, "0:/armfly.txt", FA_CREATE_ALWAYS | FA_WRITE);
	
	/* 写一串数据 */
	result = f_write(&file, "FatFS Write Demo \r\n www.armfly.com \r\n", 34, &bw);	
	if (result == FR_OK)
	{
		printf("armfly.txt 文件写入成功\r\n");
	}
	else
	{
		printf("armfly.txt 文件写入失败\r\n");
	}
	
	/* 关闭文件*/
	f_close(&file);
 	
	/* 卸载文件系统 */
	f_mount(0, NULL);	
}
