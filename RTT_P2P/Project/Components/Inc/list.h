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
** File name:               ist.c
** Descriptions:            单链表操作 
**
** -----------------------------------------------------------------------------
** Created by:              张宇飞
** Created date:            2018.05.29
** System:                  
** Version:                 V0.01
** Descriptions:            原始版本来源：（尊重原作者）
https://www.cnblogs.com/idreamo/archive/2017/11/18/7855826.html
**
** -----------------------------------------------------------------------------
** Modified by:        张宇飞     
** Modified date:         2018.07.5 
** Version:                V0.02
** Descriptions:            修改语句错误，修改为双向链表 prev
**                    2018/7/20 改变命名避免冲突
** ---------------------------------------------------------------------------*/

#ifndef _LIST_H_
#define _LIST_H_

#include <stdio.h>  
/*Define a structure for linked list elements.*/  
typedef struct ListElement  
{  
    void *data;  
    struct ListElement *next; 
    struct ListElement *prev; 
} ListElment;  
  
/*Define a structure for linked lists.*/  
typedef struct List_  
{  
    int size;  
    int (*match)(const void *key1,const void *key2);  
    void (*destroy)(void *data);  
    ListElment *head;  
    ListElment *tail;  
} ListDouble;  
  
/*Public Interface*/  
void ListInit(ListDouble *list, void(*destroy)(void *data));  
void Listdestroy(ListDouble *list);  
int ListInsertNext(ListDouble *list, ListElment *element, const void *data);  
int ListRemoveNext(ListDouble *list, ListElment *element, void **data);  

#define list_size(list) ((list)->size)  
#define list_head(list) ((list)->head)  
#define list_tail(list) ((list)->tail)  

#define list_is_head(list, element)(element==(list)->head ? 1 : 0)  
#define list_is_tail(element)((element)->next==NULL ? 1 : 0)  
#define list_data(element)((element)->data)  
#define list_next(element)((element)->next)  
      

#endif
