 /**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      application.c
  * @brief:     This file is part of RT-Thread RTOS.
  * @version:   V03.001(first implementation)
  * @author:    Bernard
  * @date:      2009-01-05
  * @updata:    [2014-04-27][Bernard][make the code cleanup]
  * @updata:    [2018-01-23][Lexun][make the code cleanup]
  */


/* INCLUDE FILES -------------------------------------------------------------*/
#include <board.h>
#include <rtthread.h>
#include <drivers/mtd_nand.h>
#include "finsh.h"
#include "time.h"
#include ".\MultiThread\multi_thread.h"

#ifdef RT_USING_DFS
/* dfs init */
/* dfs filesystem:ELM filesystem init */
#include <dfs_elm.h>
/* dfs Filesystem APIs */
#include <dfs_fs.h>
#include <dfs_posix.h>
#include <dfs_romfs.h>
#endif /* RT_USING_DFS */

#ifdef RT_USING_LWIP
#include <lwip/sys.h>
#include <lwip/api.h>
#include <netif/ethernetif.h>
#include "drv_eth.h"
#endif /* RT_USING_LWIP */

#ifdef RT_USING_GDB
#include <gdb_stub.h>
#endif /* RT_USING_GDB */

#ifdef RT_USING_GUIENGINE
#include "rtgui_demo.h"
#include <rtgui/driver.h>
#endif /* RT_USING_GUIENGINE */


/* PUBLIC FUNCTION PROTOTYPES ------------------------------------------------*/
/**
  * @brief : The initialization thread body.
  * @param : [parameter] the parameter of thread.
  * @return: None. 
  */  
void rt_init_thread_entry(void* parameter)
{    
    /* initialization RT-Thread Components */
#ifdef RT_USING_COMPONENTS_INIT
    rt_components_init();
#endif /* RT_USING_COMPONENTS_INIT */
    
    /* GDB STUB */
#ifdef RT_USING_GDB
//    gdb_set_device("uart6");
    gdb_set_device(RT_USART6_NAME);
    gdb_start();
#endif /* RT_USING_GDB */
    
#ifdef RT_USING_DFS   

    #ifdef RT_USING_DFS_UFFS
//        /* mount nand flash partition 0 as root directory */
//        if (dfs_mount(RT_MTD_NAND_NAME, "/", "uffs", 0, 0) == 0)
//        {
//            mkdir("/nand",0);
//            THREAD_PRINTF("uffs initialized!\n");
//        }
//        else        
//        {
//            THREAD_PRINTF("uffs initialzation failed!\n");
//		   	//dfs_mkfs("uffs", RT_MTD_NAND_NAME);
//        }
     //dfs_mkfs("uffs", RT_MTD_NAND_NAME);       
    #endif /* RT_USING_DFS_UFFS */ 

#ifdef RT_USING_DFS_ELMFAT
    {
        static const rt_uint8_t _romfs_root_readme_txt[] = {
            0x52,0x54,0x2d,0x54,0x68,0x72,0x65,0x61,0x64,0x0d,0x0a,0x00
        };

        static const rt_uint8_t _romfs_root_sdcard_aaa_txt[] = {
            0x52,0x54,0x2d,0x54,0x68,0x72,0x65,0x61,0x64,0x0d,0x0a,0x00
        };

        static const struct romfs_dirent _romfs_root_sdcard[] = {
            {ROMFS_DIRENT_FILE, "aaa.txt", (rt_uint8_t *)_romfs_root_sdcard_aaa_txt, sizeof(_romfs_root_sdcard_aaa_txt)/sizeof(_romfs_root_sdcard_aaa_txt[0])}
        };

        static const rt_uint8_t _romfs_root_spi_bbb_txt[] = {
            0x52,0x54,0x2d,0x54,0x68,0x72,0x65,0x61,0x64,0x0d,0x0a,0x00
        };

        static const struct romfs_dirent _romfs_root_spi[] = {
            {ROMFS_DIRENT_FILE, "bbb.txt", (rt_uint8_t *)_romfs_root_spi_bbb_txt, sizeof(_romfs_root_spi_bbb_txt)/sizeof(_romfs_root_spi_bbb_txt[0])}
        };

        static const struct romfs_dirent _romfs_root[] = {
            {ROMFS_DIRENT_FILE, "readme.txt", (rt_uint8_t *)_romfs_root_readme_txt, sizeof(_romfs_root_readme_txt)/sizeof(_romfs_root_readme_txt[0])},
            {ROMFS_DIRENT_DIR, "sdcard", (rt_uint8_t *)_romfs_root_sdcard, sizeof(_romfs_root_sdcard)/sizeof(_romfs_root_sdcard[0])},
            {ROMFS_DIRENT_DIR, "sojo", (rt_uint8_t *)_romfs_root_spi, sizeof(_romfs_root_spi)/sizeof(_romfs_root_spi[0])}
        };

        static const struct romfs_dirent romfs_root = {
            ROMFS_DIRENT_DIR, "/", (rt_uint8_t *)_romfs_root, sizeof(_romfs_root)/sizeof(_romfs_root[0])
        };

        if (dfs_mount(RT_NULL, "/", "rom", 0, &(romfs_root)) == 0)
        {
            THREAD_PRINTF("ROM file system initializated!\n");
        }
    }
    
//    /* mount sd card fat partition 0 as root directory */
//    if (dfs_mount("sd0", "/sdcard", "elm", 0, 0) == 0)
//    if (dfs_mount(RT_SDIO_SD_NAME, "/sdcard", "elm", 0, 0) == 0)
//    {
//        THREAD_PRINTF("sd card mount to /sdcard!\n");        
//    }
//    else
//    {
//        THREAD_PRINTF("sd card mount to /sdcard failed!\n");
//    }
#endif /* RT_USING_DFS_ELMFAT */

#endif /* RT_USING_DFS */
        
#ifdef RT_USING_GUIENGINE
    {
        rt_device_t device;
        
        device = rt_device_find(RT_LCD_NAME);
		
        /* re-set graphic device */
        rtgui_graphic_set_device(device);
        
        rt_gui_demo_init();
    }
#endif /* RT_USING_GUIENGINE */
}

/**
  * @brief : create initialization thread body.
  * @param : None.
  * @return: None. 
  */  
int rt_application_init(void)
{
    rt_thread_t tid;

    tid = rt_thread_create(INIT_THREAD_NAME, 
                           rt_init_thread_entry, 
                           RT_NULL, 
                           INIT_THREAD_STACK_SIZE, 
                           INIT_THREAD_PRIORITY, 
                           INIT_THREAD_TIMESLICE);

    if (tid != RT_NULL)
    {
        rt_thread_startup(tid);
    }
    
//	rt_multi_thread_start();

    return(0);
}

/* END OF FILE ---------------------------------------------------------------*/

