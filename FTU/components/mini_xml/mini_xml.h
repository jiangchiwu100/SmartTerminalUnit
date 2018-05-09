/*
** ------------------------------- Copyright (c) -------------------------------                                     
**
**                           http://www.sojoline.com
**
** note:    Copyright SOJO. ALL RIGHTS RESERVED.
**          This software is provided under license and contains proprietary and
**          confidential material which is the property of Company Name tech. 
**    
** -------------------------------- File Info ----------------------------------
** File name:               smxl.c
** Descriptions:            The driver of SPI 
**
** -----------------------------------------------------------------------------
** Created by:              Mr.Lee
** Created date:            2017.02.08
** System:                  SJFD2000 communication software system
** Version:                 V4.01
** Descriptions:            The original version
**
** -----------------------------------------------------------------------------
** Modified by:             
** Modified date:           
** Version:                
** Descriptions:            
**
** ---------------------------------------------------------------------------*/
#ifndef _SXML_H_
#define _SXML_H_

			    
/* DEFINE --------------------------------------------------------------------*/

/* PUBLIC FUNCTION API -------------------------------------------------------*/
extern void put_head(char *head);//xml文件头
extern void tag_start(char *tag,char *tabs);//xml节点开始
extern void tag_end(char *tag,char *tabs);//xml节点结束
extern void tag_value(char *tag,char *tabs);//xml内容

#endif /* END _SPI_H_ */

/* END OF FILE ---------------------------------------------------------------*/

