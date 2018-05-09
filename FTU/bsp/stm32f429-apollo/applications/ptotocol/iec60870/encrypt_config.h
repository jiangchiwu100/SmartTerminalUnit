/*
** ------------------------------- Copyright (c) -------------------------------                                     
**
**                           http://www.sojoline.com
**
** note:    Copyright SOJO. ALL RIGHTS RESERVED.
**          This software is provided under license and contains proprietary and
**          confidential material which is the property of Company Name tech. 
**    
** -------------------------------- File Info ----------------------------------
** File name:               encrypt_config.h
** Descriptions:            The config of encrypt
**
** -----------------------------------------------------------------------------
** Created by:              Mr.j
** Created date:            2015.10.25
** System:                  Protocol Software System
** Version:                 V3.01
** Descriptions:            The original version
**
** -----------------------------------------------------------------------------
** Modified by:             
** Modified date:           
** Version:                 
** Descriptions:            
**
** ---------------------------------------------------------------------------*/
#ifndef	_ENCRYPT_CONFIG_H_
#define _ENCRYPT_CONFIG_H_


/* PUBLIC VARIABLE -----------------------------------------------------------*/
typedef enum
{
    ENCRYPT_D0 = 0,
    ENCRYPT_D1,
    _ENCRYPT_VOLUMES
}ENCRYPT_DISK;

#define _ENCRYPT_LPDUSIZE  512
#define _ENCRYPT_STOREDATA1NUM	10

/* 链路层帧修饰词***************************************************************/
#define  _ENCRYPT_STARTCODE  	                     0xEB	// 起始码
#define  _ENCRYPT_ENDCODE                          0xD7	// 终止码

#define  _ENCRYPT_OBJ                              0xc0 // 连接对象
#define  _ENCRYPT_ENC                              0x08 // 加密标识

#define  _ENCRYPT_OBJ_B                              0x00 // 主站
#define  _ENCRYPT_OBJ_M                              0x40 // 现场运维工具
#define  _ENCRYPT_OBJ_G                              0x80 // 网关

//LINK_FLAG
#define  _ENCRYPT_INITIAL                	           0x00800000//加密程序初始化
#define  _ENCRYPT_SANDABLE                	           0x00010000//加密程序可发送数据

#define  _ENCRYPT_G_CERSUCCESS                  	   0x80000000//网关认证成功
#define  _ENCRYPT_B_CERSUCCESS                  	   0x40000000//后台认证成功
#define  _ENCRYPT_M_CERSUCCESS                  	   0x20000000//证书管理工具认证成功

//应用类型IEC/Gateway/Backstage/Maintain
//业务安全处理结果返回	
#define   _ENCRYPT_S_FUN1F             	0x1F	         // 业务安全处理结果返回

//IEC																									 // 应用数据/时间(6)/随机数(8)/签名结果(65)
#define   _ENCRYPT_I_FUN00             	0x00           // 含      /不含   /不含     /不含
#define   _ENCRYPT_I_FUN01              0x01           // 含      /不含   /不含     /含
#define   _ENCRYPT_I_FUN02              0x02	         // 含      /不含   /含       /不含
#define   _ENCRYPT_I_FUN03             	0x03	         // 含      /不含   /含       /含
#define   _ENCRYPT_I_FUN04             	0x04           // 含      /含     /不含     /不含
#define   _ENCRYPT_I_FUN05              0x05           // 含      /含     /不含     /含
#define   _ENCRYPT_I_FUN06              0x06	         // 含      /含     /含       /不含
#define   _ENCRYPT_I_FUN07             	0x07	         // 含      /含     /含       /含
#define   _ENCRYPT_I_FUN08              0x08	         // 不含    /含     /含       /含

//网关
#define   _ENCRYPT_G_FUN20             	0x20           // 认证网关对终端的认证请求
#define   _ENCRYPT_G_FUN21              0x21           // 终端认证确认并请求网关认证
#define   _ENCRYPT_G_FUN22              0x22	         // 网关对终端认证请求的响应
#define   _ENCRYPT_G_FUN23             	0x23	         // 终端向网关返回认证结果

//后台
#define   _ENCRYPT_B_FUN50             	0x50           // 主站对终端的认证请求
#define   _ENCRYPT_B_FUN51              0x51           // 终端认证确认并请求主站认证
#define   _ENCRYPT_B_FUN52              0x52	         // 主站对终端认证请求的响应
#define   _ENCRYPT_B_FUN53             	0x53	         // 终端向主站返回认证结果
#define   _ENCRYPT_B_FUN54              0x54	         // 主站获取终端芯片序列号
#define   _ENCRYPT_B_FUN55             	0x55	         // 终端返回内置芯片序列号

#define   _ENCRYPT_B_FUN60             	0x60           // 主站获取终端密钥版本
#define   _ENCRYPT_B_FUN61             	0x61           // 终端向主站返回密钥版本
#define   _ENCRYPT_B_FUN62             	0x62           // 主站执行远程密钥更新
#define   _ENCRYPT_B_FUN63             	0x63           // 终端向主站返回密钥更新结果
#define   _ENCRYPT_B_FUN64             	0x64           // 主站执行远程密钥恢复
#define   _ENCRYPT_B_FUN65             	0x65           // 终端向主站返回密钥恢复结果

#define   _ENCRYPT_B_FUN70             	0x70           // 主站向终端发起证书远程更新
#define   _ENCRYPT_B_FUN71             	0x71           // 终端向主站返回证书远程更新结果
#define   _ENCRYPT_B_FUN72             	0x72           // 主站向终端发起远程下载终端证书
#define   _ENCRYPT_B_FUN73             	0x73           // 终端向主站返回证书远程下载结果
#define   _ENCRYPT_B_FUN74             	0x74           // 主站提取终端证书
#define   _ENCRYPT_B_FUN75             	0x75           // 终端返回证书
#define   _ENCRYPT_B_FUN76             	0x76           // 主站向终端返回证书提取结果

//证书管理工具
#define   _ENCRYPT_M_FUN30             	0x30           // 证书管理工具发起认证请求
#define   _ENCRYPT_M_FUN31              0x31           // 终端响应证书管理工具的认证请求
#define   _ENCRYPT_M_FUN32              0x32	         // 证书管理工具对终端认证请求的响应
#define   _ENCRYPT_M_FUN33             	0x33	         // 终端向证书管理工具返回认证结果

#define   _ENCRYPT_M_FUN34              0x34	         // 获取终端密钥版本
#define   _ENCRYPT_M_FUN35             	0x35	         // 终端返回密钥版本

#define   _ENCRYPT_M_FUN36              0x36	         // 获取终端序列号
#define   _ENCRYPT_M_FUN37              0x37           // 终端返回终端序列号

#define   _ENCRYPT_M_FUN38              0x38	         // 获取终端内置安全芯片序列号
#define   _ENCRYPT_M_FUN39              0x39           // 终端返回内置安全芯片序列号

#define   _ENCRYPT_M_FUN3A              0x3A	         // 获取终端安全芯片公钥
#define   _ENCRYPT_M_FUN3B              0x3B           // 终端返回终端安全芯片公钥加密报文

#define   _ENCRYPT_M_FUN3C              0x3C	         // 证书管理工具将生成的证书请求文件发给终端
#define   _ENCRYPT_M_FUN3D              0x3D           // 向证书管理工具返回签名结果

#define   _ENCRYPT_M_FUN3E              0x3E	         // 证书管理工具将配电应用证书导入终端
#define   _ENCRYPT_M_FUN3F              0x3F           // 终端返回证书导入结果

#define   _ENCRYPT_M_FUN40              0x40	         // 终端初始证书会写
#define   _ENCRYPT_M_FUN41              0x41           // 终端返回初始证书会写结果

#define   _ENCRYPT_M_FUN42              0x42	         // 导出终端证书
#define   _ENCRYPT_M_FUN43              0x43           // 终端向证书管理工具返回终端证书
#define   _ENCRYPT_M_FUN44              0x44           // 证书管理工具向终端返回证书导出结果

#define   _ENCRYPT_M_FUN45              0x45           // 分帧数据传输结果返回

#define   _ENCRYPT_M_FUN46              0x46           // 证书管理工具恢复终端密钥
#define   _ENCRYPT_M_FUN47              0x47           // 密钥恢复结果

#define   _ENCRYPT_M_FUN48              0x48           // 证书管理工具用现场运维下行密钥对配电数据加密并发送
#define   _ENCRYPT_M_FUN49              0x49           // 终端用现场运维上行密钥对配电数据加密并发送

/* 错误码定义 ********************************************************************************/ 
//业务扩展错误码
#define   _ENCRYPT_ERROR_CODE9101      9101					// 业务应用类型错误
#define   _ENCRYPT_ERROR_CODE9102      9102					// 报文验签失败
#define   _ENCRYPT_ERROR_CODE9103      9103					// 报文解密失败
#define   _ENCRYPT_ERROR_CODE9104      9104					// 随机数验证失败
#define   _ENCRYPT_ERROR_CODE9105      9105					// 时间校验失败
#define   _ENCRYPT_ERROR_CODE9106      9106					// 需要加密报文未加密
#define   _ENCRYPT_ERROR_CODE9107      9107					// 业务安全流程非法
#define   _ENCRYPT_ERROR_CODE9108      9108					// 权限不够
#define   _ENCRYPT_ERROR_CODE9109      9109					// 未知错误
#define   _ENCRYPT_ERROR_CODE9110      9110					// 报文长度有误或解析失败

//安全扩展错误码
#define   _ENCRYPT_ERROR_CODE9000      9000					// 成功
#define   _ENCRYPT_ERROR_CODE9090      9090					// 认证失败
#define   _ENCRYPT_ERROR_CODE9091      9091					// 密钥更新失败
#define   _ENCRYPT_ERROR_CODE9092      9092					// 密钥恢复失败
#define   _ENCRYPT_ERROR_CODE9093      9093					// 证书导入失败
#define   _ENCRYPT_ERROR_CODE9094      9094					// 证书导出失败
#define   _ENCRYPT_ERROR_CODE9095      9095					// 证书提取失败
#define   _ENCRYPT_ERROR_CODE9096      9096					// 分帧数据接受失败
#define   _ENCRYPT_ERROR_CODE9097      9099					// 证书远程更新下载失败

/* TI类型标识 ********************************************************************************/ 
//监视方向过程信息
#define   _ENCRYPT_M_SP_NA_1              1            // 单点信息
#define   _ENCRYPT_M_DP_NA_1              3            // 双点信息
#define   _ENCRYPT_M_ME_NA_1              9            // 测量值，归一化值
#define   _ENCRYPT_M_ME_NB_1              11           // 测量值，标度化值
#define   _ENCRYPT_M_ME_NC_1              13           // 测量值，短浮点数
#define   _ENCRYPT_M_SP_TB_1              30           // 带时标的单点信息
#define   _ENCRYPT_M_DP_TB_1              31           // 带时标的双点信息
#define   _ENCRYPT_M_FT_NA_1              42           // 故障事件信息
#define   _ENCRYPT_M_IT_NB_1              206          // 累计量，短浮点数
#define   _ENCRYPT_M_IT_TC_1              207          // 带时标累计量，短浮点数

//控制方向过程信息
#define   _ENCRYPT_C_SC_NA_1              45            // 单点命令
#define   _ENCRYPT_C_SC_NB_1              46            // 双点命令

//监视方向的系统命令
#define   _ENCRYPT_M_EI_NA_1              70            // 初始化结束

//控制方向的系统命令
#define   _ENCRYPT_C_IC_NA_1              100            // 站总召唤命令
#define   _ENCRYPT_C_CI_NA_1              101            // 电能量召唤命令
#define   _ENCRYPT_C_CS_NA_1              103            // 时间同步命令
#define   _ENCRYPT_C_TS_NA_1              104            // 测试命令
#define   _ENCRYPT_C_RP_NA_1              105            // 复位进程命令
#define   _ENCRYPT_C_SR_NA_1              200            // 切换定值区
#define   _ENCRYPT_C_RR_NA_1              201            // 读定值区号
#define   _ENCRYPT_C_RS_NA_1              202            // 读参数和定值
#define   _ENCRYPT_C_WS_NA_1              203            // 写参数和定值
#define   _ENCRYPT_F_FR_NA_1              210            // 文件传输
#define   _ENCRYPT_F_SR_NA_1              211            // 软件升级



#endif /* END _ENCRYPT_CONFIG_H_ */
    

/* END OF FILE ---------------------------------------------------------------*/
