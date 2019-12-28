#include "string.h"
#include "exfuns.h"
#include "malloc.h"
#include "usart.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//FATFS 扩展代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/18
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

#define SD_CARD		0	//SD卡,卷标为0
#define U_CARD		1	//U盘，卷标为1     
#define EX_FLASH 	2	//外部flash,卷标为2

//文件类型列表
const u8 *FILE_TYPE_TBL[6][13]=
{
	{"BIN"},			//BIN文件
	{"LRC"},			//LRC文件
	{"NES"},			//NES文件
	{"TXT","C","H"},	//文本文件
	{"MP1","MP2","MP3","MP4","M4A","3GP","3G2","OGG","ACC","WMA","WAV","MID","FLAC"},//音乐文件
	{"BMP","JPG","JPEG","GIF"},//图片文件
};
///////////////////////////////公共文件区,使用malloc的时候////////////////////////////////////////////
FATFS 	*fs[3];  		//逻辑磁盘工作区.	 
FIL 	*file1;	  		//文件1
FIL 	*file2;	  		//文件2.
UINT 	br,bw;			//读写变量
FILINFO fileinfo;		//文件信息
DIR 	dir;  			//目录
FRESULT res;			//返回值

u8 *fatbuf;			//SD卡数据缓存区
///////////////////////////////////////////////////////////////////////////////////////
//为exfuns申请内存
//返回值:0,成功
//1,失败
u8 exfuns_init(void)
{
	fs[0]=(FATFS*)mymalloc(SRAMIN,sizeof(FATFS));	//为磁盘0工作区申请内存	
	fs[1]=(FATFS*)mymalloc(SRAMIN,sizeof(FATFS));	//为磁盘1工作区申请内存
	file1=(FIL*)mymalloc(SRAMIN,sizeof(FIL));		//为file申请内存
	file2=(FIL*)mymalloc(SRAMIN,sizeof(FIL));		//为ftemp申请内存
	fatbuf=(u8*)mymalloc(SRAMIN,512);				//为fatbuf申请内存
	if(fs[0]&&fs[1]&&file1&&file2&&fatbuf)return 0;  //申请有一个失败,即失败.
	else return 1;	
}

//将小写字母转为大写字母,如果是数字,则保持不变.
u8 char_upper(u8 c)
{
	if(c<'A')return c;//数字,保持不变.
	if(c>='a')return c-0x20;//变为大写.
	else return c;//大写,保持不变
}	      

//报告文件的类型
//fname:文件名
//返回值:0XFF,表示无法识别的文件类型编号.
//		 其他,高四位表示所属大类,低四位表示所属小类.
u8 f_typetell(u8 *fname)
{
	u8 tbuf[5];
	u8 *attr='\0';//后缀名
	u8 i=0,j;
	while(i<250)
	{
		i++;
		if(*fname=='\0')break;//偏移到了最后了.
		fname++;
	}
	if(i==250)return 0XFF;//错误的字符串.
 	for(i=0;i<5;i++)//得到后缀名
	{
		fname--;
		if(*fname=='.')
		{
			fname++;
			attr=fname;
			break;
		}
  	}
	strcpy((char *)tbuf,(const char*)attr);//copy
 	for(i=0;i<4;i++)tbuf[i]=char_upper(tbuf[i]);//全部变为大写 
	for(i=0;i<6;i++)
	{
		for(j=0;j<13;j++)
		{
			if(*FILE_TYPE_TBL[i][j]==0)break;//此组已经没有可对比的成员了.
			if(strcmp((const char *)FILE_TYPE_TBL[i][j],(const char *)tbuf)==0)//找到了
			{
				return (i<<4)|j;
			}
		}
	}
	return 0XFF;//没找到		 			   
}	 


//得到磁盘剩余容量
//drv:磁盘编号("0:"/"1:")
//total:总容量	 （单位KB）
//free:剩余容量	 （单位KB）
//返回值:0,正常.其他,错误代码
FRESULT exf_getfree(char *drv,u32 *total,u32 *free)
{
	FATFS *fs1;
	FRESULT res;
    DWORD fre_clust=0, fre_sect=0, tot_sect=0;
    //得到磁盘信息及空闲簇数量
    res = f_getfree((const TCHAR*)drv, &fre_clust, &fs1);
    if(res==0)
	{											   
	    tot_sect=(fs1->n_fatent-2)*fs1->csize;	//得到总扇区数
	    fre_sect=fre_clust*fs1->csize;			//得到空闲扇区数	   
#if _MAX_SS!=512				  				//扇区大小不是512字节,则转换为512字节
		tot_sect*=fs1->ssize/512;
		fre_sect*=fs1->ssize/512;
#endif	  
		*total=tot_sect>>1;	//单位为KB
		*free=fre_sect>>1;	//单位为KB 
 	}
	return res;
}		   


//遍历文件，显示路径下所有文件
FRESULT f_scan_files (char* path )       // Start node to be scanned (also used as work area) 
{
    FRESULT res;
    FILINFO fno;
    DIR dir;
    int i;
    char *fn;   // This function is assuming non-Unicode cfg. 
#if _USE_LFN
    static char lfn[_MAX_LFN + 1];
    fno.lfname = lfn;
    fno.lfsize = sizeof(lfn);
#endif
    res = f_opendir(&dir, path);                       // Open the directory 
    if (res == FR_OK) {
        i = strlen(path);
        for (;;) {
            res = f_readdir(&dir, &fno);                   // Read a directory item 
            if (res != FR_OK || fno.fname[0] == 0) break;  // Break on error or end of dir 
            if (fno.fname[0] == '.') continue;             // Ignore dot entry 
#if _USE_LFN
            fn = *fno.lfname ? fno.lfname : fno.fname;
#else
            fn = fno.fname;
#endif
            if (fno.fattrib & AM_DIR) {                    // It is a directory 
                sprintf(&path[i], "/%s", fn);
				printf("scan file - %s\n\r",path);
                res = f_scan_files(path);
                if (res != FR_OK) break;
                path[i] = 0;
            } else {                                       // It is a file. 
                printf("scan file - %s/%s\n\r", path, fn);
            }
        }
    }else{
		printf("scan files error : %d\n\r",res);
	}

    return res;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

