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
** File name:               single_list.c
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
#ifndef _SINGLELIST_H_
#define _SINGLELIST_H_

#include<stdlib.h>

typedef struct ListElmt_
{
    void *data;
    struct ListElmt_ *next;
}ListElmt;

typedef struct List_
{
    int size;
    ListElmt *head;
    ListElmt *tail;
}List;

void list_init(List *list);
int list_ins_next(List *list,ListElmt *element,const void *data);

#define list_size(list) ((list)->size)
#define list_head(list) ((list)->head)		
#define list_tail(list) ((list)->tail)	
#define list_is_head(list,element) ((element) == (list)->head ? 1 : 0)
#define list_is_tail(element) ((element)->next == NULL ? 1 : 0)


#endif /* END _SINGLELIST_H_ */

/* END OF FILE ---------------------------------------------------------------*/

