#include "channel_monitor.h"
#include "string.h"
#include "common_data.h"
#include "dlt634_5101slave_app.h"
#include "dlt634_5101slave_disk.h"
#include "dlt634_5104slave_disk.h"
#include "tcp_server.h"
#include "drv_w5500_socket.h"
#include "lwip/sockets.h"


//extern unsigned short DP83848TcpServerTxLen;
//extern unsigned short W5500TcpServerTxLen;

static rt_device_t dev[_DLT634_5101SLAVE_VOLUMES];
ChannelToMonitor channel_monitor;


static uint8_t DLT634_ChannelToMonitor_SLAVE_CKS(uint8_t pdrv ,uint8_t *pBuf)
{
    uint16_t sum = 0;
    uint16_t j =0;

    if (*(pBuf) == _DLT634_ChannelToMonitor_SLAVE_STARTCODE11)
    {
        for (j = 0; j < (AddrSize + 1); j++)
        {
            sum += *(pBuf + 1 + j);
        }
    }
    return (uint8_t)(sum&0xff);
}
/**
  * @brief : check iec 11.IEC校验
  * @param : [pdrv]
  * @return: res
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
static uint8_t DLT634_ChannelToMonitor_SLAVE_CheckFram11Valid(uint8_t pdrv, uint8_t *RxdBuf)
{
    if (RxdBuf[2 + AddrSize] != (DLT634_ChannelToMonitor_SLAVE_CKS(pdrv, RxdBuf)&0xFF))	                    // 固定帧长数据的校验和检测
    {
        return (0);
    }
    if (RxdBuf[3 + AddrSize] != _DLT634_ChannelToMonitor_SLAVE_ENDCODE66)                                // 固定帧长数据的结束帧检测
    {
        return (0);
    }
    return (1);
}

/**
  * @brief : Slave Frame 11 RESPONSE.监听应答
  * @param : [pdrv]
  * @return: res
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
static void DLT634_ChannelToMonitor_SLAVE_SlaveFrame11Response(uint8_t pdrv, uint8_t protocol)
{
    uint8_t pBuf[6];
	
	if (channel_monitor.ByAddr)                                                              //被监听通道地址不为零
    {
        pBuf[0] = _DLT634_ChannelToMonitor_SLAVE_STARTCODE11;                                            //起始码
        pBuf[1] = _DLT634_ChannelToMonitor_SLAVE_FC_RESPONSE;                                            //控制域，回答响应
		pBuf[2] = channel_monitor.ByAddr & 0xff;
        pBuf[3] = (channel_monitor.ByAddr>>8) & 0xff;

        pBuf[2+AddrSize] = DLT634_ChannelToMonitor_SLAVE_CKS(pdrv, pBuf);                               //帧校验和
        pBuf[3+AddrSize] = _DLT634_ChannelToMonitor_SLAVE_ENDCODE66;                                     //终止码
        if(protocol == DLT634_5101)
        {
            DLT634_5101_SLAVE_WriteData(pdrv, pBuf, 6);                                     //应答发送
        }
        if(protocol == DLT634_5104)
        {
            DLT634_5104_SLAVE_WriteData(pdrv, pBuf, 6);                                     //应答发送
        }                                               
    }        
}

/**
  * @brief : Monitor Addr Error
  * @param : [pdrv]
  * @return: [none]
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
static void DLT634_ChannelToMonitor_AddError(uint8_t pdrv, uint8_t protocol)
{
    uint8_t pBuf[6];
	
	if (channel_monitor.ByAddr)                                                              //被监听通道地址不为零
    {
        pBuf[0] = _DLT634_ChannelToMonitor_SLAVE_STARTCODE11;                                            //起始码
        pBuf[1] = _DLT634_ChannelToMonitor_AddError;                                            //控制域，回答响应
		pBuf[2] = channel_monitor.ByAddr & 0xff;
        pBuf[3] = (channel_monitor.ByAddr>>8) & 0xff;
        
        pBuf[2+AddrSize] = DLT634_ChannelToMonitor_SLAVE_CKS(pdrv, pBuf);                               //帧校验和
        pBuf[3+AddrSize] = _DLT634_ChannelToMonitor_SLAVE_ENDCODE66;                                     //终止码

        if(protocol == DLT634_5101)
        {
            DLT634_5101_SLAVE_WriteData(pdrv, pBuf, 6);                                     //应答发送
        }

        if(protocol == DLT634_5104)
        {
            DLT634_5104_SLAVE_WriteData(pdrv, pBuf, 6);                                     //应答发送
        }                                               
    }        
}

/**
  * @brief : 11 analysis.监听解析
  * @param : [pdrv]
  * @return: res
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
static uint8_t DLT634_ChannelToMonitor_SLAVE_DecodeFrame11(uint8_t pdrv, uint8_t protocol, uint8_t *RxdBuf)
{
   // memcpy(RxdBuf, CRxdBuf, 6);
    if(protocol == DLT634_5101)                                                             //101解析
    {
        if(pdrv == DLT634_5101SLAVE_DISK0)
        {
            channel_monitor.Addr = SerialPort1;
        }
        if(pdrv == DLT634_5101SLAVE_DISK1)
        {
            channel_monitor.Addr = SerialPort2;
        }
        if (RxdBuf[1] == _DLT634_ChannelToMonitor_SLAVE_FC_ON)                                           //启动通道监听
        {
            channel_monitor.ByAddr = (RxdBuf[3]<<8)|RxdBuf[2];                              //被监测端口地址
            if(channel_monitor.ByAddr != channel_monitor.Addr)
            {
                channel_monitor.MonitorFlag[pdrv] |= _ChannelToMonitor1_START;              //串口启动监听
                DLT634_ChannelToMonitor_SLAVE_SlaveFrame11Response(pdrv, protocol);                          //应答回复
                memset(RxdBuf, 0, 256);
                return 1;
            }  
            else
            {
                DLT634_ChannelToMonitor_AddError(pdrv, protocol);
                memset(RxdBuf, 0, 256);
                return 0;
            }
            
        }
        else if (RxdBuf[1] == _DLT634_ChannelToMonitor_SLAVE_FC_OFF)                            
        {
            channel_monitor.MonitorFlag[pdrv] &= ~_ChannelToMonitor1_START;                 //串口监听关闭
            channel_monitor.ByAddr = (RxdBuf[3]<<8)|RxdBuf[2];
            DLT634_ChannelToMonitor_SLAVE_SlaveFrame11Response(pdrv, protocol);                              //应答回复
            memset(RxdBuf, 0, 256);
            return 0;
        }
        else		                                                                        //错误
        {
            channel_monitor.MonitorFlag[pdrv] &= ~_ChannelToMonitor1_START;
            channel_monitor.ByAddr = 0;
            return 0;
        }
    }
    
    else if(protocol == DLT634_5104)                                                        //104解析
    {
        if(pdrv == DLT634_5101SLAVE_DISK0)
        {
             channel_monitor.Addr = InternetAccess1;
        }
        if(pdrv == DLT634_5101SLAVE_DISK1)
        {
             channel_monitor.Addr = InternetAccess2;
        }
        if (RxdBuf[1] == _DLT634_ChannelToMonitor_SLAVE_FC_ON)                                           //启动通道监听
        {
            channel_monitor.ByAddr = (RxdBuf[3]<<8)|RxdBuf[2];
            if(channel_monitor.ByAddr != channel_monitor.Addr)
            {
                channel_monitor.MonitorFlag[pdrv] |= _ChannelToMonitor2_START;              //串口启动监听
                DLT634_ChannelToMonitor_SLAVE_SlaveFrame11Response(pdrv, protocol);                          //应答回复
                memset(RxdBuf, 0, 256);
                return 1;
            }  
            else
            {
                DLT634_ChannelToMonitor_AddError(pdrv, protocol);
                memset(RxdBuf, 0, 256);
                return 0;
            }
        }
        else if (RxdBuf[1] == _DLT634_ChannelToMonitor_SLAVE_FC_OFF)                                     // 禁止通道监听
        {
            channel_monitor.MonitorFlag[pdrv] &= ~_ChannelToMonitor2_START;
            channel_monitor.ByAddr = (RxdBuf[3]<<8)|RxdBuf[2];
//            DLT634_ChannelToMonitor_SLAVE_SlaveFrame11Response(pdrv, protocol);                              //应答回复
            memset(RxdBuf, 0, 256);
            return 0;
        }
        else		                                                                        //错误
        {
            channel_monitor.MonitorFlag[pdrv] &= ~_ChannelToMonitor2_START;
            channel_monitor.ByAddr = 0;
            return 0;
        }
    }
    else
    {
        return 0;
    }
}

uint8_t RxdBuf[2][256];
/* PUBLIC FUNCTION PROTOTYPES -----------------------------------------------*/
/**
  * @brief : Search monitor Frame.搜索监听
  * @param : [pdrv]
  * @return: res
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
uint8_t DLT634_ChannelToMonitor_SLAVE_SearchMonitorFrame(uint8_t pdrv, uint8_t protocol, uint8_t *CRxdBuf, uint16_t len)
{
    uint8_t stop = 0;
    uint16_t MonitorRxdHead = 0;
    uint16_t MonitorRxdTail = len;
    uint16_t RxdLength=0;
	
	memcpy(RxdBuf[pdrv], CRxdBuf, 256);
	
    while ((MonitorRxdHead < MonitorRxdTail) && (!stop))
    {
        if (channel_monitor.CRxdStatus == CRXDSTART)								        // 寻找起始标志状态
        {
            while (	(RxdBuf[pdrv][MonitorRxdHead]!= _DLT634_ChannelToMonitor_SLAVE_STARTCODE11)&&(MonitorRxdHead < MonitorRxdTail))
            {
                MonitorRxdHead++;									
            }
            if (MonitorRxdHead < MonitorRxdTail) 						                    // 找到启动字符，并将报文与缓冲区对齐。
            {
                channel_monitor.CRxdStatus = CRXDHEAD;				                        //寻找报头
                if (MonitorRxdHead != 0)
                {	
                    memcpy(RxdBuf[pdrv], RxdBuf[pdrv]+MonitorRxdHead, MonitorRxdTail - MonitorRxdHead); //目的地址，原地址，长度
                    MonitorRxdTail -= MonitorRxdHead;
                    MonitorRxdHead = 0;
                }
            }
        }
        RxdLength = MonitorRxdTail - MonitorRxdHead;                                        // 接收一帧数据长度
        if (channel_monitor.CRxdStatus == CRXDHEAD) 									    // 报文够一帧则处理
        {
            if (RxdLength >= 4 + AddrSize)                                                  //一帧长6字节
            {
                if (!DLT634_ChannelToMonitor_SLAVE_CheckFram11Valid(pdrv, RxdBuf[pdrv]))                      //校验
                {
                    MonitorRxdHead++;
                    channel_monitor.CRxdStatus = CRXDSTART;				                    // 寻找起始标志状态
                }
                else
                {
                    channel_monitor.CRxdStatus = CRXDCONTINUE;					            // 接收后续报文
                }
            }
            else
            {
                stop = 1;
            }
        }
        if (channel_monitor.CRxdStatus == CRXDCONTINUE)										//收后续报文
        {
            if (MonitorRxdHead > 0)
            {
                memcpy(RxdBuf[pdrv], RxdBuf[pdrv]+MonitorRxdHead, MonitorRxdTail - MonitorRxdHead);
                MonitorRxdTail -= MonitorRxdHead;
                MonitorRxdHead = 0;
            }
            MonitorRxdHead += 4 + AddrSize;
            channel_monitor.CRxdStatus = CRXDSTART; 
            return (DLT634_ChannelToMonitor_SLAVE_DecodeFrame11(pdrv, protocol, RxdBuf[pdrv]));
        }
    }
        return (0);                        //报文解析  
}

/**
  * @brief : Monitoring data transmission.监测数据发送
  * @param : [pbuf]发送数据地址
  * @param : [count]发送字节长度
  * @param : [ReceiveAndDispatch]接收还是发送
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
void MonitoringDataTransmission(uint8_t *pbuf, uint16_t count, uint8_t ReceiveAndDispatch)
{
   uint8_t Buf[2];					//发送数据标志
	Buf[0] = 0xAA;
	Buf[1] = 0x55;
    switch(channel_monitor.Addr)
    {
        case SerialPort1:                                                                   //串口1
			if(ReceiveAndDispatch == TransmittedData)
			{
				rt_device_write(dev[DLT634_5101SLAVE_DISK0], 0,  Buf, 2); 
			}
            rt_device_write(dev[DLT634_5101SLAVE_DISK0], 0, pbuf, count); 
            break;
        case SerialPort2:                                                                   //串口2
			if(ReceiveAndDispatch == TransmittedData)
			{
				rt_device_write(dev[DLT634_5101SLAVE_DISK1], 0, Buf, 2); 
			}
            rt_device_write(dev[DLT634_5101SLAVE_DISK1], 0, pbuf, count); 
            break;
        case InternetAccess1:                                                               //网口1
			if(ReceiveAndDispatch == TransmittedData)
			{
				lwip_send(1, Buf, 2, 0);
			}
            lwip_send(1, pbuf, count, 0);
            break;
        case InternetAccess2:                                                               //网口2
			if(ReceiveAndDispatch == TransmittedData)
			{
				w5500_send(0, Buf, 2);
			}
            w5500_send(0, pbuf, count);
            break;
    }
}

/**
  * @brief : channel_monitor_init
  * @param : [none]
  * @return: [1]
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
int rt_channel_monitor_init(void)
{
    dev[DLT634_5101SLAVE_DISK0] = rt_device_find(RT_UART4_NAME);    
    dev[DLT634_5101SLAVE_DISK1] = rt_device_find(RT_USART3_NAME); 
    rt_device_open(dev[DLT634_5101SLAVE_DISK0], RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX);
    rt_device_open(dev[DLT634_5101SLAVE_DISK1], RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX);  
    return 1;
}
//INIT_APP_EXPORT(rt_channel_monitor_init);

