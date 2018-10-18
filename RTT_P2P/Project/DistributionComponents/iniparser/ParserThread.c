/**
 * @file GooseParser.c
 * @brief 解析goose的ini配置文件的线程文件
 * @copyright Copyright (c) 2017 Beijing SOJO Electric CO., LTD.
 * @company SOJO
 * @date 2018.08.27
 *
 * @author Lei
 * @version ver 1.0
 **/


/*------------------------------Include-------------------------------------*/
#include <stdio.h>
#include <rtthread.h>
#include "rtconfig.h"
#include "rtdef.h"
#include "gooseparser.h"
#include "ParserThread.h"



/*--------------------------------------------------------------------------*/

#ifdef RT_USING_STATIC_THREAD

#if RT_USING_INIPARSER    
static struct rt_thread *rt_thread_iniparser;
static uint8_t *rt_thread_iniparser_stack;
#endif /* END RT_USING_INIPARSER */

#endif /* RT_USING_STATIC_THREAD */

const char* g_GoosePathName = "//sojo//stu-goose.txt";



/**
  * @brief : iniparser thread entry
  * @param : [param] the parameter.
  * @return: none
  * @updata: [2018-08-25][Lei][newly]
  */
#if RT_USING_INIPARSER
static void rt_iniparser_thread_entry(void *param)
{   
	rt_thread_delay(3000);

    GooseTxMessage gooseTxMessage;
    GooseRxMessage gooseRxMessage;

	printf("rt_iniparser_thread_entry start\r\n");

	rt_thread_delay(5000);
	
	printf("%d:\r\n", 0);

	GooseIniParser((uint8_t*)g_GoosePathName, &gooseTxMessage, &gooseRxMessage);
	PrintGooseTxRxMessage(&gooseTxMessage, &gooseRxMessage);
	FreeGooseMessageMem(&gooseTxMessage, &gooseRxMessage);

	printf("\r\n");
	

	
}
#endif /* RT_USING_INIPARSER */


/**
  * @brief : goose ini file parser test
  * @param : none
  * @return: none
  * @updata:
  */
#if RT_USING_INIPARSER
static void iniparser_thread_start(void *param)
{

    rt_thread_t tid; 

    tid = rt_thread_create(INIPARSER_THREAD_NAME, 
                           rt_iniparser_thread_entry, 
                           param, 
                           INIPARSER_THREAD_STACK_SIZE, 
                           INIPARSER_THREAD_PRIORITY, 
                           INIPARSER_THREAD_TIMESLICE);

    if (tid != RT_NULL)
    {
        rt_thread_startup(tid);

        rt_kprintf("iniparser thread start \r\n"); 
    }   
    else
    {
        rt_kprintf("rt_thread_create failure \r\n"); 
    }
        

}
#endif /* RT_USING_INIPARSER */


uint8_t rt_goose_parser_thread_start(void *param)
{
	#if RT_USING_INIPARSER
		iniparser_thread_start(RT_NULL);
	#endif /* RT_USING_INIPARSER */

	return(RT_EOK);
}
//INIT_APP_EXPORT(rt_goose_parser_thread_start);





