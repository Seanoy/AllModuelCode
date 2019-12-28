#include "string.h"
#include "exfuns.h"
#include "malloc.h"
#include "usart.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//FATFS ��չ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/18
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

#define SD_CARD		0	//SD��,���Ϊ0
#define U_CARD		1	//U�̣����Ϊ1     
#define EX_FLASH 	2	//�ⲿflash,���Ϊ2

//�ļ������б�
const u8 *FILE_TYPE_TBL[6][13]=
{
	{"BIN"},			//BIN�ļ�
	{"LRC"},			//LRC�ļ�
	{"NES"},			//NES�ļ�
	{"TXT","C","H"},	//�ı��ļ�
	{"MP1","MP2","MP3","MP4","M4A","3GP","3G2","OGG","ACC","WMA","WAV","MID","FLAC"},//�����ļ�
	{"BMP","JPG","JPEG","GIF"},//ͼƬ�ļ�
};
///////////////////////////////�����ļ���,ʹ��malloc��ʱ��////////////////////////////////////////////
FATFS 	*fs[3];  		//�߼����̹�����.	 
FIL 	*file1;	  		//�ļ�1
FIL 	*file2;	  		//�ļ�2.
UINT 	br,bw;			//��д����
FILINFO fileinfo;		//�ļ���Ϣ
DIR 	dir;  			//Ŀ¼
FRESULT res;			//����ֵ

u8 *fatbuf;			//SD�����ݻ�����
///////////////////////////////////////////////////////////////////////////////////////
//Ϊexfuns�����ڴ�
//����ֵ:0,�ɹ�
//1,ʧ��
u8 exfuns_init(void)
{
	fs[0]=(FATFS*)mymalloc(SRAMIN,sizeof(FATFS));	//Ϊ����0�����������ڴ�	
	fs[1]=(FATFS*)mymalloc(SRAMIN,sizeof(FATFS));	//Ϊ����1�����������ڴ�
	file1=(FIL*)mymalloc(SRAMIN,sizeof(FIL));		//Ϊfile�����ڴ�
	file2=(FIL*)mymalloc(SRAMIN,sizeof(FIL));		//Ϊftemp�����ڴ�
	fatbuf=(u8*)mymalloc(SRAMIN,512);				//Ϊfatbuf�����ڴ�
	if(fs[0]&&fs[1]&&file1&&file2&&fatbuf)return 0;  //������һ��ʧ��,��ʧ��.
	else return 1;	
}

//��Сд��ĸתΪ��д��ĸ,���������,�򱣳ֲ���.
u8 char_upper(u8 c)
{
	if(c<'A')return c;//����,���ֲ���.
	if(c>='a')return c-0x20;//��Ϊ��д.
	else return c;//��д,���ֲ���
}	      

//�����ļ�������
//fname:�ļ���
//����ֵ:0XFF,��ʾ�޷�ʶ����ļ����ͱ��.
//		 ����,����λ��ʾ��������,����λ��ʾ����С��.
u8 f_typetell(u8 *fname)
{
	u8 tbuf[5];
	u8 *attr='\0';//��׺��
	u8 i=0,j;
	while(i<250)
	{
		i++;
		if(*fname=='\0')break;//ƫ�Ƶ��������.
		fname++;
	}
	if(i==250)return 0XFF;//������ַ���.
 	for(i=0;i<5;i++)//�õ���׺��
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
 	for(i=0;i<4;i++)tbuf[i]=char_upper(tbuf[i]);//ȫ����Ϊ��д 
	for(i=0;i<6;i++)
	{
		for(j=0;j<13;j++)
		{
			if(*FILE_TYPE_TBL[i][j]==0)break;//�����Ѿ�û�пɶԱȵĳ�Ա��.
			if(strcmp((const char *)FILE_TYPE_TBL[i][j],(const char *)tbuf)==0)//�ҵ���
			{
				return (i<<4)|j;
			}
		}
	}
	return 0XFF;//û�ҵ�		 			   
}	 


//�õ�����ʣ������
//drv:���̱��("0:"/"1:")
//total:������	 ����λKB��
//free:ʣ������	 ����λKB��
//����ֵ:0,����.����,�������
FRESULT exf_getfree(char *drv,u32 *total,u32 *free)
{
	FATFS *fs1;
	FRESULT res;
    DWORD fre_clust=0, fre_sect=0, tot_sect=0;
    //�õ�������Ϣ�����д�����
    res = f_getfree((const TCHAR*)drv, &fre_clust, &fs1);
    if(res==0)
	{											   
	    tot_sect=(fs1->n_fatent-2)*fs1->csize;	//�õ���������
	    fre_sect=fre_clust*fs1->csize;			//�õ�����������	   
#if _MAX_SS!=512				  				//������С����512�ֽ�,��ת��Ϊ512�ֽ�
		tot_sect*=fs1->ssize/512;
		fre_sect*=fs1->ssize/512;
#endif	  
		*total=tot_sect>>1;	//��λΪKB
		*free=fre_sect>>1;	//��λΪKB 
 	}
	return res;
}		   


//�����ļ�����ʾ·���������ļ�
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

