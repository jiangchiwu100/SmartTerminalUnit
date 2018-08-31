/**
*             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
* @file:      distribution.h
* @brief:     分布式相关的公共定义，避免循环引用
* @version:   V1.0.0
* @author:    Zhang Yufei
* @date:      2018-06-14
* @update:
*/

#ifndef  __DISTRIBUTION_DEF_H
#define  __DISTRIBUTION_DEF_H
#include <stdint.h>
#include "common_def.h"
#include "distribution_enum.h"

#include "list.h"  



/**
* 是否为故障
*/
typedef struct TagFaultInformation
{
    FaultState state; //是否有故障
    bool isFaultEdgeConnected;           //故障区域边缘，且为联络开关路径上
}FaultInformation;
/**
* 检测时标
*update:
* [2018-07-30][张宇飞][创建]
*/
typedef struct TagCheckTimeStamp
{
	bool isValid; //是否超时
	uint32_t updateTime; //更新时间
}CheckTimeStamp;
/**
* 开关属性
*update:
* [2018-07-28][张宇飞][时间属性]
*[2018-07-31][张宇飞][添加onlineStamp]
*/
typedef struct TagSwitchProperty
{
    uint32_t        id;                             //开关编号
    SwitchType      type;                           //开关类型
    SwitchState     state;                          // 开关状态
    uint8_t         neighbourNum;                   //邻接数量
    uint32_t*       neighbourCollect;               //开关邻接关系，反应相邻STU的ID地址
    float           capacity;                       // 开关容量
                                                    //void*           other;                        //其它属性备用
    uint8_t         tempflag;                       //临时使用

    OperateType		operateType;					//操作类型
    OverTimeType    overTimeType;                      //超时类型
    ResultType      removalType;                        //切除类型
	ResultType      insulateType;						//隔离结果
    FaultInformation  fault;                        //故障信息
    struct TagDistributionStation* distributionArea;     // 指向所属的配电区域
    uint8_t          indexArea[2];                         //配电区域内索引 与 配电区域内相对应
    bool             isExitArea[2];                          //对应配电区域是否存在
    bool             isGather[2];                          //是否收集，如果为true则判定对应配电区域

    bool            isPermitOpen;                   //是否允许分闸
    bool            isOpening;                    //是否正在分闸
    bool            isClosing;                 //是否正在合闸
    	
    bool            isChanged;   //是否更新 TODO：完善变换使用 ，注意同步互斥
	
	struct TagStationTopology* parent;

	CheckTimeStamp timeStamp; //有效时间戳——用于指示更新时间
	CheckTimeStamp onlineStamp; //在线时间戳——用于指示所有在线

	void* pDeviceIndicate; // 绑定的设备指示
}SwitchProperty;





/**
* 拓扑片信息
*/
typedef struct TagTopologyMessage
{
    uint32_t         id; //由 4bytes IP组成的地, IP高位在前
    uint8_t          idArray[6]; //拓扑片ID号数组表示形式
    TopologyType     type;  //拓扑片类型
    uint8_t          switchNum; //开关数量
    SwitchProperty*  switchCollect;//开关集合

}TopologyMessage;


/**
* 广度优先搜索辅助类
*/
typedef struct TagBFSHelper
{
    uint8_t size;  //大小
    uint32_t* idArray;
    uint8_t* edgeTo; //到达数组
    uint8_t* marked;
    ListDouble* path;  //路径保存
}BFSHelper;

/**
* 配电区域 相关信息
*/
typedef struct TagDistributionPowerArea
{
    //ListDouble powerArea;             //配电区域列表
    SwitchProperty** areaSwitch; // 配电区域列表 数组形式
    uint8_t switchNum;            //开关数量

    uint32_t upadetedFlag;        //按照 areaSwitch索引置位或者复位，表示更新完毕upadetedFull
    uint8_t  isUpadeted;          //更新完成标志

    bool isFaultArea;            //是否为故障区域
    bool isExitFaultMessage;        //配电区域内，是否存在故障信息

    uint32_t upadetedFull;      //(2^switchNum - 1)
	ResultType removalType;    //是否切除成功，代表本配电区域切除成功或者下游切除成功
	ResultType insulateType;    //是否异常成功
}DistributionPowerArea;

/**
*  开关节点的配电区域信息
*/
typedef struct TagDistributionStation
{
    ListDouble powerAreaList[POWER_AREA_NUM];
    DistributionPowerArea powerArea[POWER_AREA_NUM];  //配电区域相关信息
    uint8_t areaCount;                                 //配电区域个数
    bool isComplted; //是否已经实现 注意需要维护
	bool isGatherCompleted; //是否收集完成
	bool isGatherCalculateCompleted;//是否收集计算完成
    SwitchProperty* switchRef; //参考开关
	bool isAlreayExitedFault;// 是否已经存在故障区域
	bool isExitedInsulateFailure;//隔离是否失败

    ErrorCode (*StartUpdate)(SwitchProperty* switchProperty);
    ErrorCode (*UpdatePowerArea)(SwitchProperty* switchProperty);
    ErrorCode (*JudgeUpdateCompleted)(SwitchProperty* switchProperty, bool* updated);
    ErrorCode (*JudgeIsExitFaultArea)(struct TagDistributionStation* distribution, bool iscover); //判断是否存在故障区域
	void (*GatherCompletedAndJudgeFaultArea)(struct TagDistributionStation* distribution, SwitchProperty* switchProperty);
	bool (*IsAlreayExitedFault)(struct TagDistributionStation* distributionArea);
	bool(*JudgeIsExitFault)(SwitchProperty* switchProperty);//判断是否存在故障信息
    ErrorCode (*SignExitFaultMessage)(SwitchProperty* switchProperty);
	ErrorCode(*SignRemovalMessage)(SwitchProperty* switchProperty, ResultType reult);
	ErrorCode(*SignInsulateMessage)(SwitchProperty* switchProperty);
}DistributionStation;
/**
*  整个组网区域ID信息
*/
typedef struct TagAreaID
{
    uint8_t count; //开关数量
    uint32_t* idCollect;//ID合集
    bool isGainComplted; //是否获取完整
}AreaID;
/**
*  联络开关信息
*@update[2018-7-30][张宇飞][增加是否闭锁]
*/
typedef struct TagConnectSwitch
{
    //ListDouble bfsPower;// 以此开关开始到电源开关的路径，全部为合位
	TransferCode transferCode; //转供电
    ListDouble path[2]; //路径数组  SwitchProperty 电源开关路径 ，存储的为开关属性节点
    uint8_t count;// 路径数量
    bool isConnect; //是否为联络开关
	bool isLock;//是否闭锁判断
}ConnectSwitch;

/**
*  开关所在路径上的联络开关的信息，ID，
*/
typedef struct TagConnectPath
{
	uint32_t id; //id
	uint32_t remainderCapacity; //剩余容量
	uint8_t  hopsNumber; //到联络开关的跳数
	bool isUpdated; //是否更新
	CheckTimeStamp timeStamp;
}ConnectPath;



/**
*  开关节点站信息
* 添加有效监测isValidAll 所有开关列表
*@update [2018-7-31][张宇飞][增加systemOnlineStamp]
*@update [2018-8-31][张宇飞][增加isRunDistribution]
*/
typedef struct TagStationTopology
{
    TopologyMessage*  localTopology;      //本地（当前）拓扑
    SwitchProperty *  localSwitch;       //本地（当前）开关节点
    TopologyMessage*  neighbourTopology;  //邻居拓扑

    TopologyMessage*  aimTopology; //目的拓扑

    ListDouble globalTopologyList;   //全局拓扑列表
    ListDouble neighbourSwitchList; //邻居开关列表
    ListDouble globalSwitchList;   //全局开关列表
	bool isValidAll;//globalSwitchList是否全部有效

	CheckTimeStamp systemOnlineStamp;//系统在线时标


    bool isNeighbourComplted; //邻居是否完整 //注意需要维护
	
    DistributionStation powerArea; //配电区域信息


    BFSHelper* BFSResult;

   

    uint32_t id;  //ID 区分不同站点拓扑

    ConnectSwitch connect;//联络开关维护判别所需信息
	
    
    ListDouble connectPath; //联络开关路径上判别,有成员则是，否则不是 ConnectPath
	bool isConnectPathUpdatedComplted; //更新完毕,用于转供电
	bool isValidConnectPath;//信息是否全部有效


    AreaID areaID;// 区域ID合集

    bool isRunDistribution;  //是否允许允许分布式保护
}StationTopology;



/**
*缓冲队列信息
*/
typedef struct TagNodeFifo
{
    FifoHandle reciveHandle; //接收
    FifoHandle sendHandle;   //发送
    uint32_t id;       //ID  
    uint8_t (*Send)(FifoHandle* handle , PointUint8*); //发送函数
}NodeFifo;

/**
*传输节点
*/
typedef struct TagDatagramTransferNode
{
	RingQueue  reciveRing; //接收
	RingQueue  sendRing;   //发送
	uint32_t id;       //ID  
	ErrorCode (*Write)(RingQueue* ring, PointUint8*); //发送函数
	ErrorCode (*Send)(struct TagDatagramTransferNode* node, PointUint8*); //发送函数
}DatagramTransferNode;
/**
*故障移除判别控制体,字节对齐提高效率
*/
typedef struct TagFaultDealHandle
{
    SwitchProperty* switchProperty; //开关属性
    FaultDealJudgeState state; //当前处理状态
    FaultDealJudgeState lastState; //上一个状态
    FaultDealJudgeState nextState; //下一个状态

    bool isRun; //是否正在运行 正在进行切除等一系列动作

    uint8_t step; //状态内的处理阶段
    uint32_t startTime; //开始时间
    uint32_t limitTime; //限制时间
                        //限制时间合集
    uint32_t t1;     //收集信息总时间
    uint32_t t2;     //限时检测时间
    uint32_t t3;     //延时检测时间
    uint32_t t4;     //收集总时间
    uint32_t t5;     //收集总时间


    uint32_t checkOpenTime;//分闸检测时间
    uint32_t checkBackupTime;//后备保护检测时间
    uint32_t rejectTime;     //拒分后的检测测时间
	uint32_t reciveRemovalSuccessTime;//收到切除成功时间
	uint32_t reciveConnectTime; //接收联络开关信息超时时间
   
   
	DatagramTransferNode* pTransferNode; //用于收发控制

    bool isCheckPass; //自检检测通过

    //函数合集
    bool (*IsFault)(struct TagFaultDealHandle* handle);
	bool (*IsPowerIncomeLossFault)(struct TagFaultDealHandle* handle);
    bool (*IsOpenPosition)(struct TagFaultDealHandle* handle);
	bool(*IsClosePosition)(struct TagFaultDealHandle* handle);
    bool (*IsTrigger)(struct TagFaultDealHandle* handle);
    bool (*IsGatherCompleted)(struct TagFaultDealHandle* handle);
    bool (*IsFaultEdgeConnected)(struct TagFaultDealHandle* handle); //故障区域边缘，且为联络开关路径上
    bool (*IsFaultArea)(struct TagFaultDealHandle* handle); //
	bool(*IsRejectArea)(struct TagFaultDealHandle* handle); //是否为故障区域
	bool(*IsRejectInsulate)(struct TagFaultDealHandle* handle);
	bool(*IsRemovalSuccessInsulate)(struct TagFaultDealHandle* handle); //是否为故障区域
	bool(*IsRemovalSuccessInsulateBackup)(struct TagFaultDealHandle* handle); //是否为故障区域
	bool(*IsRemovalSuccessBackup)(struct TagFaultDealHandle* handle); //是否为故障区域
	 //uint8_t (*IsFaultHigherUpNeighbour)(struct TagFaultDealHandle* handle); //是否为故障上一级邻居
    bool (*IsPermitOpen)(struct TagFaultDealHandle* handle); //是否可以进跳闸操作
    bool (*OpenOperate)(struct TagFaultDealHandle* handle); //分闸操作
	bool (*CloseOperate)(struct TagFaultDealHandle* handle); //合闸操作
	bool (*IsPermitInTransferPowerMode)(struct TagFaultDealHandle* handle);//是否允许进行转供电模式
    uint32_t (*DiffTime)(uint32_t lastTime);//当前时间与上一个时间的差值
    bool (*GetNowTime)(struct TagFaultDealHandle* handle);
    bool (*IsOverTime)(struct TagFaultDealHandle* handle);

    bool (*TransmitMessage)(struct TagFaultDealHandle* handle, FuncionCode code);
    uint8_t (*TransmitData)(struct TagFaultDealHandle* handle, PointUint8* packet);
    bool (*Reset)(struct TagFaultDealHandle* handle);
}FaultDealHandle;


/**
*路由器
*/
typedef struct TagRouter
{
    ListDouble nodeStation; //数据节点管理列表，每个需要管理的站点 NodeFifo
    NodeFifo* (*FindMemberById)(const  ListDouble* list, uint32_t id);

}Router;





typedef struct TagFrameRtu
{
    uint16_t address; //地址
    uint16_t destAddress; //目的地址
    uint8_t funcode; //功能代码
    uint16_t datalen; //数据长度
    uint8_t* pData; //指向数据指针
    uint8_t* pValidData; //指向发指向有效数据部分
    bool completeFlag; //0-未完成
} FrameRtu;

/**
* 数据分析器
*/
typedef struct TagProtocolAnylast
{
    FifoHandle* fifohanlde;
    FifoHandle* sendFifohanlde;
    uint8_t step;
    uint16_t reciveIndex;
    uint8_t reviceFrame[RECIVE_FRAME_MAX]; //简化处理使用固定数组，每次仅处理一帧
    uint16_t  frameCapcity;

    uint16_t address;

    FrameRtu sendRtu;
    FrameRtu recvRtu;


    uint8_t (*ProtocolAnylastDeal)(struct TagProtocolAnylast* anylast);
    uint8_t (*SendData)(struct TagProtocolAnylast*, uint8_t* buffer, uint16_t len);
}ProtocolAnylast;



/**
* 数据分析器--数据包
*/
typedef struct TagProtocolAnylastDatagram
{
	
	DatagramTransferNode* pTransferNode;

	DatagramFrame* pReiveDatagram;

	uint16_t address;

	FrameRtu sendRtu;
	FrameRtu recvRtu;

	ErrorCode(*ProtocolAnylastDeal)(struct TagProtocolAnylastDatagram* anylast);	
}ProtocolAnylastDatagram;
/**
*仿真站状态
*/
typedef struct TagSimulationStation
{
    uint32_t id;   //ID

    FaultState   faultState;    //故障状态
    SwitchState  switchState;   //开关状态


    uint32_t  closeTime;        //合闸时间
    uint32_t  openTime;        //合闸时间


    SwitchRunState runState;   //运行状态
    SwitchRunState nextRunState;   //运行状态


    uint8_t isStartClose;   //启动合闸
    uint8_t isStartOpen;    //启动分闸

    uint32_t startTime;    //开始时间
    uint32_t limitTime; //限制时间


	//故障设置
	bool isRejectAction;// 是否拒绝动作（合闸或者分闸）


    uint8_t(*GetNowTime)(struct TagSimulationStation*);
    uint8_t(*IsOverTime)(struct TagSimulationStation*);
    void(*OpenOperate)(struct TagSimulationStation*);   //合闸操作
    void(*CloseOperate)(struct TagSimulationStation*);  //分闸操作

    SwitchProperty** pBindSwitch;   //关联开关

}SimulationStation;

/**
* 开关站服务
*/
typedef struct TagSimulationStationServer
{
    ListDouble SimulationStationList; //模拟开关站列表


}SimulationStationServer;






/**
* 开关站管理器
*/
typedef struct TagStationPoint
{
    uint32_t id;   //ID

    StationTopology topology;  //拓扑
    
    FaultDealHandle removalHandle; // 故障切除模块
    
	DatagramTransferNode transferNode;//
	ProtocolAnylastDatagram  anylast;

	void* server61850; //61850对应服务
    bool isAllowUpdate; //是否允许开入更新

}StationPoint;

/**
* 开关站服务器
*/
typedef struct TagStationServer
{
    ListDouble stationPointList; // 站点列表 StationPoint
    StationPoint* (*FindMemberById)(const  ListDouble* list, uint32_t id);  //通过ID查找成员


}StationServer;
/**
* 开关站管理器
*/
typedef struct TagStationManger
{
    StationServer stationServer; //站点服务器
    SimulationStationServer  simulationServer; //仿真开关服务
											   

	StationPoint* pWorkPoint; //工作站点
	SimulationStation* pWorkSimulation; //工作模拟站点
    bool firstRun; //首次运行
    bool isMaintanceRun; //维护正在运行
   
}StationManger;

#define GetDeviceIndicate(handle) ((DeviceIndicate*)(handle->switchProperty->pDeviceIndicate))
#define GetDeviceIndicateBySwitch(sw) ((DeviceIndicate*)(sw->pDeviceIndicate))


#endif // ! __DISTRIBUTION_H
