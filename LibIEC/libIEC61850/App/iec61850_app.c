/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      iec61850_app.c
  * @brief:     61850娴犺濮?
  * @version:   V0.0.0 
  * @author:    Zhang Yufei
  * @date:      2018-08-13
  * @update:    
  */

#include "extern_interface.h"





static struct rt_thread iec61850_thread;//缁捐法鈻奸幒褍鍩楅崸锟?



ALIGN(RT_ALIGN_SIZE)
static rt_uint8_t rt_iec61850_thread_stack[2048];//缁捐法鈻奸弽锟?




static void iec61850App(void);


static void iec61850_thread_entry(void* parameter)
{    
	rt_kprintf("thread iec61850App start.\r\n");
    rt_thread_delay(1000);
	config_flie_test();
    //TestGooseBeat();
}
  




/**
  * @brief :iec61850App
  * @param  Svoid
  * @return: 0--濮濓絽鐖?
  * @update: [2018-08-13][瀵姴鐣ゆ鐎匸閸掓稑缂揮
  */
static void iec61850App(void)
{    

        
    
}





/**
  * @brief :IEC61850AppInit
  * @param  void
  * @return: void
  * @update: [2018-08-13][瀵姴鐣ゆ鐎匸閸掓稑缂揮
  */
void IEC61850AppInit(void)
{

    rt_thread_init(&iec61850_thread,                 //缁捐法鈻奸幒褍鍩楅崸锟?
		"61850",                       //缁捐法鈻奸崥宥呯摟閿涘苯婀猻hell闁插矂娼伴崣顖欎簰閻鍩?
                   iec61850_thread_entry,            //缁捐法鈻奸崗銉ュ經閸戣姤鏆?
                   RT_NULL,                      //缁捐法鈻奸崗銉ュ經閸戣姤鏆熼崣鍌涙殶
                   &rt_iec61850_thread_stack,     //缁捐法鈻奸弽鍫ｆ崳婵婀撮崸锟?
		sizeof(rt_iec61850_thread_stack), //缁捐法鈻奸弽鍫濄亣鐏忥拷
		13,                            //缁捐法鈻奸惃鍕喘閸忓牏楠?
		20);                          //缁捐法鈻奸弮鍫曟？閻楋拷
                               
    rt_thread_startup(&iec61850_thread);  
    


    return;

}

