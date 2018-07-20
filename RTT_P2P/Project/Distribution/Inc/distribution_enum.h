/**
*             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
* @file:      distribution_enum.h
* @brief:     放置枚举常量，统一管理
* @version:   V1.0.0
* @author:    Zhang Yufei
* @date:      2018-06-14
* @update:
*/

#ifndef  __DISTRIBUTION_ENUM_H
#define  __DISTRIBUTION_ENUM_H

#define NULL 0
//#define NULL_PTR (void*)0

/**
* 配电区域最大数量
*/
#define POWER_AREA_NUM   2


//用于RTU帧
#define RECIVE_FRAME_MAX  128

#define  FRAME_DATALEN_INDEX 2 //数据长度开始索引
#define  FRAME_VALID_INDEX 11
#define  FRAME_SOURCE_INDEX 6
#define  FRAME_DEST_INDEX 8
#define  FRAME_MIN_LEN  14
#define  FRAME_FUNCODE_INDEX 10
#define  FRAME_END_LEN   4 //3+1
#define  FRAME_CRC_OFFSET     11 //datalen


#define SEND_FRAME_LEN 64UL

#define LOCAL_ADDRESS   0xAA

#define START_CHAR1 0x65
#define START_CHAR2 0xA6
#define START_CHAR3 0xA6
#define START_CHAR4 0x65
#define END_CHAR    0x1A

/**
* 拓扑片类型定义
*/
typedef enum TagTopologyType
{
    TOPOLOGY_START = 0,
    TOPOLOGY_TYPE_SINGLE, //单开关
    TOPOLOGY_TYPE_SINGLE_SINGLE, //单母线单终端
    TOPOLOGY_TYPE_SINGLE_DOUBLE, //单母线双终端
    TOPOLOGY_TYPE_DISCRATE_DOUBLE, //分段母线开关
    TOPOLOGY_LENGTH //类型数量-1

}TopologyType;

/**
* 开关类型定义
*/
typedef enum TagSwitchType
{
    SWITCH_TYPE_LOAD = 0x10,//负荷   
    SWITCH_TYPE_LOAD_POWER, //负荷--电源开关
    SWITCH_TYPE_LOAD_BRANCH,  //负荷--分支开关
    SWITCH_TYPE_LOAD_SECTION,  //负荷--分段开关
    SWITCH_TYPE_LOAD_BUS,  //负荷--母联开关
    SWITCH_TYPE_LOAD_INTERCONNECTION, //负荷联络开关

    SWITCH_TYPE_BREAKER = 0x40,//断路器
    SWITCH_TYPE_BREAKER_POWER, //断路器--电源开关
    SWITCH_TYPE_BREAKER_BRANCH,  //断路器--分支开关
    SWITCH_TYPE_BREAKER_SECTION,  //断路器--分段开关
    SWITCH_TYPE_BREAKER_BUS,  //断路器--母联开关
    SWITCH_TYPE_BREAKER_INTERCONNECTION, //断路器--联络开关


}SwitchType;


/**
* 故障信息枚举
*/
typedef enum TagFaultState
{
    FAULT_YES = 0x01, //有故障
    FAULT_NO = 0x02, //无故障
	FAULT_INCOME_LOSS = 0x11, //进线失压故障
}FaultState;
/**
*动作信息
*/
typedef enum TagOperateType
{
    OPERATE_NULL = 0, //空
    OPERATE_OPEN = 1, //分
    OPERATE_CLOSE = 2, //合
    OPERATE_REJECT_CLOSE = 11, //拒合
    OPERATE_REJECT_OPEN = 12, //拒分
}OperateType;

/**
* 开关合分位定义
*/
typedef enum TagSwitchState
{
    SWITCH_CLOSE = 1, //合位
    SWITCH_OPEN = 2, //分位   
}SwitchState;


/**
*超时枚举类型
*/
typedef enum TagOverTimeType
{
    OVER_TIME_NULL = 0,
    OVER_TIME_REMOVAL = 1, //切除超时
    OVER_TIME_GATHER = 2, //收集超时	
}OverTimeType;

/**
*切除信息
*/
typedef enum TagResultType
{
    RESULT_TYPE_NULL = 0, //NULL
    RESULT_SUCCESS = 1, //成功
    RESULT_FAILURE = 3, //失败

}ResultType;


/**
*控制操作
*/
typedef enum TagSwitchControlOperate
{
    CONTROL_CLOSE = 1, //合闸
    CONTROL_OPEN= 2, //分闸
    CONTROL_SET_OVERCURRENT = 3, //设置过流
    CONTROL_REMOVAL_RESET = 4,
	CONTROL_SET_REJECT_ACTION = 5,
	CONTROL_CANCER_REJECT_ACTION = 6,
	CONTROL_CANCER_OVERCURRENT = 7, //设置过流
	CONTROL_POWER_INCOME_LOSS = 8, //电源进线失压
}SwitchControlOperate;


/**
*获取信息命令
*/
typedef enum TagGetMessageCmd
{
    GET_TOPOLOGY = 0x01, //获取信息
    GET_SWITCH = 0x02,//获取开关
}GetMessageCmd;

typedef enum TagTransferCode
{
    TRANSFER_NULL = 0, //空
	TRANSFER_REMAINDER_CAP = 1, //剩余容量
	TRANSFER_SET = 2, //启动转供电
	TRANSFER_CANCER = 3, //不进行转供电
}TransferCode;


/**
*功能码
*/
typedef enum TagFuncionCode
{
    UPDATE_NEIGHBOUR = 0x01, //更新邻居配置信息
    GET_MESSAGE = 0x02, //获取信息
    REPLY_MESSAGE = 0x03, //应答信息


    LOCAL_MESSAGE = 0x05, //本地信息
    AIM_MESSAGE = 0x06, //目的信息
    STATUS_MESSAGE = 0x07, //故障信息
    REMOVAL_MESSAGE = 0x08, //切除信息
    NANOPB_TYPE = 0x09, //NANOPB 信息
    GET_LOG = 0x0A, //获取日志
    CONNECTED_MESSAGE = 0x0B, //联络开关
	INSULATE_MESSAGE = 0x0C, //隔离信息
	TRANSFER_MESSAGE = 0x0D, //转供电信息

    CAL_POWER_AREA = 0x10, //计算配电区域
    CAL_PATH = 0x11, //计算路径
    CAL_PATH_ALL = 0x12, //计算路径
    DELETE_ONE = 0x20, //删除指定拓扑节点   

    SWITCH_OPERATE = 0x30, //开关操作


    ADD_STATION = 0x40, //增加站点

    GET_ALL = 0x50, //获取所有开关信息
    JUDGE_CONNECT = 0x51, //判断联络开关



}FuncionCode;

/**
*状态结果定义
*/
typedef enum TagStateResult
{
    RESULT_NULL = 0,          //正常


    RESULT_ERROR = 0x50,      //错误
    RESULT_ERROR_MATCH = 0x51,    //不匹配
    RESULT_ERROR_EXIST = 0x52,   //不存在的状态

    RESULT_ACCELERATE = 0xA2,   //加速 
    RESULT_TIME = 0xA3,   //存在时间检测


}StateResult;

/**
*故障移除判据状态定义
*/
typedef enum TagFaultDealJudgeState
{
	REMOVAL_START = 0x21, 	//故障移除开始
	REMOVAL_GATHER, 		//故障信息收集状态	
	REMOVAL_TREATMENT, 		//故障处理状态
	REMOVAL_CENTER_DECTECT, 	//控制跳闸后的检测状态
	REMOVAL_DELAY_GATHER, 	//延时故障收集状态
	REMOVAL_BACKUP,   		//后备保护状态	
	REMOVAL_BACKUP_DECTECT,  //后备保护跳闸后的检测状态
	


	REMOVAL_EXIT = 0x31, 		            //恢复原始监控
	REMOVAL_OVERTIME,       //超时处理状态

	INSULATE_DEAL = 0x51, // 移除处理


	TRANSFER_MASTER = 0x71, // 转供电--主控制（切除成功开关）
	TRANSFER_CONNECT = 0x72, // 转供电--从(联络开关)
	
	DISTRIBUTION_LOCK = 0x91, 	    //分布式保护功能闭锁
	PROTECT_LOCK,           //保护功能闭锁

    REMOVAL_STOP = 0xFF ,               //停止 测试使用
}FaultDealJudgeState;

/**
*仿真开关动作状态
*/
typedef enum  TagSwitchRunState
{
    RUN_STATE_CLOSE_POSITION_STORED = 0xA1, //合闸储能状态
    RUN_STATE_OPEN_ACTION,              //分闸动作状态
    RUN_STATE_OPEN_POSITION_UNSTORED,    //分闸未储能
    RUN_STATE_OPEN_POSITION_STORED,      //分闸储能状态
    RUN_STATE_CLOSE_ACTION,              //合闸动作状态

}SwitchRunState;

/**
*联络开关路径上的开关
*/
typedef enum TagPathConnected
{
    SET_PATH_CONNECT = 1,
    CANCER_PATH_CONNECT = 2,
}PathConnected;

/**
*隔离子状态
*/
typedef enum TagInsulateState
{
	INSULATE_INIT = 0, //隔离初始
	INSULATE_IS_REMOVE = 1, //是否移除成功判别
	INSULATE_DECT = 2, //延时跳闸侦测
	INSULATE_BACKUP = 3, //后备状态
	INSULATE_AREA_BACKUP = 4, //隔离区域备份状态
	INSULATE_BACKUP_IS_MEET = 5, //隔离区域备份状态是否满足条件
}InsulateState;




#endif // 
