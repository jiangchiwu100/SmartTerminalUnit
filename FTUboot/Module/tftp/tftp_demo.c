#include "config.h"
#include <stdint.h>
#include "stm32f4xx_hal.h"
#include "ult.h"
#include "device.h"
#include "w5500.h"
#include "socket.h"
#include "sdram.h"
#include "ult.h"
#include "tftp.h"
#include <stdio.h>
#include <string.h>
#include "drv_fm25vxx.h"
#include "fm25v10.h"


#include "systemconfig.h"


#define rt_kprintf DebugPrintf
#define rt_memcpy memcpy

static void set_network_check(void);
   


/***************测试时需要修改为测试PC机的IP地址******************/
/*定义远端IP信息*/
uint8  RemoteIp[4]={192,168,10,111};							/*远端IP地址*/
uint16 RemotePort=6000;										/*远端端口号*/

TFTP_MSG  tftp;
uint8_t g_socket_rcv_buf[MAX_MTU_SIZE];
uint8 Buffer[1024];
uint32_t tftp_server;
uint8 filename_flag=0;


uint32_t SaveCount = 0;
#define DOWN_MAX_SIZE (1024*1024*1)

static uint32_t ProgramSize __attribute__((at(Bank5_SDRAM_ADDR )));//2M空间
static uint8_t SaveData[DOWN_MAX_SIZE] __attribute__((at(Bank5_SDRAM_ADDR + 4)));//2M空间

int tftp_demo(void)
{
	uint8 len,ret;	
	u32 addr; 
    u32 app_first_4_bytes;
	
	SaveCount = 0;
    ProgramSize = 0;
	

	//TIM3_Int_Init(1000,71);//10Khz的计数频率，计数到1000为1ms 
	rt_kprintf("\r\n MCU initialized.\r\n");
	
     w5500_spi_init();
     Reset_W5500();
    set_default();
    set_network_check();
    
    
	rt_kprintf("Network is ready!\r\n");
 
	get_config();//read config data from flash 
    
  
	TFTP_init(SOCK_TFTP, g_socket_rcv_buf);
    char test[] = "stu_v00.001.bin";
    rt_memcpy(ConfigMsg.filename, test, strlen(test) + 1);
    filename_flag = 1;
	while(1)
    {
		if(1)
		{			
					
            if(filename_flag==1)
            {
                filename_flag=0;			
                rt_kprintf(" TFTP START\r\n");
                tftp_server = (RemoteIp[0] << 24) | (RemoteIp[1] << 16) | (RemoteIp[2] << 8) | (RemoteIp[3]);
                TFTP_read_request(tftp_server, ConfigMsg.filename);
                while(1)
                {	
                    ret = TFTP_run();							
                    //if(ret == TFTP_SUCCESS)
                    if(ret != TFTP_PROGRESS)
                    break;
                }
                if(ret == TFTP_SUCCESS)
                {
                    rt_kprintf("TFTP SUCCEED\r\n");	
                    rt_kprintf("Size : %d\r\n", SaveCount);
                    //简单检测不超范围
                    if (SaveCount < DOWN_MAX_SIZE)
                    {
                        //保存长度
                        ProgramSize = SaveCount;
                        //写长度
                        uint8_t data = IS_WILLUPDATA_FLAG;
                        FramWriteByte(PRO_STATE_BEGINADDR + PRO_MAINBOARD_STATE, data);
                         data =  FramReadByte(PRO_STATE_BEGINADDR + PRO_MAINBOARD_STATE);
                        if (data != IS_WILLUPDATA_FLAG)
                        {
                            rt_kprintf("Write Frame ERROR!\r\n");
                        }
                    }
                    
                    //TIM_Cmd(TIM3, DISABLE);							
                    Delay_ms(100);
                    return 0;
                    //reboot_app();
                }
                else if (ret == TFTP_FAIL)
                {
                    rt_kprintf("TFTP Failured\r\n");	
                    rt_kprintf("Size : %d\r\n", SaveCount);
                    //TIM_Cmd(TIM3, DISABLE);							
                    Delay_ms(100);
                    return 0;
                }
                else
                {
                    rt_kprintf("TFTP Unkonw Error\r\n");
                    return 0;
                }
                
            }
            Delay_ms(10);
		}
		else
		{
			rt_kprintf(" newersion do not find, start oldversion\r\n");
			//TIM_Cmd(TIM3, DISABLE);	
			//reboot_app();
		}

	}										
}
void save_data(uint8_t *data, uint32_t data_len, uint16_t block_number)
{
    if (SaveCount < DOWN_MAX_SIZE)
    {
       rt_memcpy(SaveData + SaveCount, data, data_len);
       SaveCount += data_len; 
        
    }
    else
    {
        SaveCount = DOWN_MAX_SIZE;
    }
    
    
}
static void set_network_check(void)
{
    uint8_t result = 0;
    do
    {
        
        result = set_network();	// 配置初始化IP信息并打印，初始化8个Socket
        if(result)
        {
             rt_kprintf("w5500 Init Failure.\r\n"); /* 打印线程计数值输出 */
             Delay_ms(500);
        }
        else
        {
             rt_kprintf("w5500 Init Success.\r\n"); /* 打印线程计数值输出 */
            break;
        }
        
    } while(1);
}