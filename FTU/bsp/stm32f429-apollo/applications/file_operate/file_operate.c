/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      file_operate.c
  * @brief:     The protect control logic.
  * @version:   V1.0.0
  * @author:    J.Lee
  * @date:      2017.12.19
  * @update:    
  */
  
/* INCLUDE FILES -------------------------------------------------------------*/
#include "file_operate.h"
#include "string.h"
#include "mini_xml.h"
#include "common_data.h"
#include "wave_recording.h"
#include <dfs_posix.h>

#include "point_table_config.h"
#include "drv_wdg.h"
	
	
#include "JsonFileOperation.h"

/* PUBLIC VARIABLES ----------------------------------------------------------*/


/* PRIVATE VARIABLES ---------------------------------------------------------*/
static int MyFile; /* File object */
static int MyFile_SOE; /* File object */
static int MyFile_CO; /* File object */
static struct stat FileSta;
static char content[512];
static char FileName[255]; /* File Name */
static char DirName[255]; /* Dir Name */
static int failnum = 0;
static uint8_t temp_array[256];

static struct DIR_ARRAY *Dir_Array;// __attribute__((at(SDRAM_ADDR_FILE)));
static struct DIR_ARRAY *Dir_ArrayClear;// __attribute__((at(0x00050000 + SDRAM_ADDR_FILE)));
static struct READ_DIR Read_Dir[DEV_MAX_NUM];
static struct READ_FILE Read_File[DEV_MAX_NUM];
static struct WRITE_FILE Write_File[DEV_MAX_NUM];
	
/* PRIVATE FUNCTION PROTOTYPES -----------------------------------------------*/
/**
  * @brief : Scan_Files.
  * @param : [path]
  * @param : [path_out]
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
DIR * Scan_Files (
    char* path,        /* Start node to be scanned (***also used as work area***) */
		struct DIR_ARRAY* path_out
)
{
    DIR *dir;
    DIR *dirtemp;
    unsigned int i;
    struct dirent *fno;

    dir = opendir(path);                       /* Open the directory */
    if (dir != RT_NULL) {
        for (;;) {
            fno = readdir(dir);                   /* Read a directory item */
            if (fno == RT_NULL || fno->d_namlen == 0) break;  /* Break on error or end of dir */
            if (fno->d_type == FT_DIRECTORY) {                    /* It is a directory */
                i = strlen(path);
                sprintf(&path[i], "/%s", fno->d_name);
                dirtemp = Scan_Files(path,path_out);                    /* Enter the directory */
                if (dirtemp == RT_NULL) break;
                path[i] = 0;
            } else {                                       /* It is a file. */
                sprintf((*path_out).Array[(*path_out).num], "%s/%s\n", path, fno->d_name);
				(*path_out).num++;
            }
        }
        closedir(dir);
    }

    return dir;
}

/**
  * @brief : TIME_TO_STR.
  * @param : [content]
  * @param : [time]
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
void TIME_TO_STR(char *content,struct CP56Time2a_t time)
{ 
	char tempstr[255];
	
	sprintf(tempstr,"%02d",(time.year&0x7f));
	strcpy(content,tempstr);
	sprintf(tempstr,"%02d",(time.month&0x0f));
	strcat(content,tempstr);
	sprintf(tempstr,"%02d",(time.dayofWeek&0x1f));
	strcat(content,tempstr);
	strcat(content,"_");
	sprintf(tempstr,"%02d",(time.hour&0x1f));
	strcat(content,tempstr);
	sprintf(tempstr,"%02d",(time.minute&0x3f));
	strcat(content,tempstr);
	sprintf(tempstr,"%02d",(time.msecondL|(time.msecondH<<8))/1000);
	strcat(content,tempstr);
	strcat(content,"_");
	sprintf(tempstr,"%03d",(time.msecondL|(time.msecondH<<8))%1000);
	strcat(content,tempstr);
} 

/**
  * @brief : file_Dir_Clear.
  * @param : [DIR]
  * @param : [maxnum]
  * @param : [cmpsize]
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
void file_Dir_Clear(char *dir,uint16_t maxnum,uint16_t cmpsize)
{ 
	uint8_t i;
	char old_file[255];
	
	memset(Dir_ArrayClear,0,sizeof(struct DIR_ARRAY));
	memset(old_file,0,sizeof old_file);
	Scan_Files(dir, Dir_ArrayClear);
	
    if(Dir_ArrayClear->num != 0)
    {
        for(i=0;(*Dir_ArrayClear->Array[i] != 0);i++)
        {
            if((*old_file == 0)||(strcmp(&old_file[cmpsize],&Dir_ArrayClear->Array[i][cmpsize]) > 0))
            {
                strcpy(old_file,Dir_ArrayClear->Array[i]);
            }      
        }
        if(i>=(maxnum-1))
        {
            unlink(old_file);
            FILE_PRINTF("f_unlink old_file \r\n"); 
        }
    }
}     

/**
  * @brief : CreatDoc_FEVENT.
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
void CreatDoc_FEVENT(void)
{ 
	char tabs = 0;

	memset(DirName,0,sizeof(DirName));	
	strcpy(DirName,"/sojo");
	strcat(DirName,"/HISTORY/FEVENT");//建立FEVENT文件目录
	mkdir(DirName,0);//建立目录
	
	strcpy(FileName,"/sojo");
	strcat(FileName,"/HISTORY/FEVENT");
	strcat(FileName,"/fevent.xml");
	
	MyFile = open(FileName, O_RDWR | O_CREAT, 0);
	
	strcpy(content,"xml version=\"1.0\" encoding=\"UTF-8\"");
	put_head(content);
	write(MyFile, content, strlen(content));
	
	strcpy(content,"DataFile");
	tag_start(content,&tabs);
	write(MyFile, content, strlen(content));
	
	strcpy(content,"Header fileType=\"FEVENT\" fileVer=\"1.00\" devName=\"SHFD2000\" ");
	tag_value(content,&tabs);
	write(MyFile, content, strlen(content));
	
	strcpy(content,"DataRec num=\"0000\"");
	tag_start(content,&tabs);
	write(MyFile, content, strlen(content));
	
	strcpy(content,"DataRec");
	tag_end(content,&tabs);
	write(MyFile, content, strlen(content));
	
	strcpy(content,"DataFile");
	tag_end(content,&tabs);
	write(MyFile, content, strlen(content));
	
	close(MyFile);
}  

/**
  * @brief : CreatDoc_SOE.
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
void CreatDoc_SOE(void)
{ 
	char tabs = 0;

	memset(DirName,0,sizeof(DirName));	
	strcpy(DirName,"/sojo");
	strcat(DirName,"/HISTORY/SOE");//建立SOE文件目录
	mkdir(DirName,0);//建立目录
	
	strcpy(FileName,"/sojo");
	strcat(FileName,"/HISTORY/SOE");
	strcat(FileName,"/soe.xml");
	
    MyFile = open(FileName, O_RDWR | O_CREAT, 0);
	
	strcpy(content,"xml version=\"1.0\" encoding=\"UTF-8\"");
	put_head(content);
	write(MyFile, content, strlen(content));
	
	strcpy(content,"DataFile");
	tag_start(content,&tabs);
	write(MyFile, content, strlen(content));
	
	strcpy(content,"Header fileType=\"SOE\" fileVer=\"1.00\" devName=\"SHFD2000\" ");
	tag_value(content,&tabs);
	write(MyFile, content, strlen(content));
	
	strcpy(content,"DataRec num=\"0000\"");
	tag_start(content,&tabs);
	write(MyFile, content, strlen(content));
	
	strcpy(content,"DataRec");
	tag_end(content,&tabs);
	write(MyFile, content, strlen(content));
	
	strcpy(content,"DataFile");
	tag_end(content,&tabs);
	write(MyFile, content, strlen(content));
	
	close(MyFile);
}       

/**
  * @brief : CreatDoc_CO.
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
void CreatDoc_CO(void)
{ 
	char tabs = 0;
	
	memset(DirName,0,sizeof(DirName));
	strcpy(DirName,"/sojo");
	strcat(DirName,"/HISTORY/CO");//建立SOE文件目录
	mkdir(DirName,0);//建立目录
	
	strcpy(FileName,"/sojo");
	strcat(FileName,"/HISTORY/CO");
	strcat(FileName,"/co.xml");
	
    MyFile = open(FileName, O_RDWR | O_CREAT, 0);
	
	strcpy(content,"xml version=\"1.0\" encoding=\"UTF-8\"");
	put_head(content);
    write(MyFile, content, strlen(content));
	
	strcpy(content,"DataFile");
	tag_start(content,&tabs);
	write(MyFile, content, strlen(content));
	
	strcpy(content,"Header fileType=\"CO\" fileVer=\"1.00\" devName=\"SHFD2000\" ");
	tag_value(content,&tabs);
	write(MyFile, content, strlen(content));
	
	strcpy(content,"DataRec num=\"0000\"");
	tag_start(content,&tabs);
	write(MyFile, content, strlen(content));
	
	strcpy(content,"DataRec");
	tag_end(content,&tabs);
	write(MyFile, content, strlen(content));
	
	strcpy(content,"DataFile");
	tag_end(content,&tabs);
	write(MyFile, content, strlen(content));
	
	close(MyFile);
} 

/**
  * @brief : file_operate_DirRead.
  * @param : [dev]
  * @param : [pbuf]
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
uint8_t file_operate_DirRead(uint8_t dev,uint8_t *pbuf)
{//LENTH/Lock_ID/TypeID/VSQ/COT_L/COT_H/PubAddr_L/PubAddr_H/InfoAddr_L/InfoAddr_H/sty/Array
    struct CP56Time2a_t time;
	int j=0,txnum=0,size;
    unsigned int *i;

    i = &Read_Dir[dev].dir_i;
    
    pbuf[1] = dev;
    
    txnum = 2;
    pbuf[txnum++] = 210;//TypeID
    pbuf[txnum++] = 0;
    pbuf[txnum++] = 5;
    pbuf[txnum++] = 0;
    pbuf[txnum++] = 1;
    pbuf[txnum++] = 0;
    txnum = 8;
    pbuf[txnum++] = 0;
    pbuf[txnum++] = 0;
    pbuf[txnum++] = 1;
        
    while(1)
    {
        if(*Dir_Array->Array[*i] != 0)
        {
            //填表
            if(j==0)
            {txnum = 19;}
            Dir_Array->Array[*i][strlen(Dir_Array->Array[*i])-1] = '\0';
            pbuf[txnum++] = strlen(Dir_Array->Array[*i]);
            memcpy(&pbuf[txnum],Dir_Array->Array[*i],strlen(Dir_Array->Array[*i]));
            txnum += strlen(Dir_Array->Array[*i]);
            pbuf[txnum++] = 0;
            MyFile = open(Dir_Array->Array[*i], O_RDONLY, 0);
            if(MyFile == -1)
            {return(0);}
            fstat(MyFile, &FileSta);
            size = FileSta.st_size;
            close(MyFile);
            pbuf[txnum++] = size&0xff;
            pbuf[txnum++] = (size>>8)&0xff;
            pbuf[txnum++] = (size>>16)&0xff;
            pbuf[txnum++] = (size>>24)&0xff;
            if(strncmp(&Dir_Array->Array[*i][strlen(Dir_Array->Array[*i])-8],"20",2))
            {
                DBReadSystemTime(&time);
                pbuf[txnum++] = time.msecondL;
                pbuf[txnum++] = time.msecondH;
                pbuf[txnum++] = time.minute;
                pbuf[txnum++] = time.hour;
                pbuf[txnum++] = time.dayofWeek;
                pbuf[txnum++] = time.month;
                pbuf[txnum++] = time.year;
            }
            else
            {
                pbuf[txnum++] = 0;
                pbuf[txnum++] = 0;
                pbuf[txnum++] = 0;
                pbuf[txnum++] = 0;
                pbuf[txnum++] = (Dir_Array->Array[*i][strlen(Dir_Array->Array[*i])-6]-30)*10 + (Dir_Array->Array[*i][strlen(Dir_Array->Array[*i])-5]-30);
                pbuf[txnum++] = (Dir_Array->Array[*i][strlen(Dir_Array->Array[*i])-4]-30)*10 + (Dir_Array->Array[*i][strlen(Dir_Array->Array[*i])-3]-30);;
                pbuf[txnum++] = (Dir_Array->Array[*i][strlen(Dir_Array->Array[*i])-2]-30)*10 + (Dir_Array->Array[*i][strlen(Dir_Array->Array[*i])-1]-30);;
            }
            //j>6,发送j=0
            if(++j>2)
            {
                pbuf[18] = j;
                pbuf[0] = txnum;
                txnum = 11;
                pbuf[1] = dev;
                pbuf[txnum++] = 2;
                pbuf[txnum++] = 0;
                memcpy(&pbuf[txnum],&Read_Dir[dev].dirid,sizeof(Read_Dir[dev].dirid));
                txnum += sizeof(Read_Dir[dev].dirid);
                if(*Dir_Array->Array[*i+1] != 0)
                {pbuf[txnum++] = 1;}
                else
                {pbuf[txnum++] = 0;}
                j=0;
                ++(*i);
                return(1);
            }
            ++(*i);
        }
        else
        {
            //如果可发送目录个数为0，发送
            if(*i==0)
            {
                txnum = 11;
                pbuf[1] = (pbuf[1]>>4);
                pbuf[txnum++] = 2;
                pbuf[txnum++] = 0;
                memcpy(&pbuf[txnum],&Read_Dir[dev].dirid,sizeof(Read_Dir[dev].dirid));
                txnum += sizeof(Read_Dir[dev].dirid);
                pbuf[txnum++] = 0;
                pbuf[txnum++] = 0;
                pbuf[0] = txnum;
                return(0);
            }
            //如果j！=0发送已打包目录数据
            if(j!=0)
            {
                pbuf[18] = j;
                pbuf[0] = txnum;
                txnum = 11;
                pbuf[1] = dev;
                pbuf[txnum++] = 2;
                pbuf[txnum++] = 0;
                memcpy(&pbuf[txnum],&Read_Dir[dev].dirid,sizeof(Read_Dir[dev].dirid));
                txnum += sizeof(Read_Dir[dev].dirid);
                pbuf[txnum++] = 0;
                j=0;
                return(0);
            }
            return(0);
        }
    }
}

/**
  * @brief : file_operate_DirCall.
  * @param : [dev]
  * @param : [pbuf]
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
uint8_t file_operate_DirCall(uint8_t dev,uint8_t *pbuf)
{//LENTH/Lock_ID/TypeID/VSQ/COT_L/COT_H/PubAddr_L/PubAddr_H/InfoAddr_L/InfoAddr_H/sty/Array
	int txnum=0;
	
	memcpy(temp_array, pbuf,pbuf[0]);
	

	if(pbuf[16])
	{
        memset(DirName,0,sizeof(DirName));
		memcpy(DirName, &pbuf[17],pbuf[16]);//建立查询目录
		DirName[pbuf[16]] = '\0';
	}
	else
	{
		memset(DirName,0,sizeof(DirName));
		strcpy(DirName,"/sojo");
		strcat(DirName,"/HISTORY");//建立查询目录	
	}
	memset(Dir_Array,0,sizeof(struct DIR_ARRAY));
	Scan_Files(DirName, Dir_Array);
	if(Dir_Array->num == 0)//失败
	{
		temp_array[1] = dev;
        txnum = 8;
        temp_array[txnum++] = 0;
		temp_array[txnum++] = 0;
        temp_array[txnum++] = 1;
		txnum = 11;
		temp_array[txnum++] = 2;
		temp_array[txnum++] = 1;
		temp_array[txnum++] = pbuf[12];
		temp_array[txnum++] = pbuf[13];
		temp_array[txnum++] = pbuf[14];
		temp_array[txnum++] = pbuf[15];
		temp_array[txnum++] = 0;
		temp_array[txnum++] = 0;
		temp_array[0] = txnum;
        memcpy(pbuf,temp_array,temp_array[0]);
        return(0);
	}
	else//成功
	{
        Read_Dir[dev].dir_i = 0;
        memcpy(&Read_Dir[dev].dirid,&pbuf[12],sizeof(Read_Dir[dev].dirid));
        return(file_operate_DirRead(dev,pbuf));
	}
}

/**
  * @brief : file_operate_ReadFile.
  * @param : [dev]
  * @param : [pbuf]
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
uint8_t file_operate_ReadFile(uint8_t dev,uint8_t *pbuf)
{//LENTH/Lock_ID/TypeID/VSQ/COT_L/COT_H/PubAddr_L/PubAddr_H/InfoAddr_L/InfoAddr_H/sty/Array
	int i,txnum;
	int eachoffset = 200;
	int bytesread;
	
    pbuf[1] = (dev);
    MyFile = open(Read_File[dev].filename, O_WRONLY, 0);
    lseek(MyFile, Read_File[dev].offset, SEEK_SET);
    bytesread = read(MyFile, content, eachoffset);
    close(MyFile);
    if(MyFile == -1)
    {return(0);}
    if(bytesread)
    {
        txnum = 2;
        pbuf[txnum++] = 210;//TypeID
        pbuf[txnum++] = 0;
        pbuf[txnum++] = 5;
        pbuf[txnum++] = 0;
        pbuf[txnum++] = 1;
        pbuf[txnum++] = 0;
        txnum = 8;
        pbuf[txnum++] = 0;
        pbuf[txnum++] = 0;
        pbuf[txnum++] = 2;
        txnum = 11;
        pbuf[txnum++] = 5;				
        pbuf[txnum++] = Read_File[dev].fileid&0xff;
        pbuf[txnum++] = (Read_File[dev].fileid>>8)&0xff;
        pbuf[txnum++] = (Read_File[dev].fileid>>16)&0xff;
        pbuf[txnum++] = (Read_File[dev].fileid>>24)&0xff;
        pbuf[txnum++] = Read_File[dev].offset&0xff;
        pbuf[txnum++] = (Read_File[dev].offset>>8)&0xff;
        pbuf[txnum++] = (Read_File[dev].offset>>16)&0xff;
        pbuf[txnum++] = (Read_File[dev].offset>>24)&0xff;
        if((bytesread < eachoffset)|(Read_File[dev].filesize == eachoffset ))
        {pbuf[txnum++] = 0;}
        else
        {pbuf[txnum++] = 1;}
        memcpy(&pbuf[txnum],content,bytesread);
        pbuf[txnum+bytesread] = 0;
        for(i=0;i<bytesread;i++)
        {pbuf[txnum+bytesread] = (pbuf[txnum+bytesread] + pbuf[txnum+i])&0xff;}	
        pbuf[0] = txnum+bytesread+1;
        Read_File[dev].offset += eachoffset;
        Read_File[dev].filesize -= eachoffset;
        if((bytesread < eachoffset)|(Read_File[dev].filesize == eachoffset ))
        {memset(&Read_File,0,sizeof(Read_File));}
        if((bytesread < eachoffset)|(Read_File[dev].filesize == eachoffset ))
        {return(0);}
        else
        {return(1);}    
    }
    else
    {
        {memset(&Read_File,0,sizeof(Read_File));}
        return(0);
    }
}

/**
  * @brief : file_operate_ReadFileAct.
  * @param : [dev]
  * @param : [pbuf]
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
uint8_t file_operate_ReadFileAct(uint8_t dev,uint8_t *pbuf)
{//LENTH/Lock_ID/TypeID/VSQ/COT_L/COT_H/PubAddr_L/PubAddr_H/InfoAddr_L/InfoAddr_H/sty/Array
	int i,txnum,filesize;
	int fileid;
	
	memcpy(temp_array, pbuf,pbuf[0]);
	
	switch(pbuf[11])
	{
		//读文件激活
		case 3:
            temp_array[4] = 7;
			temp_array[1] = dev;
			memcpy(FileName,&pbuf[13],pbuf[12]);
			FileName[pbuf[12]] = '\0';
            MyFile = open(FileName, O_WRONLY, 0);
            fstat(MyFile, &FileSta);
			filesize = FileSta.st_size;
			close(MyFile);
			txnum = 11;
			temp_array[txnum++] = 4;
			if(MyFile == -1)
			{temp_array[txnum++] = 1;}
			else
			{temp_array[txnum++] = 0;}	
			temp_array[txnum++] = strlen(FileName);
			memcpy(&temp_array[txnum],FileName,strlen(FileName));
			txnum += strlen(FileName);
			for(i=0;i<strlen(FileName);i++)
			{fileid += FileName[i];}
			temp_array[txnum++] = fileid&0xff;
			temp_array[txnum++] = (fileid>>8)&0xff;
			temp_array[txnum++] = (fileid>>16)&0xff;
			temp_array[txnum++] = (fileid>>24)&0xff;
			temp_array[txnum++] = filesize&0xff;
			temp_array[txnum++] = (filesize>>8)&0xff;
			temp_array[txnum++] = (filesize>>16)&0xff;
			temp_array[txnum++] = (filesize>>24)&0xff;
			temp_array[0] = txnum;
            memcpy(pbuf, temp_array,temp_array[0]);
			if(!temp_array[12])//激活成功
			{
                memset(&Read_File,0,sizeof(Read_File));
				Read_File[dev].fileid = fileid;
				Read_File[dev].filesize = filesize;
				memcpy(Read_File[dev].filename,FileName,strlen(FileName)+1);
                return(1);
			}
			return(0);
		//读文件确认
		case 6:
			memset(&Read_File[dev],0,sizeof(Read_File));
            return(0);
		default:
			break;
	}
	
	return(1);
}

/**
  * @brief : ReadDoc_Record.
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
void ReadDoc_Record(void)
{ 	
    strcpy(FileName,"/sojo");
    strcat(FileName,"/ConfigurationSet.cfg");
	
    MyFile = open(FileName, O_WRONLY, 0);
    
    if (MyFile == -1)
    {
        FILE_PRINTF("NO ConfigurationSet.cfg\n", MyFile);
        return;
    }
	
    read(MyFile, &g_ConfigurationSetDB, sizeof(g_ConfigurationSetDB));
   
    FILE_PRINTF("f_read ConfigurationSet.cfg\n", MyFile);
    
//存储FRAM语句 	
    if(g_ConfigurationSetDB.ID_Value[0] != 0)
    {
        rt_multi_common_data_fram_record_write(CFG_RECODE, (uint8_t *)&g_ConfigurationSetDB, sizeof(g_ConfigurationSetDB));
    }
    else
    {
        rt_multi_common_data_fram_record_write(CFG_RECODE, (uint8_t *)&g_ConfigurationSetDB, sizeof(g_ConfigurationSetDB) - sizeof(g_ConfigurationSetDB.ID_Value));
    }
    
	close(MyFile);
}

/**
  * @brief : file_operate_WriteFileAct.
  * @return: none
  * @param : [dev]
  * @param : [pbuf]
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
void file_operate_WriteFileAct(uint8_t dev,uint8_t *pbuf)
{//LENTH/Lock_ID/TypeID/VSQ/COT_L/COT_H/PubAddr_L/PubAddr_H/InfoAddr_L/InfoAddr_H/sty/Array
	
    unsigned long i;
    unsigned long crc;
	
	memcpy(temp_array, pbuf,pbuf[0]);
	
	switch(pbuf[11])
	{
		//写文件激活
		case 7:
            temp_array[4] = 7;
			temp_array[1] = (pbuf[1]>>4);
			temp_array[11] = 8;
			temp_array[12] = 0;
            memcpy(&temp_array[13],&pbuf[12],sizeof(temp_array)-12);
			temp_array[0] = pbuf[0]+1;        
            memset(&Write_File,0,sizeof(Write_File));
            Write_File[dev].fileid = *(unsigned long *)&pbuf[11 + 2 + pbuf[12]];//记录id
            Write_File[dev].filesize = *(unsigned long *)&pbuf[11 + 6 + pbuf[12]];//文件长度
            for(i=0;i<pbuf[12];i++)
            {
                if(!strncmp("ConfigurationSet",(char *)&pbuf[i+13],sizeof("ConfigurationSet") - 1))
                {
                    strcpy(Read_File[dev].filename,"/sojo");
                    strcat(Read_File[dev].filename,"/ConfigurationSet.cfg");
                    break;   
                }       
                if(!strncmp("UpdateProgram",(char *)&pbuf[i+13],sizeof("UpdateProgram") - 1))
                {
                    strcpy(Read_File[dev].filename,"/sojo");
                    strcat(Read_File[dev].filename,"/UpdateProgram.bin");
                    memcpy((unsigned long *)SDRAM_ADDR_UPDATE,(unsigned long *)&Write_File[dev].filesize,sizeof(unsigned long));
                    FILE_PRINTF("start recive UpdateProgram.bin,size:%x\n",*(unsigned long *)SDRAM_ADDR_UPDATE);
                    break;   
                }                   
            }
            if(i>=pbuf[12])
            {
                temp_array[12] = 2;
                memset(&Write_File,0,sizeof(Write_File));
            }
			break;
		//写文件确认
		case 9:
			temp_array[4] = 7;
			temp_array[0] = 21;
			temp_array[11] = 10;//写文件数据传输确认
			for(crc=0,i=0;i<pbuf[0]-1-21;i++)
			{
				crc += pbuf[i+21];
			}
			if((crc&0xff) != pbuf[pbuf[0]-1])
			{
				temp_array[20] = 2;
			}
			else if(Write_File[dev].fileid != *(unsigned long *)&pbuf[11 + 1])
			{
				temp_array[20] = 4;
			}
			else if(Write_File[dev].filesize < (i + *(unsigned long *)&pbuf[11 + 5]))
			{
				temp_array[20] = 3;
			}
			else
			{
				temp_array[20] = 0;
                if(strcmp(Read_File[dev].filename,"/sojo/UpdateProgram.bin") == 0)//在线更新
                {     
                    memcpy((unsigned long *)(SDRAM_ADDR_UPDATE + 4 + *(unsigned long *)&pbuf[11 + 5]),&pbuf[21],i);                    
                }
                else
                {
                    MyFile = open(Read_File[dev].filename, O_WRONLY, 0);
                    lseek(MyFile, *(unsigned long *)&pbuf[11 + 5], SEEK_SET);
                    write(MyFile,&pbuf[21], i);
                    close(MyFile);               
                }

				if(pbuf[20] ==0)
                {
                    FILE_PRINTF("recive success\n",*(unsigned long *)SDRAM_ADDR_UPDATE);
                    if(strcmp(Read_File[dev].filename,"/sojo/ConfigurationSet.cfg") == 0)//读配置文件
                    {
                        ReadDoc_Record();
                    }                            
                }
                else
                {
                    temp_array[0] = 0;//不回复数据
                }               
			}
			break;
		default:
			break;
	}
    memcpy(pbuf,temp_array,sizeof(temp_array));
}

/**
  * @brief : CreatDoc_Record.
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
void CreatDoc_Record(void)
{ 
	memset(FileName,0,sizeof(DirName));
	strcpy(FileName,"/sojo");
	strcat(FileName,"/ConfigurationSet.cfg");
	
	MyFile = open(FileName, O_RDWR | O_CREAT, 0);
	
    write(MyFile,&g_ConfigurationSetDB, sizeof(g_ConfigurationSetDB));
   
    FILE_PRINTF("f_write ConfigurationSet.cfg\n", MyFile );
    
	close(MyFile);
}

/**
  * @brief : file_operate_Format.
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
void file_operate_Format(void)
{ 
//    if(dfs_mkfs("elm","W25Q256FV") != -1) 
    if (dfs_mkfs("elm", RT_SPI_FLASH_NAME) != -1) 
    {
        FILE_PRINTF("spi flash mk to /spi !\n");
    }
    else
    {
        FILE_PRINTF("spi flash mk to /spi failed!\n");
    }
    
//    dfs_mount("W25Q256FV", "/sojo", "elm", 0, 0);
    dfs_mount(RT_SPI_FLASH_NAME, "/sojo", "elm", 0, 0);
    
    memset(DirName,0,sizeof(DirName));
    strcpy(DirName,"/sojo");
    strcat(DirName,"/HISTORY");//建立历史文件目录
    mkdir(DirName,0);;//建立目录
    
    memset(DirName,0,sizeof(DirName));
    strcpy(DirName,"/sojo");
    strcat(DirName,"/HISTORY/COMTRADE");//建立SOE文件目录
    mkdir(DirName,0);//建立目录
    
    memset(&g_FlagDB.fatfs_co,0,sizeof(g_FlagDB.fatfs_co));
    memset(&g_FlagDB.fatfs_soe,0,sizeof(g_FlagDB.fatfs_soe));
    memset(&g_FlagDB.fatfs_fevent,0,sizeof(g_FlagDB.fatfs_fevent));
    memset(&g_FlagDB.fatfs_ulog,0,sizeof(g_FlagDB.fatfs_ulog));
    
    //创建CO
    CreatDoc_CO();
    //创建SOE
    CreatDoc_SOE();
    //创建FEVENT
    CreatDoc_FEVENT();
    
    //写入文件配置
    CreatDoc_Record();
    
//		//创建EXV目录
//		CreatDIR_EXV();
//		//创建FIXPT目录
//		CreatDIR_FIXPT();

}

/**
  * @brief : AddDoc_FEVNET.
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
uint8_t AddDoc_FEVNET(void)
{
	char tabs = 0;
	char tempstr[20];
    char i;
    
    if(g_FlagDB.queue_fevent.in != g_FlagDB.queue_fevent.out[FLASH_ID])
    {

        strcpy(FileName,"/sojo");
        strcat(FileName,"/HISTORY/FEVENT");
        strcat(FileName,"/fevent.xml");
        
        MyFile = open(FileName, O_RDWR, 0);
        if(MyFile == -1)
        {
            failnum++;
            FILE_PRINTF("/sojo/HISTORY/FEVENT/fevent.xml open failed!\n");
            return(1);
        }
        
        while(g_FlagDB.queue_fevent.in != g_FlagDB.queue_fevent.out[FLASH_ID])
        {
            tabs = 2;
            lseek(MyFile, 0x89+(0x34+26*10)*(g_FlagDB.fatfs_fevent.currentnum), SEEK_SET);
            strcpy(content,\
            "DI ioa=\"00001\" tm=\"160813_180000_011\" val=\"0\" ioa0=\"00001\" val0=\"000.0\" ioa1=\"00000\" val1=\"000.0\" ioa2=\"00000\" val2=\"000.0\" ioa3=\"00000\" val3=\"000.0\" ioa4=\"00000\" val4=\"000.0\" ioa5=\"00000\" val5=\"000.0\" ioa6=\"00000\" val6=\"000.0\" ioa7=\"00000\" val7=\"000.0\" ioa8=\"00000\" val8=\"000.0\" ioa9=\"00000\" val9=\"000.0\"");
            sprintf(tempstr,"%05d",g_FeventDB[g_FlagDB.queue_fevent.out[FLASH_ID]].yx[0].addr);
            memcpy((content+strlen("DI ioa=\"")),tempstr,5);
            sprintf(tempstr,"%01d",(g_FeventDB[g_FlagDB.queue_fevent.out[FLASH_ID]].yx[0].value- 1));
            memcpy((content+strlen("DI ioa=\"00001\" tm=\"160813_180000_011\" val=\"")),tempstr,1);
            TIME_TO_STR(tempstr,g_FeventDB[g_FlagDB.queue_fevent.out[FLASH_ID]].yx[0].time);
            memcpy((content+strlen("DI ioa=\"00001\" tm=\"")),tempstr,17);
            for(i=0;i<10;i++)
            {
                sprintf(tempstr,"%05d",g_FeventDB[g_FlagDB.queue_fevent.out[FLASH_ID]].yc[i].addr);
                memcpy((content+strlen("DI ioa=\"00001\" tm=\"160813_180000_011\" val=\"0\" ioa0=\"")+i*26),tempstr,5);
                sprintf(tempstr,"%0.4f",g_FeventDB[g_FlagDB.queue_fevent.out[FLASH_ID]].yc[i].value);
                memcpy((content+strlen("DI ioa=\"00001\" tm=\"160813_180000_011\" val=\"0\" ioa0=\"00001\" val0=\"")+i*26),tempstr,4);
            }
            tag_value(content,&tabs);
            write(MyFile,content, strlen(content));
            
            if(MyFile == -1)
            {
                failnum++;
                FILE_PRINTF("/sojo/HISTORY/FEVENT/fevent.xml write failed!\n");
                return(1);
            }
            if(++g_FlagDB.fatfs_fevent.fullnum >= FEVENT_FILE_MAXNUM)
            {
                g_FlagDB.fatfs_fevent.fullnum = FEVENT_FILE_MAXNUM;
            }
            
            if(++g_FlagDB.fatfs_fevent.currentnum >= FEVENT_FILE_MAXNUM)
            {
                g_FlagDB.fatfs_fevent.currentnum = 0;
                strcpy(content,"DataRec");
                tag_end(content,&tabs);
                write(MyFile,content, strlen(content));
                
                strcpy(content,"DataFile");
                tag_end(content,&tabs);
                write(MyFile,content, strlen(content));
            }
                
            if (++(g_FlagDB.queue_fevent.out[FLASH_ID]) >= FEVENT_MAX_NUM)
            {
                g_FlagDB.queue_fevent.out[FLASH_ID] = 0;
            }               
        }

        if(g_FlagDB.fatfs_fevent.fullnum < FEVENT_FILE_MAXNUM)
        {
            strcpy(content,"DataRec");
            tag_end(content,&tabs);
            write(MyFile,content, strlen(content));
            if(MyFile == -1)
            {
                failnum++;
                FILE_PRINTF("/sojo/HISTORY/FEVENT/fevent.xml write failed!\n");
                return(1);
            }
            
            strcpy(content,"DataFile");
            tag_end(content,&tabs);
            write(MyFile,content, strlen(content));
            if(MyFile == -1)
            {
                failnum++;
                FILE_PRINTF("/sojo/HISTORY/FEVENT/fevent.xml write failed!\n");
                return(1);
            }
        }
        
        tabs = 1;
        lseek(MyFile,0x73+1,SEEK_SET);
        strcpy(content,"DataRec num=\"0010\"");
        sprintf(tempstr,"%04d",g_FlagDB.fatfs_fevent.fullnum);
        memcpy((content+strlen("DataRec num=\"")),tempstr,4);
        tag_start(content,&tabs);
        write(MyFile,content, strlen(content));
        if(MyFile == -1)
        {
            failnum++;
            FILE_PRINTF("/sojo/HISTORY/FEVENT/fevent.xml write failed!\n");
            return(1);
        }
        FILE_PRINTF("Write FEVENT\r\n");	
        
        close(MyFile);
        
        failnum = 0;
    }
    
    return(0);
}

/**
  * @brief : AddDoc_SOE_CO.
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
uint8_t AddDoc_SOE_CO(void)
{
	char tabs = 0;
	char tempstr[20];
    char have_soe=0,have_co=0;
    
    if(g_FlagDB.queue_soe.in != g_FlagDB.queue_soe.out[FLASH_ID])
    {	
        strcpy(FileName,"/sojo");
        strcat(FileName,"/HISTORY/SOE");
        strcat(FileName,"/soe.xml");	
        MyFile_SOE = open(FileName, O_RDWR, 0);
        if(MyFile_SOE == -1)
        {
            failnum++;
            FILE_PRINTF("/sojo/HISTORY/SOE/soe.xml open failed!\n");
            return(1);
        }
     
        strcpy(FileName,"/sojo");
        strcat(FileName,"/HISTORY/CO");
        strcat(FileName,"/co.xml"); 
        MyFile_CO = open(FileName, O_RDWR, 0);        
        if(MyFile_CO == -1)
        {
            failnum++;
            FILE_PRINTF("/sojo/HISTORY/CO/co.xml open failed!\n");
            return(1);
        }
        
        while(g_FlagDB.queue_soe.in != g_FlagDB.queue_soe.out[FLASH_ID])
        {
            if(g_SOEDB[g_FlagDB.queue_soe.out[FLASH_ID]].addr >= REMOTE_START_ADDR)//操作记录
            {
                have_co = 1;
                tabs = 2;
                lseek(MyFile_CO, 0x85+0x49*(g_FlagDB.fatfs_co.currentnum), SEEK_SET);
                strcpy(content,"DI ioa=\"00001\" tm=\"160813_180000_011\" cmd=\"select success\" val=\"0\"");
                sprintf(tempstr,"%05d",g_SOEDB[g_FlagDB.queue_soe.out[FLASH_ID]].addr);
                memcpy((content+strlen("DI ioa=\"")),tempstr,5);
                if((g_SOEDB[g_FlagDB.queue_soe.out[FLASH_ID]].value)>=0x20)
                {
                    if(g_SOEDB[g_FlagDB.queue_soe.out[FLASH_ID]].value&0x10)
                    {
                        strcpy(tempstr,"cancle        ");
                    }
                    else
                    {
                        if(g_SOEDB[g_FlagDB.queue_soe.out[FLASH_ID]].value&0x08)
                        {strcpy(tempstr,"oper   ");}
                        else
                        {strcpy(tempstr,"select ");}
                        if(g_SOEDB[g_FlagDB.queue_soe.out[FLASH_ID]].value&0x04)
                        {strcat(tempstr,"success");}
                        else
                        {strcat(tempstr,"fail   ");}            
                    }
                }
                else
                {
                    {strcpy(tempstr,"oper   success");} 
                }
                memcpy((content+strlen("DI ioa=\"00001\" tm=\"160813_180000_011\" cmd=\"")),tempstr,14);
                sprintf(tempstr,"%01d",((g_SOEDB[g_FlagDB.queue_soe.out[FLASH_ID]].value >> 1)&0x01));
                memcpy((content+strlen("DI ioa=\"00001\" tm=\"160813_180000_011\" cmd=\"select success\" val=\"")),tempstr,1);
                TIME_TO_STR(tempstr,g_SOEDB[g_FlagDB.queue_soe.out[FLASH_ID]].time);
                memcpy((content+strlen("DI ioa=\"00001\" tm=\"")),tempstr,17);
                tag_value(content,&tabs);
                write(MyFile_CO,content, strlen(content));        
                if(MyFile_CO == -1)
                {
                    failnum++;
                    FILE_PRINTF("/sojo/HISTORY/CO/co.xml write failed!\n");
                    return(1);
                }
                
                if(++g_FlagDB.fatfs_co.fullnum >= CO_FILE_MAXNUM)
                {
                    g_FlagDB.fatfs_co.fullnum = CO_FILE_MAXNUM;
                }
                
                if(++g_FlagDB.fatfs_co.currentnum >= CO_FILE_MAXNUM)
                {
                    g_FlagDB.fatfs_co.currentnum = 0;
                    strcpy(content,"DataRec");
                    tag_end(content,&tabs);
                    write(MyFile_CO,content, strlen(content)); 
                    if(MyFile_CO == -1)
                    {
                        failnum++;
                        FILE_PRINTF("/sojo/HISTORY/CO/co.xml write failed!\n");
                        return(1);
                    }
                    
                    strcpy(content,"DataFile");
                    tag_end(content,&tabs);
                    write(MyFile_CO,content, strlen(content));
                    if(MyFile_CO == -1)
                    {
                        failnum++;
                        FILE_PRINTF("/sojo/HISTORY/CO/co.xml write failed!\n");
                        return(1);
                    }
                }     
            }           
            else//SOE
            {
                have_soe = 1;
                tabs = 2;
                lseek(MyFile_SOE, 0x86+0x34*(g_FlagDB.fatfs_soe.currentnum), SEEK_SET);
                strcpy(content,"DI ioa=\"00001\" tm=\"160813_180000_011\" val=\"0\"");
                sprintf(tempstr,"%05d",g_SOEDB[g_FlagDB.queue_soe.out[FLASH_ID]].addr);
                memcpy((content+strlen("DI ioa=\"")),tempstr,5);
                sprintf(tempstr,"%01d",(g_SOEDB[g_FlagDB.queue_soe.out[FLASH_ID]].value - 1));
                memcpy((content+strlen("DI ioa=\"00001\" tm=\"160813_180000_011\" val=\"")),tempstr,1);
                TIME_TO_STR(tempstr,g_SOEDB[g_FlagDB.queue_soe.out[FLASH_ID]].time);
                memcpy((content+strlen("DI ioa=\"00001\" tm=\"")),tempstr,17);
                tag_value(content,&tabs);
                write(MyFile_SOE,content, strlen(content));
                if(MyFile_SOE == -1)
                {
                    failnum++;
                    FILE_PRINTF("/sojo/HISTORY/CO/co.xml write failed!\n");
                    return(1);
                }
                
                if(++g_FlagDB.fatfs_soe.fullnum >= SOE_FILE_MAXNUM)
                {
                    g_FlagDB.fatfs_soe.fullnum = SOE_FILE_MAXNUM;
                }
                
                if(++g_FlagDB.fatfs_soe.currentnum >= SOE_FILE_MAXNUM)
                {
                    g_FlagDB.fatfs_soe.currentnum = 0;
                    strcpy(content,"DataRec");
                    tag_end(content,&tabs);
                    write(MyFile_SOE,content, strlen(content));
                    if(MyFile_SOE == -1)
                    {
                        failnum++;
                        FILE_PRINTF("/sojo/HISTORY/SOE/soe.xml write failed!\n");
                        return(1);
                    }
                    
                    strcpy(content,"DataFile");
                    tag_end(content,&tabs);
                    write(MyFile_SOE,content, strlen(content));
                    if(MyFile_SOE == -1)
                    {
                        failnum++;
                        FILE_PRINTF("/sojo/HISTORY/SOE/soe.xml write failed!\n");
                        return(1);
                    }
                }
            }
            if (++(g_FlagDB.queue_soe.out[FLASH_ID]) >= SOE_MAX_NUM)
            {
                g_FlagDB.queue_soe.out[FLASH_ID] = 0;
            }    
        }
      
        if(have_co)
        {
            if(g_FlagDB.fatfs_co.fullnum < CO_FILE_MAXNUM)
            {
                strcpy(content,"DataRec");
                tag_end(content,&tabs);
                write(MyFile_CO,content, strlen(content));
                if(MyFile_CO == -1)
                {
                    failnum++;
                    FILE_PRINTF("/sojo/HISTORY/CO/co.xml write failed!\n");
                    return(1);
                }
                
                strcpy(content,"DataFile");
                tag_end(content,&tabs);
                write(MyFile_CO,content, strlen(content));
                if(MyFile_CO == -1)
                {
                    failnum++;
                    FILE_PRINTF("/sojo/HISTORY/CO/co.xml write failed!\n");
                    return(1);
                }
            }
            tabs = 1;
            lseek(MyFile_CO, 0x6f+1, SEEK_SET);
            strcpy(content,"DataRec num=\"0010\"");
            sprintf(tempstr,"%04d",g_FlagDB.fatfs_co.fullnum);
            memcpy((content+strlen("DataRec num=\"")),tempstr,4);
            tag_start(content,&tabs);
            write(MyFile_CO,content, strlen(content)); 
            if(MyFile_CO == -1)
            {
                failnum++;
                FILE_PRINTF("/sojo/HISTORY/CO/co.xml write failed!\n");
                return(1);
            }
            FILE_PRINTF("Write CO \r\n");
        }    

        if(have_soe)
        {
            if(g_FlagDB.fatfs_soe.fullnum < SOE_FILE_MAXNUM)
            {
                strcpy(content,"DataRec");
                tag_end(content,&tabs);
                write(MyFile_SOE,content, strlen(content));
                if(MyFile_SOE == -1)
                {
                    failnum++;
                    FILE_PRINTF("/sojo/HISTORY/SOE/soe.xml write failed!\n");
                    return(1);
                }
                
                strcpy(content,"DataFile");
                tag_end(content,&tabs);
                write(MyFile_SOE,content, strlen(content));
                if(MyFile_SOE == -1)
                {
                    failnum++;
                    FILE_PRINTF("/sojo/HISTORY/SOE/soe.xml write failed!\n");
                    return(1);
                }
            }
            
            tabs = 1;
            lseek(MyFile_SOE, 0x6f+1, SEEK_SET);
            strcpy(content,"DataRec num=\"0010\"");
            sprintf(tempstr,"%04d",g_FlagDB.fatfs_soe.fullnum);
            memcpy((content+strlen("DataRec num=\"")),tempstr,4);
            tag_start(content,&tabs);
            write(MyFile_SOE,content, strlen(content));	
            if(MyFile_SOE == -1)
            {
                failnum++;
                FILE_PRINTF("/sojo/HISTORY/SOE/soe.xml write failed!\n");
                return(1);
            }
            FILE_PRINTF("Write SOE \r\n");
        }
        
        close(MyFile_CO);
        close(MyFile_SOE);
        
        failnum = 0;
    }
    return(0);
}

/**
  * @brief : AddDoc_Random.
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */	
uint8_t AddDoc_Random(void)
{
    if(AddDoc_SOE_CO())
    {return(1);}
    if(AddDoc_FEVNET())
    {return(1);}
    rt_multi_common_data_fram_record_write(MEMORY_FLAG, (uint8_t *)&g_FlagDB, sizeof(g_FlagDB));    
    return(0);
}

/**
  * @brief : start_Comtrade.
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
uint8_t start_Comtrade(void)
{
    static int intervalNumber = 0;//间隔序号
    static int recordNumber = 1;//录波序号
    int configDataSize;
    char *filename;
    
    if (g_WaveOut != g_WaveIn)
    {
        strcpy(DirName,"/sojo");
        strcat(DirName,"/HISTORY/COMTRADE");  
        file_Dir_Clear(DirName,COMTRADE_FILE_MAXNUM,sizeof("/sojo/HISTORY/COMTRADE/BAY00_0001_")); 
        file_Dir_Clear(DirName,COMTRADE_FILE_MAXNUM,sizeof("/sojo/HISTORY/COMTRADE/BAY00_0001_"));
        
        FILE_PRINTF("Comtrade Start \r\n"); 
        strcpy(FileName,"/sojo");
        strcat(FileName,"/HISTORY/COMTRADE/");      
        filename = Get_filenameCfg(&g_WaveBuf[g_WaveOut].time, &intervalNumber, &recordNumber);
        strcat(FileName,filename); 
        MyFile = open(FileName, O_RDWR | O_CREAT, 0);
        if (MyFile == -1)
        {
            failnum++;
            FILE_PRINTF("/sojo/HISTORY/HISTORY/COMTRADE/ open failed!\n");
            return 1;
        }        
        FILE_PRINTF("f_open =%d//=%d\n", MyFile , recordNumber);
        write(MyFile,CreatConfigurationHandler(Get_firsttime(&g_WaveBuf[g_WaveOut].time), Get_triggertime(&g_WaveBuf[g_WaveOut].time), &configDataSize), configDataSize);
        FILE_PRINTF("f_write =%d//=%d\n", MyFile , recordNumber );
        close(MyFile);

        strcpy(FileName,"/sojo");
        strcat(FileName,"/HISTORY/COMTRADE/"); 
        filename = Get_filenameDat(&g_WaveBuf[g_WaveOut].time, &intervalNumber, &recordNumber);
        strcat(FileName,filename);
        MyFile = open(FileName, O_RDWR | O_CREAT, 0); 
        if (MyFile == -1)
        {
            failnum++;
            FILE_PRINTF("/sojo/HISTORY/HISTORY/COMTRADE/ open failed!\n");
            return 1;
        }        
        FILE_PRINTF("f_open =%d//=%d\n", MyFile , recordNumber );        
        write(MyFile,CreatDataHandler(g_WaveBuf[g_WaveOut].ia, g_WaveBuf[g_WaveOut].ib, g_WaveBuf[g_WaveOut].ic, g_WaveBuf[g_WaveOut].i0,
            g_WaveBuf[g_WaveOut].ua, g_WaveBuf[g_WaveOut].ub, g_WaveBuf[g_WaveOut].uC, g_WaveBuf[g_WaveOut].u0, g_WaveBuf[g_WaveOut].digital), 26 * WAVE_NUM_ALL * 2 * ADC_SAMPLE_NUM);
        FILE_PRINTF("f_write =%d//=%d\n", MyFile , recordNumber );
        if (MyFile == -1)
        {
            failnum++;
            FILE_PRINTF("/sojo/HISTORY/HISTORY/COMTRADE/ write failed!\n");
            return 1;
        }
        close(MyFile);

        recordNumber++;
        if (recordNumber == 9999)
        {
            recordNumber = 1;
        }
        
        if (++g_WaveOut >= WAVE_SDRAM_NUM)
        {
            g_WaveOut = 0;					
        }
        FILE_PRINTF("Comtrade Finsh \r\n"); 
        failnum = 0;
    }

    return (0);
}

/**
  * @brief : file_operate.
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
void file_operate(void)
{      
    if (!(g_CommunicatFlag[COM_FILE]&COMMUNICATLOCKUSERSTA))
    {    
        if ((failnum>FAILEMAXNUM)|| (g_CommunicatFlag[COM_FILE]&COMMUNICATLOCKCLEAR))
        {
            g_CommunicatFlag[COM_FILE] |= COMMUNICATLOCKMAKEFS;
            file_operate_Clear();
            failnum = 0;
            g_CommunicatFlag[COM_FILE] &= ~COMMUNICATLOCKMAKEFS;
            g_CommunicatFlag[COM_FILE] &= ~COMMUNICATLOCKCLEAR;
        }
    
        if (g_WaveOut != g_WaveIn)
        {
            g_CommunicatFlag[COM_FILE] |= COMMUNICATLOCKCOMTRADE;
            start_Comtrade();
            g_CommunicatFlag[COM_FILE] &= ~COMMUNICATLOCKCOMTRADE;
        }        
               
        if (g_CommunicatFlag[COM_FILE]&COMMUNICATLOCKRECORD)
        {
            g_CommunicatFlag[COM_FILE] |= COMMUNICATLOCKRANDOM;
            if (!AddDoc_Random())
            {
				rt_multi_common_data_powerdown_storage();
                g_CommunicatFlag[COM_FILE] &= ~COMMUNICATLOCKRECORD;
            }
            g_CommunicatFlag[COM_FILE] &= ~COMMUNICATLOCKRANDOM;
        }
    }
}

/**
  * @brief : file_operate_Clear.
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
void file_operate_Clear(void)
{    
    file_operate_Format();
	
    memset(&Read_Dir, 0, sizeof(Read_Dir));
    memset(&Read_File, 0, sizeof(Read_File));
}

/**
  * @brief : file_operate_Init.
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
void file_operate_Init(void)
{ 
    Dir_Array = (struct DIR_ARRAY *)rt_malloc(sizeof(struct DIR_ARRAY)); 
    Dir_ArrayClear = (struct DIR_ARRAY *)rt_malloc(sizeof(struct DIR_ARRAY)); 	
    
    /* mount sd card fat partition 0 as root directory */
//    if (dfs_mount("W25Q256FV", "/sojo", "elm", 0, 0) == 0)
    if (dfs_mount(RT_SPI_FLASH_NAME, "/sojo", "elm", 0, 0) == 0)
    {  
        FILE_PRINTF("spi flash mount to /spi !\n");        
    }
    else
    {
        FILE_PRINTF("spi flash mount to /spi failed!\n");  
        file_operate_Format();        
    }
    
    memset(&Read_Dir, 0, sizeof(Read_Dir));
    memset(&Read_File, 0, sizeof(Read_File));
    
	    
	rt_s2j_init();
    
    Create_JsonFile("FixedValueCfg2", g_FixedValueCfg2_Len, _CFG_FIXED_VALUE_2);
    
}

/* END OF FILE ---------------------------------------------------------------*/

