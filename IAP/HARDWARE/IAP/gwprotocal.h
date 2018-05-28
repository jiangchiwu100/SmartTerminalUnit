
#ifndef __GWPROTOCAL_H
#define	__GWPROTOCAL_H

#include "stm32f4xx.h"

/* 最大一次接收数据量 */
#define RECEIVE_MAXBYTE	1024
/* 最大一次发送数据量 */
#define SEND_MAXBYTE	1024
/* 主机位 */
#define ISMASTERBIT	0x01
/* 用户号位 */
#define USERNUMBER	0xFE
/* 最大用户数 */
#define MAXUSER_NUM 4
/* 发送队列大小 */
#define USERSEND_MAXNUM 8


typedef enum FrameOffset_{
	FRAME_BEGIN,		
	USERID,
	FRAME_NUMBER,
	DATALEN_L,
	DATALEN_H,
}FrameOffset;


typedef enum ReturnStatus_{
	ISTURE,		/* 失败 */
	ISFAILURE,	/* 成功 */
}ReturnStatus;

enum ReceiveFlag{
	CANRECEIVE,
	REVISBUSYING,
};
struct ReceiveControl{
	enum ReceiveFlag  flag;			/* 是否可以接收标志 */
	uint16_t outTim;				/* 接收完成检测 */
	uint16_t pInCount;				/* 接收计数 */
	uint8_t buff[RECEIVE_MAXBYTE];	/* 接收缓冲区 */
};

typedef enum UserCurrentStatus{
	IDLE,
	PRIVATE,
	SENDING,
	SENDFALSE,
	WAITACK,
	OUTTIME,
	ISACK,
	ACKFALSE,
	ISMASTERCALL,
}UserStatus;
typedef struct UserInfo_{
	uint8_t ID;			/* 用户ID */
	uint8_t number;		/* 帧流水号 */
	UserStatus status;	/* 当前用户状态 */
	uint8_t outTimeFlag;
	uint32_t outTime;	/* 超时时间 */
	uint32_t timConut;	/* 超时时间计时 */
	uint16_t revBuffMax;
	uint8_t *pRevBuff;
	void (*slaveFun)(void);
}UserInfo;

typedef struct UserTab_{
	UserInfo *user[MAXUSER_NUM];
	uint8_t num;
}UserTab;

typedef struct UserSendInfo_{
	UserInfo *user;
	uint8_t *pSendBuff;
}UserSendInfo;
typedef struct UserSendQueue_{
	UserSendInfo user[USERSEND_MAXNUM];
	uint8_t pIn;
	uint8_t pOut;
}UserSendQueue;

typedef union Uint8To16_{
	uint16_t Uint16;
	uint8_t Uint8[2];
}Uint8To16;

typedef union Uint8To32_{
	uint32_t Uint32;
	uint8_t Uint8[4];
}Uint8To32;

extern void Protocalmain(void);
extern void ProtocalInit(void);
/* 底层接收数据接口 */
void ReceiveDealWith(uint8_t data);
/* 底层接收数据一帧完成 */
void ReceiveFinish(void);
/* 添加用户 */
ReturnStatus AddUserInfo(UserInfo *info);
/* 发送填充 */
ReturnStatus UserDataSendFill(UserInfo *sendInfo,uint8_t *pSendBuff);

extern void SendData(uint8_t *pBuff, uint16_t length);
#endif /* __GWPROTOCAL_H */

/* END */
