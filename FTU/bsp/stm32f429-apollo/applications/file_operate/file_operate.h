/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      file_operate.h
  * @brief:     The protect control logic.
  * @version:   V1.0.0
  * @author:    J.Lee
  * @date:      2017.12.19
  * @update:    
  */
  
#ifndef  __FILE_OPERATE_H__
#define  __FILE_OPERATE_H__			 


/* INCLUDE FILES -------------------------------------------------------------*/


/* DEFINES -------------------------------------------------------------------*/
#ifdef  DEBUG
#define FILE_PRINTF(...)            rt_kprintf(__VA_ARGS__)
#else
#define FILE_PRINTF(...)
#endif /* DEBUG */

#define FILEMAXNUM                  1024
#define FAILEMAXNUM                 5

#define SOE_FILE_MAXNUM             512
#define CO_FILE_MAXNUM              64
#define FEVENT_FILE_MAXNUM          64
#define ULOG_FILE_MAXNUM            64
#define COMTRADE_FILE_MAXNUM        64
//#define EXV_FILE_MAXNUM        31
//#define EXV_START_ADDR         4001
//#define EXV_MAXNUM             14
//#define FIXPT_FILE_MAXNUM        31
//#define FIXPT_START_ADDR         4001
//#define FIXPT_MAXNUM             14

struct FileType
{
	char old_file[255];
	int num;
};

__packed struct DIR_ARRAY
{
	int num;
	char Array[FILEMAXNUM][255];
};

struct READ_DIR
{
	unsigned int dir_i;
    unsigned int dirid;
};

struct READ_FILE
{
	unsigned int fileid;
	unsigned int offset;
	unsigned int filesize;
	char filename[100];
};

struct WRITE_FILE
{
	unsigned int fileid;
	unsigned int filesize;
	unsigned int overtime;
	char filename[100];
};
/* PUBLIC VARIABLES ----------------------------------------------------------*/		   
extern struct DIR_ARRAY *Dir_Array;

/* PUBLIC FUNCTIONS ----------------------------------------------------------*/
extern void file_operate(void);
extern void file_operate_Init(void);
extern void file_operate_Format(void);
extern void file_operate_Clear(void);
extern unsigned char file_operate_DirRead(unsigned char dev,unsigned char *pbuf);
extern unsigned char file_operate_DirCall(unsigned char dev,unsigned char *pbuf);
extern unsigned char file_operate_ReadFileAct(unsigned char dev,unsigned char *pbuf);
extern unsigned char file_operate_ReadFile(unsigned char dev,unsigned char *pbuf);
extern void file_operate_WriteFileAct(unsigned char dev,unsigned char *pbuf);
extern void CreateJsonFile(void);

#endif /* __FILE_OPERATE_H__ */


/* END OF FILE ---------------------------------------------------------------*/



