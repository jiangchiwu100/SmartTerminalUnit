/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      single_list.c
  * @brief:     The xml file systerm.
  * @version:   V1.0.1 
  * @author:    LEX
  * @date:      2017.12.06
  * @update:    [YYYY-MM-DD] [更改人姓名][变更描述]
  */


/* INCLUDE FILES -------------------------------------------------------------*/
#include "single_list.h"
#include <rtthread.h>


void list_init(List *list)
{
    list->size = 0;
    list->head = NULL;
    list->tail = NULL;
    return;
}

int list_ins_next(List *list,const void *data)
{
    ListElmt *new_element;
    
    if((new_element = (ListElmt *)rt_malloc(sizeof(ListElmt))) == NULL)
        return -1;
    
    new_element->data = (void *)data;

    if(list_size(list) == 0)
    list->tail = new_element;
    
    new_element->next = list->head;
    list->head = new_element;
    
    list->size++;
    
    return 0;
}

