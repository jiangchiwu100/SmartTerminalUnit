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
**
** ---------------------------------------------------------------------------*/

#include <stdio.h>  
#include <string.h>  
  
#include "list.h"  

#include "extern_interface.h"

/*ListInit*/  
void ListInit(ListDouble *list,void(*destroy)(void *data))  
{  
    list->size = 0;  
    list->destroy = destroy;  
    list->head = NULL;  
    list->tail = NULL;  
  
    return;  
}  
  
/*Listdestroy*/  
void Listdestroy(ListDouble *list)  
{  
    void *data;  
    /*Remove each element*/  
    while(list_size(list)>0)  
    {  
        if(ListRemoveNext(list,NULL,(void **)&data)==0 && list->destroy!=NULL)  
        {
            /*Call a user-defined function to FREE dynamically allocated data.*/  
            list->destroy(data);  
        }  
    }  
    MEMSET(list, 0, sizeof(list));
    return;  
}  
  
/*ListInsertNext*/  
int ListInsertNext(ListDouble *list, ListElment *element, const void *data)  
{  
    ListElment *new_element;  
      
    /*Allocate storage for the element*/  
	new_element = (ListElment *)CALLOC(1, sizeof(ListElment));
    if(new_element == NULL )
        return -1;  
    /*insert the element into the list*/  
    new_element->data=(void *)data;  
    if(element == NULL)  
    {  
        /*Handle insertion at the head of the list. */  
        if(list_size(list) == 0)  
            list_tail(list) = new_element;  
              
        new_element->next = list->head; 
        list->head = new_element;

        //previous
        if (list->head->next == NULL)
        {
            list->head->prev = NULL;
        }
        else
        {
            list->head->next->prev = list->head;
            list->head->prev = NULL;
        }
    }  
    else   
    {  
        /*Handle insertion somewhere other than at the head*/  
        if(element->next==NULL)  
            list->tail = new_element;  
          
        new_element->next = element->next;  
        element->next = new_element;  

        //previous
        new_element->prev = element;
       
    }  
    /*Adjust the size of the list of account for the inserted element. */  
    list->size++;  
      
    return 0;  
}  
  
/*ListRemoveNext*/  
int ListRemoveNext(ListDouble *list,ListElment *element,void **data)  
{  
    ListElment *old_element;  
      
    /*Do not allow removal from an empty list. */  
    if(list_size(list) == 0)  
        return -1;  
      
    /*Remove the element from the list. */  
    if(element == NULL)  
    {  
        /*Handle removal from the head of the list. */  
        *data = list->head->data;  
        old_element = list->head;  
        list->head = list->head->next;  
        
        //previous
        if (list->head != NULL)        
        {
            list->head->prev = old_element->prev;
        }

        if(list_size(list) == 1)  
            list->tail = NULL;  
    }  
    else   
    {  
        /*Handle removal from somewhere other than the head. */  
        if(element->next == NULL)  
            return -1;  
          
        *data = element->next->data;  
        old_element = element->next;  
        element->next = element->next->next;  
                  
        if (element->next == NULL)
        {
            list->tail = element;
        }
        else
        {
            //previous
            element->next->prev = old_element->prev;
        }
    }  
    /*FREE the storage allocated by the abstract datatype.*/  
    FREE(old_element);  
    /*Adjust the size of the list account for the removed element. */  
    list->size--;  
    return 0;  
}




