#ifndef	_CHANNEL_MONITOR_H_
#define _CHANNEL_MONITOR_H_
#include "stm32f4xx_hal.h"



#define SerialPort1     1                   //串口1
#define SerialPort2     2                   //串口2
#define InternetAccess1 3                   //网口1
#define InternetAccess2 4                   //网口2

#define TransmittedData         0xAA    //此数据为发送数据          
#define ReceivedData            0x55    //此数据为接收数据
#define LinkAliveTime           20000    //5s
#define DLT634_CHANNEL_MONITOR_VOLUMES  2

/* 通道监视报文修饰词***************************************************************/
#define  _DLT634_ChannelToMonitor_SLAVE_STARTCODE11	             0x11					// 起始码
#define  _DLT634_ChannelToMonitor_SLAVE_STARTCODE12              0x12                   // 停止监听
#define  _DLT634_ChannelToMonitor_SLAVE_ENDCODE66                0x66					// 终止码

#define  _DLT634_ChannelToMonitor_SLAVE_FC_TEST                  0x01 					// 通道监听测试
#define  _DLT634_ChannelToMonitor_SLAVE_FC_TEST_CON              0x02 					// 通道监听测试回复
#define  _DLT634_ChannelToMonitor_SLAVE_FC_ON                    0x09 					// 通道监听启动
#define  _DLT634_ChannelToMonitor_SLAVE_FC_OFF                   0x0A 					// 通道监听结束
#define  _DLT634_ChannelToMonitor_SLAVE_FC_RESPONSE              0x0B 					// 通道监听应答
#define  _DLT634_ChannelToMonitor_SLAVE_FC_OFF_RESPONSE          0x0C 					// 停止通道监听应答
#define  _DLT634_ChannelToMonitor_AddError                       0X0F                   // 地址错误

#define _ChannelToMonitor1_START         	        0x01					//串口启动通道监听标志
#define _ChannelToMonitor2_START         	        0x02					//网口启动通道监听标志


#define AddrSize                                    2                       //链路地址长度


#define DLT634_5101                                 101
#define DLT634_5104                                 104
    
    
typedef struct 
{
    enum
    {
        CRXDSTART = 0,	        											// 寻找起始标志状态
        CRXDHEAD,	            											// 寻找报文头
        CRXDCONTINUE             											// 接收后续报文
    }CRxdStatus;
    uint8_t MonitorFlag[2];                                                       //监听标志
    uint16_t ByAddr;                                                    //被监听地址
    uint16_t Addr;                                                      //监听地址
	uint32_t LinkCounter;                                               //链路计数器
}ChannelToMonitor;                                                      //通道监听

extern ChannelToMonitor channel_monitor;

int rt_channel_monitor_init(void);
void rt_channel_monitor_task(void);
uint8_t DLT634_ChannelToMonitor_SLAVE_SearchMonitorFrame(uint8_t pdrv, uint8_t protocol, uint8_t *RxdBuf, uint16_t len);
void MonitoringDataTransmission(uint8_t *pbuf, uint16_t count, uint8_t ReceiveAndDispatch);
#endif


