/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      mini_xml.c
  * @brief:     The xml file systerm.
  * @version:   V1.0.1 
  * @author:    LEX
  * @date:      2017.12.06
  * @update:    [YYYY-MM-DD] [更改人姓名][变更描述]
  */


/* INCLUDE FILES -------------------------------------------------------------*/
#include "mini_xml.h"
#include "string.h"

char temp[512];

void put_head(char *head)
{
	strcpy(temp,head);
	*head = '\0';
	strcat(head,"<?");
	strcat(head,temp);
	strcat(head,"?>\n");
}

void tag_start(char *tag,char *tabs)
{
	int i;

	strcpy(temp,tag);
	*tag = '\0';
	for(i=0;i<(*tabs);i++)
	{
		strcat(tag,"\t");
	}
	strcat(tag,"<");
	strcat(tag,temp);
	strcat(tag,">\n");
	(*tabs) = (*tabs)+1;
}

void tag_end(char *tag,char *tabs)
{
	int i;
	
	strcpy(temp,tag);
	*tabs=*tabs-1;
	*tag = '\0';
	for(i=0;i<(*tabs);i++)
	{
		strcat(tag,"\t");
	}
	strcat(tag,"</");
	strcat(tag,temp);
	strcat(tag,">\n");
}

void tag_value(char *tag,char *tabs)
{
	int i;
	
	strcpy(temp,tag);
	*tag = '\0';
	for(i=0;i<(*tabs);i++)
	{
		strcat(tag,"\t");
	}
	strcat(tag,"<");
	strcat(tag,temp);
	strcat(tag,"\t/>\n");
	
}

