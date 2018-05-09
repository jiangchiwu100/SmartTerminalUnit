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
** File name:               encrypt_app.c
** Descriptions:            The application of encrypt
**
** -----------------------------------------------------------------------------
** Created by:              Mr.J
** Created date:            2017.5.16
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


/* INCLUDE FILES -------------------------------------------------------------*/
#include "encrypt_app.h"
#include "encrypt_disk.h"

/* PUBLIC VARIABLES ----------------------------------------------------------*/
ENCRYPT_APPINFO  *Encrypt_App = RT_NULL;

/* PRIVATE FUNCTION PROTOTYPES -----------------------------------------------*/

/* PUBLIC FUNCTION PROTOTYPES -----------------------------------------------*/

static BYTE Encrypt_CheckFramValid(BYTE pdrv)
{
    BYTE *pBuf = 0;
    
    pBuf = (Encrypt_App[pdrv].RxdBuf + Encrypt_App[pdrv].RxdHead);

	  // 报头校验
    if (pBuf[0] != pBuf[3])/// EB = EB
    {
        return(0);
    }
    
    return (1);
}

static WORD Encrypt_CKS(BYTE pdrv ,BYTE *pBuf)
{
    WORD sum = 0;
    BYTE *p = 0;
    BYTE i = 0;

    if(*(pBuf) == _ENCRYPT_STARTCODE)
    {
        p = pBuf+4;
        i = ((*(pBuf+1)<<8)|(*(pBuf+2)));

        while (i--)
        {
            sum += (*p++);
        }
    }

    return (WORD)(sum);
}

BYTE Encrypt_LRC(WORD num,BYTE *pBuf)
{
	WORD i;
	BYTE temp;
	
	for(temp=0x00,i=0;i<num;i++)
	{
		temp = (temp^pBuf[i]);
	}
    temp = ~temp;
	return(temp);
}

static BYTE Encrypt_StoreIN(BYTE pdrv,ENCRYPT_PASDU *buf)
{	
	ENCRYPT_PASDU *temp;
	
	temp = Encrypt_App[pdrv].StoreDATA1IN;///1级数据入指针
	if(++temp >= Encrypt_App[pdrv].StoreDATA1Buf + _ENCRYPT_STOREDATA1NUM)
	{
		temp = Encrypt_App[pdrv].StoreDATA1Buf;
	}
	
	if(temp == Encrypt_App[pdrv].StoreDATA1OUT)
	{
		return(FALSE);
	}
	
	memcpy(Encrypt_App[pdrv].StoreDATA1IN,buf,sizeof(ENCRYPT_PASDU));///把数据发给 一级数据指针	为了发送1级数据

	if(++Encrypt_App[pdrv].StoreDATA1IN >= Encrypt_App[pdrv].StoreDATA1Buf + _ENCRYPT_STOREDATA1NUM)
	{
		Encrypt_App[pdrv].StoreDATA1IN = Encrypt_App[pdrv].StoreDATA1Buf;
	}
		
	return(TRUE);
}

static void Encrypt_StatusReset(BYTE pdrv)
{///待发送缓冲区1级入指针 = 1级数据缓冲区
    Encrypt_App[pdrv].StoreDATA1IN = Encrypt_App[pdrv].StoreDATA1Buf;
    Encrypt_App[pdrv].StoreDATA1OUT = Encrypt_App[pdrv].StoreDATA1Buf;///1级出指针
    memset(Encrypt_App[pdrv].StoreDATA1Buf,0,sizeof(Encrypt_App[pdrv].StoreDATA1Buf));//数组清0
}

static BYTE Encrypt_WriteDataToMISI(BYTE pdrv)
{
    WORD sendlen = 0;
   
    sendlen = Encrypt_WriteData(Encrypt_App[pdrv].TxdBuf + Encrypt_App[pdrv].TxdHead , Encrypt_App[pdrv].TxdTail - Encrypt_App[pdrv].TxdHead , pdrv );
    
    if(sendlen < _ENCRYPT_LPDUSIZE*2)
    {
        Encrypt_App[pdrv].TxdHead += sendlen;
        if (Encrypt_App[pdrv].TxdHead >= Encrypt_App[pdrv].TxdTail) // 该次任务数据已经发完
        {
            Encrypt_App[pdrv].TxdHead = 0;
            Encrypt_App[pdrv].TxdTail = 0;
            return (TRUE);
        }
        else
        {
            return (FALSE);
        }
    }
    else
    {
        return (FALSE);
    }
}

static void Encrypt_ReturnFaultInfo(BYTE apptype,BYTE faulttype_h,BYTE faulttype_l,BYTE pdrv)
{
    WORD num=0;

    Encrypt_App[pdrv].StoreDATA1Temp.Head.MesType_H = 0x00;
    Encrypt_App[pdrv].StoreDATA1Temp.Head.MesType_L = ((Encrypt_App[pdrv].Object&_ENCRYPT_OBJ)|(0x00&_ENCRYPT_ENC));
    Encrypt_App[pdrv].StoreDATA1Temp.Data.Type = apptype;//应用类型
    num=0;
    Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num++] = 0x00;//信息安全扩展区len
    Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num++] = 0x02;//信息安全扩展区len
    Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num++] = faulttype_h;
    Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num++] = faulttype_l;
    Encrypt_App[pdrv].StoreDATA1Temp.status.Length_H = ((num+2+1)>>8)&0xff;
    Encrypt_App[pdrv].StoreDATA1Temp.status.Length_L = (num+2+1)&0xff;
    Encrypt_StoreIN(pdrv,&Encrypt_App[pdrv].StoreDATA1Temp);
}

static void Encrypt_TakeRandomNum(BYTE *txbuff,BYTE *rxbuff)
{
    WORD num=0;
    
    txbuff[num++] = 0x55;
    txbuff[num++] = 0x00;
    txbuff[num++] = 0x84;
    txbuff[num++] = 0x00;
    txbuff[num++] = 0x08;
    txbuff[num++] = 0x00;
    txbuff[num++] = 0x00;
    txbuff[num] = Encrypt_LRC(num-1,&txbuff[1]);
    num++;

    Encrypt_ChipCOM(txbuff,rxbuff);//取随机数
}

static void Encrypt_VerifySign(BYTE *txbuff, BYTE *rxbuff ,BYTE pdrv)
{
    WORD num=0;
    WORD length=0;
    
    length = ((Encrypt_App[pdrv].RxTemp.status.Length_H<<8)|Encrypt_App[pdrv].RxTemp.status.Length_L);
    txbuff[num++] = 0x55;
    txbuff[num++] = 0x80;
    txbuff[num++] = 0x08;
    txbuff[num++] = 0x00;
    txbuff[num++] = Encrypt_App[pdrv].RxTemp.Data.buff[(length-1)-2-1];//长度-1数组最后一位，-2报文类型，-1应用类型
    txbuff[num++] = ((length-2-1)>>8)&0xff;//-2报文类型，-1ASKID
    txbuff[num++] = ((length-2-1))&0xff;
    memcpy(&txbuff[num],&Encrypt_App[pdrv].RxTemp.Data.Type,(length-2-1));
    num += length;
    txbuff[num] = Encrypt_LRC(num-1,&txbuff[1]);
    num++;

    Encrypt_ChipCOM(txbuff,rxbuff);
}

static BYTE Encrypt_Ciphertext(BYTE pdrv)
{	
    WORD num=0;
    WORD length=0;
    BYTE res=FALSE;

    memset(Encrypt_App[pdrv].Enc_txbuff,0,sizeof(Encrypt_App[pdrv].Enc_txbuff));
    if((Encrypt_App[pdrv].Object&_ENCRYPT_OBJ) == _ENCRYPT_OBJ_B)//链接对象是主站
    {
        num=0;
        length = ((Encrypt_App[pdrv].RxdBuf[1]<<8)|(Encrypt_App[pdrv].RxdBuf[2]));
        Encrypt_App[pdrv].Enc_txbuff[num++] = 0x55;///芯片SPI初始接收0x55
        Encrypt_App[pdrv].Enc_txbuff[num++] = 0x80;
        Encrypt_App[pdrv].Enc_txbuff[num++] = 0x2c;
        Encrypt_App[pdrv].Enc_txbuff[num++] = 0x60;
        Encrypt_App[pdrv].Enc_txbuff[num++] = 0x01;
        Encrypt_App[pdrv].Enc_txbuff[num++] = ((length + 16 - 2)>>8)&0xff;///发送数据长度  （数据长度+IVdata:主站随机数）
        Encrypt_App[pdrv].Enc_txbuff[num++] = (length + 16 - 2)&0xff;
        memcpy(&Encrypt_App[pdrv].Enc_txbuff[num],Encrypt_App[pdrv].IVData,16);
        num += 16;
        memcpy(&Encrypt_App[pdrv].Enc_txbuff[num],&Encrypt_App[pdrv].RxdBuf[6],(length - 2));///Encrypt_App[pdrv].RxdBuf[6]封装数据域开始位
        num += (length - 1);
        Encrypt_App[pdrv].Enc_txbuff[num] = Encrypt_LRC(num-1,&Encrypt_App[pdrv].Enc_txbuff[1]);///MAC校验
        num++;
    }
    else if((Encrypt_App[pdrv].Object&_ENCRYPT_OBJ) == _ENCRYPT_OBJ_M)//链接对象是证书管理工具
    {
        num=0;
        length = ((Encrypt_App[pdrv].RxdBuf[1]<<8)|(Encrypt_App[pdrv].RxdBuf[2]));
        Encrypt_App[pdrv].Enc_txbuff[num++] = 0x55;
        Encrypt_App[pdrv].Enc_txbuff[num++] = 0x80;
        Encrypt_App[pdrv].Enc_txbuff[num++] = 0x2c;
        Encrypt_App[pdrv].Enc_txbuff[num++] = 0x62;
        Encrypt_App[pdrv].Enc_txbuff[num++] = 0x02;
        Encrypt_App[pdrv].Enc_txbuff[num++] = ((length + 8 + 8 + 16 - 2)>>8)&0xff;
        Encrypt_App[pdrv].Enc_txbuff[num++] = (length + 8 + 8 + 16 - 2)&0xff;
        memcpy(&Encrypt_App[pdrv].Enc_txbuff[num],Encrypt_App[pdrv].M_ID,8);
        num += 8;
        memcpy(&Encrypt_App[pdrv].Enc_txbuff[num],Encrypt_App[pdrv].IVData,8);
        num += 8;
        memcpy(&Encrypt_App[pdrv].Enc_txbuff[num],Encrypt_App[pdrv].IVData,16);
        num += 16;
        memcpy(&Encrypt_App[pdrv].Enc_txbuff[num],&Encrypt_App[pdrv].RxdBuf[6],(length - 2));
        num += (length - 2);
        Encrypt_App[pdrv].Enc_txbuff[num] = Encrypt_LRC(num-1,&Encrypt_App[pdrv].Enc_txbuff[1]);
        num++;
    }
    Encrypt_ChipCOM(Encrypt_App[pdrv].Enc_txbuff,Encrypt_App[pdrv].Enc_rxbuff);///向加密芯片写数据 并读数据
    if((Encrypt_App[pdrv].Enc_rxbuff[0]==0x90)&&(Encrypt_App[pdrv].Enc_rxbuff[1]==0x00))
    {
        Encrypt_App[pdrv].RxdBuf[1] = ((((Encrypt_App[pdrv].Enc_rxbuff[2]<<8)|Encrypt_App[pdrv].Enc_rxbuff[3])+2)>>8)&0xff;///返回数据长度
        Encrypt_App[pdrv].RxdBuf[2] = (((Encrypt_App[pdrv].Enc_rxbuff[2]<<8)|Encrypt_App[pdrv].Enc_rxbuff[3])+2)&0xff;
        memcpy(&Encrypt_App[pdrv].RxdBuf[6],&Encrypt_App[pdrv].Enc_rxbuff[4],(((Encrypt_App[pdrv].Enc_rxbuff[2]<<8)|Encrypt_App[pdrv].Enc_rxbuff[3])+2));///数据

        length = ((Encrypt_App[pdrv].Enc_rxbuff[2]<<8)|Encrypt_App[pdrv].Enc_rxbuff[3]);///长度

        Encrypt_App[pdrv].RxTemp.status.Length_H = ((length+2)>>8)&0xff;
        Encrypt_App[pdrv].RxTemp.status.Length_L = (length+2)&0xff;
        memcpy(&Encrypt_App[pdrv].RxTemp.Data,&Encrypt_App[pdrv].Enc_rxbuff[4],length);
        res = TRUE;
    }
    else
    {
        Encrypt_ReturnFaultInfo(0x1f,Encrypt_App[pdrv].Enc_rxbuff[0],Encrypt_App[pdrv].Enc_rxbuff[1],pdrv);
        res = FALSE;
    }
    return(res);
}

static BYTE Encrypt_encrypttext(BYTE pdrv)
{	
    WORD num=0;
    WORD length=0;
    BYTE res=FALSE;

    memset(Encrypt_App[pdrv].Enc_txbuff,0,sizeof(Encrypt_App[pdrv].Enc_txbuff));
    if((Encrypt_App[pdrv].Object&_ENCRYPT_OBJ) == _ENCRYPT_OBJ_B)//链接对象是主站
    {
        if(Encrypt_App[pdrv].TxTemp.Data.Type == 0x00)
        {
            num=0;
            length = ((Encrypt_App[pdrv].TxTemp.status.Length_H<<8)|Encrypt_App[pdrv].TxTemp.status.Length_L)-2;
            Encrypt_App[pdrv].Enc_txbuff[num++] = 0x55;
            Encrypt_App[pdrv].Enc_txbuff[num++] = 0x80;
            Encrypt_App[pdrv].Enc_txbuff[num++] = 0x26;
            Encrypt_App[pdrv].Enc_txbuff[num++] = 0x62;
            Encrypt_App[pdrv].Enc_txbuff[num++] = 0x01;
            Encrypt_App[pdrv].Enc_txbuff[num++] = ((length + 16)>>8)&0xff;
            Encrypt_App[pdrv].Enc_txbuff[num++] = (length + 16)&0xff;
            memcpy(&Encrypt_App[pdrv].Enc_txbuff[num],Encrypt_App[pdrv].IVData,16);
            num += 16;
            memcpy(&Encrypt_App[pdrv].Enc_txbuff[num],&Encrypt_App[pdrv].TxTemp.Data,length);
            num += length;
            Encrypt_App[pdrv].Enc_txbuff[num] = Encrypt_LRC(num-1,&Encrypt_App[pdrv].Enc_txbuff[1]);
            num++;
            Encrypt_ChipCOM(Encrypt_App[pdrv].Enc_txbuff,Encrypt_App[pdrv].Enc_rxbuff);
        }
        else
        {
            Encrypt_TakeRandomNum(Encrypt_App[pdrv].Enc_txbuff,Encrypt_App[pdrv].Enc_rxbuff);
            
            if((Encrypt_App[pdrv].Enc_rxbuff[0]==0x90)&&(Encrypt_App[pdrv].Enc_rxbuff[1]==0x00))
            {
                num=0;
                length = ((Encrypt_App[pdrv].TxTemp.status.Length_H<<8)|Encrypt_App[pdrv].TxTemp.status.Length_L)-2;
                Encrypt_App[pdrv].Enc_txbuff[num++] = 0x55;
                Encrypt_App[pdrv].Enc_txbuff[num++] = 0x80;
                Encrypt_App[pdrv].Enc_txbuff[num++] = 0x26;
                Encrypt_App[pdrv].Enc_txbuff[num++] = 0x60;////0x62  change by chen
                Encrypt_App[pdrv].Enc_txbuff[num++] = 0x01;
                Encrypt_App[pdrv].Enc_txbuff[num++] = ((length + 16 + 8)>>8)&0xff;
                Encrypt_App[pdrv].Enc_txbuff[num++] = (length + 16 + 8)&0xff;
                memcpy(&Encrypt_App[pdrv].Enc_txbuff[num],Encrypt_App[pdrv].IVData,16);
                num += 16;
                memcpy(&Encrypt_App[pdrv].Enc_txbuff[num],&Encrypt_App[pdrv].TxTemp.Data,length);
                num += length;
                memcpy(&Encrypt_App[pdrv].Enc_txbuff[num],&Encrypt_App[pdrv].Enc_rxbuff[4],8);
                num += 8;
                Encrypt_App[pdrv].Enc_txbuff[num] = Encrypt_LRC(num-1,&Encrypt_App[pdrv].Enc_txbuff[1]);
                num++;
                Encrypt_ChipCOM(Encrypt_App[pdrv].Enc_txbuff,Encrypt_App[pdrv].Enc_rxbuff);
            }
            else
            {
                Encrypt_ReturnFaultInfo(0x1f,Encrypt_App[pdrv].Enc_rxbuff[0],Encrypt_App[pdrv].Enc_rxbuff[1],pdrv);
                Encrypt_App[pdrv].LinkFlag |= _ENCRYPT_SANDABLE;
                res = FALSE;
            }
        }
    }
    else if((Encrypt_App[pdrv].Object&_ENCRYPT_OBJ) == _ENCRYPT_OBJ_M)//链接对象是证书管理工具
    {
        num=0;
        length = ((Encrypt_App[pdrv].TxTemp.status.Length_H<<8)|Encrypt_App[pdrv].TxTemp.status.Length_L)-2;
        Encrypt_App[pdrv].Enc_txbuff[num++] = 0x55;
        Encrypt_App[pdrv].Enc_txbuff[num++] = 0x80;
        Encrypt_App[pdrv].Enc_txbuff[num++] = 0x26;
        Encrypt_App[pdrv].Enc_txbuff[num++] = 0x62;
        Encrypt_App[pdrv].Enc_txbuff[num++] = 0x02;
        Encrypt_App[pdrv].Enc_txbuff[num++] = ((length + 8 + 8 + 16)>>8)&0xff;
        Encrypt_App[pdrv].Enc_txbuff[num++] = (length + 8 + 8 + 16)&0xff;
        memcpy(&Encrypt_App[pdrv].Enc_txbuff[num],Encrypt_App[pdrv].M_ID,8);
        num += 8;
        memcpy(&Encrypt_App[pdrv].Enc_txbuff[num],Encrypt_App[pdrv].IVData,8);
        num += 8;
        memcpy(&Encrypt_App[pdrv].Enc_txbuff[num],Encrypt_App[pdrv].IVData,16);
        num += 16;
        memcpy(&Encrypt_App[pdrv].Enc_txbuff[num],&Encrypt_App[pdrv].TxTemp.Data,length);
        num += length;
        Encrypt_App[pdrv].Enc_txbuff[num] = Encrypt_LRC(num-1,&Encrypt_App[pdrv].Enc_txbuff[1]);
        num++;
        Encrypt_ChipCOM(Encrypt_App[pdrv].Enc_txbuff,Encrypt_App[pdrv].Enc_rxbuff);
    }
    if((Encrypt_App[pdrv].Enc_rxbuff[0]==0x90)&&(Encrypt_App[pdrv].Enc_rxbuff[1]==0x00))
    {
        length = ((Encrypt_App[pdrv].Enc_rxbuff[2]<<8)|Encrypt_App[pdrv].Enc_rxbuff[3]);
        Encrypt_App[pdrv].TxTemp.status.Length_H = ((length+2)>>8)&0xff;
        Encrypt_App[pdrv].TxTemp.status.Length_L = ((length+2))&0xff;
        memcpy(&Encrypt_App[pdrv].TxTemp.Data,&Encrypt_App[pdrv].Enc_rxbuff[4],length);
        res = TRUE;
    }
    else
    {
        Encrypt_ReturnFaultInfo(0x1f,Encrypt_App[pdrv].Enc_rxbuff[0],Encrypt_App[pdrv].Enc_rxbuff[1],pdrv);
        Encrypt_App[pdrv].LinkFlag |= _ENCRYPT_SANDABLE;
        res = FALSE;
    }
    return(res);
}

static void Encrypt_PackSend(BYTE pdrv)
{
    BYTE *pBuf;
    WORD length;

    pBuf = Encrypt_App[pdrv].TxdBuf + Encrypt_App[pdrv].TxdTail;
    
    if(Encrypt_App[pdrv].TxTemp.Head.MesType_L&_ENCRYPT_ENC)///判断加密标识
    {
        if(!Encrypt_encrypttext(pdrv))///判断加密 发送给加密芯片，如取出值有误，return
        {return;}
    }

    length = ((Encrypt_App[pdrv].TxTemp.status.Length_H<<8)|Encrypt_App[pdrv].TxTemp.status.Length_L);

    pBuf[0] = pBuf[3] = _ENCRYPT_STARTCODE;
    pBuf[1] = (length>>8)&0xff;
    pBuf[2] = length&0xff;

    memcpy(&pBuf[4],&Encrypt_App[pdrv].TxTemp.Head,length);

    length += 6; // 全部报文长度	
    pBuf[length-2] = Encrypt_CKS(pdrv, pBuf)&0xFF;
    pBuf[length-1] = _ENCRYPT_ENDCODE;

    Encrypt_App[pdrv].TxdTail += length;

    Encrypt_WriteDataToMISI(pdrv);
}

static BYTE Encrypt_EncryptCheck(BYTE pdrv)
{
    BYTE res=TRUE;
    WORD length;
	
	length = (Encrypt_App[pdrv].RxdBuf[1]<<8)|Encrypt_App[pdrv].RxdBuf[2];///数据长度
	
	Encrypt_App[pdrv].RxTemp.status.Length_H = (length>>8)&0xff;
	Encrypt_App[pdrv].RxTemp.status.Length_L = length&0xff;
	memcpy(&Encrypt_App[pdrv].RxTemp.Head,&Encrypt_App[pdrv].RxdBuf[4],length);///从报文类型开始
	///把从报文类型开始数据 送到Encrypt_App[pdrv].RxTemp.Head（接收数据 头）
    switch(Encrypt_App[pdrv].RxTemp.Data.Type)///判断报文类型
    {
        case _ENCRYPT_I_FUN00:///0x00 明文下 只有0x00
            if(((Encrypt_App[pdrv].RxdBuf[1]<<8)|Encrypt_App[pdrv].RxdBuf[2])>20)///数据长度不能超过10 /////change by chen
            {res = FALSE;}
            break;
        case _ENCRYPT_I_FUN01:case _ENCRYPT_I_FUN02:case _ENCRYPT_I_FUN03:case _ENCRYPT_I_FUN04:
        case _ENCRYPT_I_FUN05:case _ENCRYPT_I_FUN06:case _ENCRYPT_I_FUN07:case _ENCRYPT_I_FUN08:
            res = FALSE;
            break;
        //添加其他类型
    }
    
    if(res == FALSE)///如果数据长度超过10 或者报文类型不为0x00
    {
        Encrypt_ReturnFaultInfo(0x1f,0x91,0x06,pdrv);///返回错误报文信息
    }
    return(res);
}

//长度/长度/证书标识/总帧数/当前帧序号/证书管理工具N/证书工具ID
static void Encrypt_AppProcess_M_FUN30(BYTE pdrv)
{	
    WORD num=0;
    WORD length=0;
    WORD length_n=0;
    WORD num_n;
    BYTE num_max = 0;
    BYTE i;
	
    if(Encrypt_App[pdrv].RxTemp.Data.buff[4] == 1)//第一包清缓冲区
    {
        memset(Encrypt_App[pdrv].CerContent,0,sizeof(Encrypt_App[pdrv].CerContent));
        Encrypt_StatusReset(pdrv);
    }

    num_n = 0;
    while(Encrypt_App[pdrv].CerContent[num_n+0] != 0)//查找未存储位置
    {
        if(Encrypt_App[pdrv].RxTemp.Data.buff[4] == Encrypt_App[pdrv].CerContent[num_n])//如果分帧包已存在，则跳出
        {break;}
        length_n = ((Encrypt_App[pdrv].CerContent[num_n+1]<<8)|Encrypt_App[pdrv].CerContent[num_n+2]);
        num_n = num_n+length_n+3;
        num_max++;
    }
		
    length_n = (((Encrypt_App[pdrv].RxTemp.Data.buff[0]<<8)|Encrypt_App[pdrv].RxTemp.Data.buff[1])-11);//存储
    if((num_n +3 + length_n) < sizeof(Encrypt_App[pdrv].CerContent))
    {
        Encrypt_App[pdrv].CerContent[num_n+0] = Encrypt_App[pdrv].RxTemp.Data.buff[4];
        Encrypt_App[pdrv].CerContent[num_n+1] = (length_n>>8)&0xff;
        Encrypt_App[pdrv].CerContent[num_n+2] = length_n&0xff;
        memcpy(&Encrypt_App[pdrv].CerContent[num_n+3],&Encrypt_App[pdrv].RxTemp.Data.buff[5],length_n);
        memcpy(Encrypt_App[pdrv].M_ID,&Encrypt_App[pdrv].RxTemp.Data.buff[5+length_n],8);//证书ID赋值
    }
		
    if((num_max+1) == Encrypt_App[pdrv].RxTemp.Data.buff[3])//已存储数量满足要求
    {			
        memset(Encrypt_App[pdrv].Enc_txbuff,0,sizeof(Encrypt_App[pdrv].Enc_txbuff));
        num=0;
        Encrypt_App[pdrv].Enc_txbuff[num++] = 0x55;
        Encrypt_App[pdrv].Enc_txbuff[num++] = 0x80;
        Encrypt_App[pdrv].Enc_txbuff[num++] = 0x1e;
        Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
        Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
        for(i=0,length = 0;i<Encrypt_App[pdrv].RxTemp.Data.buff[3];i++)//组包
        {
            num_n = 0;length_n = 0;
            while(Encrypt_App[pdrv].CerContent[num_n+0] != 0)//查找位置
            {
                length_n = ((Encrypt_App[pdrv].CerContent[num_n+1]<<8)|Encrypt_App[pdrv].CerContent[num_n+2]);
                if(Encrypt_App[pdrv].CerContent[num_n+0] == (i+1))
                {
                    memcpy(&Encrypt_App[pdrv].Enc_txbuff[num+2+length],&Encrypt_App[pdrv].CerContent[num_n+3],length_n);
                    break;
                }
                num_n = num_n+length_n+3;
                if(num_n > sizeof(Encrypt_App[pdrv].CerContent))
                {break;}
            }
            length += length_n;
        }
        Encrypt_App[pdrv].Enc_txbuff[num++] = (length>>8)&0xff;
        Encrypt_App[pdrv].Enc_txbuff[num++] = (length)&0xff;
        num += length;
        Encrypt_App[pdrv].Enc_txbuff[num] = Encrypt_LRC(num-1,&Encrypt_App[pdrv].Enc_txbuff[1]);
        
        Encrypt_ChipCOM(Encrypt_App[pdrv].Enc_txbuff,Encrypt_App[pdrv].Enc_rxbuff);//发送身份验证证书
        
        if((Encrypt_App[pdrv].Enc_rxbuff[0]==0x90)&&(Encrypt_App[pdrv].Enc_rxbuff[1]==0x00))
        {       
            Encrypt_TakeRandomNum(Encrypt_App[pdrv].Enc_txbuff,Encrypt_App[pdrv].Enc_rxbuff);//取随机数
        
            if((Encrypt_App[pdrv].Enc_rxbuff[0]==0x90)&&(Encrypt_App[pdrv].Enc_rxbuff[1]==0x00))
            {
                Encrypt_App[pdrv].StoreDATA1Temp.Head.MesType_H = 0x00;
                Encrypt_App[pdrv].StoreDATA1Temp.Head.MesType_L = ((Encrypt_App[pdrv].Object&_ENCRYPT_OBJ)|(0x00&_ENCRYPT_ENC));
                Encrypt_App[pdrv].StoreDATA1Temp.Data.Type = 0x31;//应用类型
                num=0;
                Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num++] = 0x00;//信息安全扩展区len
                Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num++] = 0x08;//信息安全扩展区len
                memcpy(Encrypt_App[pdrv].IVData,&Encrypt_App[pdrv].Enc_rxbuff[4],8);
                for(i=0;i<8;i++)
                {
                    Encrypt_App[pdrv].IVData[i+8] = ~Encrypt_App[pdrv].IVData[i];
                }
                memcpy(&Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num],&Encrypt_App[pdrv].Enc_rxbuff[4],8);
                num += 8;
                Encrypt_App[pdrv].StoreDATA1Temp.status.Length_H = ((num+2+1)>>8)&0xff;
                Encrypt_App[pdrv].StoreDATA1Temp.status.Length_L = (num+2+1)&0xff;
                Encrypt_StoreIN(pdrv,&Encrypt_App[pdrv].StoreDATA1Temp);
            }
            else
            {
                Encrypt_ReturnFaultInfo(0x33,Encrypt_App[pdrv].Enc_rxbuff[0],Encrypt_App[pdrv].Enc_rxbuff[1],pdrv);
            }
        }
        else
        {
            Encrypt_ReturnFaultInfo(0x33,Encrypt_App[pdrv].Enc_rxbuff[0],Encrypt_App[pdrv].Enc_rxbuff[1],pdrv);
        }
    }
    else
    {
        Encrypt_ReturnFaultInfo(0x45,0x90,0x00,pdrv);
    }
}

//长度/长度/签名
static void Encrypt_AppProcess_M_FUN32(BYTE pdrv)
{	
    WORD num=0;

    num=0;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x55;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x80;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x20;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x40;
    memcpy(&Encrypt_App[pdrv].Enc_txbuff[num],&Encrypt_App[pdrv].RxTemp.Data.buff[2],0x40);
    num += 0x40;
    Encrypt_App[pdrv].Enc_txbuff[num] = Encrypt_LRC(num-1,&Encrypt_App[pdrv].Enc_txbuff[1]);
    num++;

    Encrypt_ChipCOM(Encrypt_App[pdrv].Enc_txbuff,Encrypt_App[pdrv].Enc_rxbuff);

    if((Encrypt_App[pdrv].Enc_rxbuff[0]==0x90)&&(Encrypt_App[pdrv].Enc_rxbuff[1]==0x00))
    {
        Encrypt_App[pdrv].LinkFlag |= _ENCRYPT_M_CERSUCCESS;
    }
    Encrypt_ReturnFaultInfo(0x33,Encrypt_App[pdrv].Enc_rxbuff[0],Encrypt_App[pdrv].Enc_rxbuff[1],pdrv);
}

//长度/长度
static void Encrypt_AppProcess_M_FUN34(BYTE pdrv)
{
    BYTE keyversion;	
    WORD num=0;

    num=0;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x55;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x80;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x1A;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
    Encrypt_App[pdrv].Enc_txbuff[num] = Encrypt_LRC(num-1,&Encrypt_App[pdrv].Enc_txbuff[1]);
    num++;

    Encrypt_ChipCOM(Encrypt_App[pdrv].Enc_txbuff,Encrypt_App[pdrv].Enc_rxbuff);

    if((Encrypt_App[pdrv].Enc_rxbuff[0]==0x90)&&(Encrypt_App[pdrv].Enc_rxbuff[1]==0x00))
    {
        keyversion = Encrypt_App[pdrv].Enc_txbuff[4];
    
        Encrypt_TakeRandomNum(Encrypt_App[pdrv].Enc_txbuff,Encrypt_App[pdrv].Enc_rxbuff);
    
        if((Encrypt_App[pdrv].Enc_rxbuff[0]==0x90)&&(Encrypt_App[pdrv].Enc_rxbuff[1]==0x00))
        {
            Encrypt_App[pdrv].StoreDATA1Temp.Head.MesType_H = 0x00;
            Encrypt_App[pdrv].StoreDATA1Temp.Head.MesType_L = ((Encrypt_App[pdrv].Object&_ENCRYPT_OBJ)|(0x00&_ENCRYPT_ENC));
            Encrypt_App[pdrv].StoreDATA1Temp.Data.Type = 0x35;//应用类型
            num=0;
            Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num++] = 0x00;//信息安全扩展区len
            Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num++] = 0x09;//信息安全扩展区len
            Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num++] = keyversion;
            memcpy(&Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num],&Encrypt_App[pdrv].Enc_rxbuff[4],0x08);
            num += 0x08;
            Encrypt_App[pdrv].StoreDATA1Temp.status.Length_H = ((num+2+1)>>8)&0xff;
            Encrypt_App[pdrv].StoreDATA1Temp.status.Length_L = (num+2+1)&0xff;
            Encrypt_StoreIN(pdrv,&Encrypt_App[pdrv].StoreDATA1Temp);
        }
        else
        {
            Encrypt_ReturnFaultInfo(0x1F,Encrypt_App[pdrv].Enc_rxbuff[0],Encrypt_App[pdrv].Enc_rxbuff[1],pdrv);
        }
    }
    else
    {
        Encrypt_ReturnFaultInfo(0x1F,Encrypt_App[pdrv].Enc_rxbuff[0],Encrypt_App[pdrv].Enc_rxbuff[1],pdrv);
    }		
}

//长度/长度
static void Encrypt_AppProcess_M_FUN36(BYTE pdrv)
{
    WORD num=0;
    BYTE id[24] ="F20xxxxxx201708250906";

    Encrypt_App[pdrv].StoreDATA1Temp.Head.MesType_H = 0x00;
    Encrypt_App[pdrv].StoreDATA1Temp.Head.MesType_L = ((Encrypt_App[pdrv].Object&_ENCRYPT_OBJ)|(0x00&_ENCRYPT_ENC));
    Encrypt_App[pdrv].StoreDATA1Temp.Data.Type = 0x37;//应用类型
    num=0;
    Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num++] = 0x00;//信息安全扩展区len
    Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num++] = 0x18;//信息安全扩展区len
    memcpy(&Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num],id,0x18);
    num += 0x18;
    Encrypt_App[pdrv].StoreDATA1Temp.status.Length_H = ((num+2+1)>>8)&0xff;
    Encrypt_App[pdrv].StoreDATA1Temp.status.Length_L = (num+2+1)&0xff;
    Encrypt_StoreIN(pdrv,&Encrypt_App[pdrv].StoreDATA1Temp);	
}

//长度/长度
static void Encrypt_AppProcess_M_FUN38(BYTE pdrv)
{
    WORD num=0;

    num=0;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x55;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0xb0;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x99;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x05;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x02;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x08;
    Encrypt_App[pdrv].Enc_txbuff[num] = Encrypt_LRC(num-1,&Encrypt_App[pdrv].Enc_txbuff[1]);
    num++;

    Encrypt_ChipCOM(Encrypt_App[pdrv].Enc_txbuff,Encrypt_App[pdrv].Enc_rxbuff);

    if((Encrypt_App[pdrv].Enc_rxbuff[0]==0x90)&&(Encrypt_App[pdrv].Enc_rxbuff[1]==0x00))
    {
        Encrypt_App[pdrv].StoreDATA1Temp.Head.MesType_H = 0x00;
        Encrypt_App[pdrv].StoreDATA1Temp.Head.MesType_L = ((Encrypt_App[pdrv].Object&_ENCRYPT_OBJ)|(0x00&_ENCRYPT_ENC));
        Encrypt_App[pdrv].StoreDATA1Temp.Data.Type = 0x39;//应用类型
        num=0;
        Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num++] = 0x00;//信息安全扩展区len
        Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num++] = 0x08;//信息安全扩展区len
        memcpy(&Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num],&Encrypt_App[pdrv].Enc_rxbuff[4],0x08);
        num += 0x08;
        Encrypt_App[pdrv].StoreDATA1Temp.status.Length_H = ((num+2+1)>>8)&0xff;
        Encrypt_App[pdrv].StoreDATA1Temp.status.Length_L = (num+2+1)&0xff;
        Encrypt_StoreIN(pdrv,&Encrypt_App[pdrv].StoreDATA1Temp);
    }
    else
    {
        Encrypt_ReturnFaultInfo(0x1F,Encrypt_App[pdrv].Enc_rxbuff[0],Encrypt_App[pdrv].Enc_rxbuff[1],pdrv);
    }	
}

//长度/长度
static void Encrypt_AppProcess_M_FUN3A(BYTE pdrv)
{	
    WORD num=0;
    WORD length=0;

    num=0;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x55;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x80;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x30;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
    Encrypt_App[pdrv].Enc_txbuff[num] = Encrypt_LRC(num-1,&Encrypt_App[pdrv].Enc_txbuff[1]);
    num++;

    Encrypt_ChipCOM(Encrypt_App[pdrv].Enc_txbuff,Encrypt_App[pdrv].Enc_rxbuff);

    if((Encrypt_App[pdrv].Enc_rxbuff[0]==0x90)&&(Encrypt_App[pdrv].Enc_rxbuff[1]==0x00))
    {
        length = (Encrypt_App[pdrv].Enc_rxbuff[2]<<8)|Encrypt_App[pdrv].Enc_rxbuff[3];
        Encrypt_App[pdrv].StoreDATA1Temp.Head.MesType_H = 0x00;
        Encrypt_App[pdrv].StoreDATA1Temp.Head.MesType_L = ((Encrypt_App[pdrv].Object&_ENCRYPT_OBJ)|(0x08&_ENCRYPT_ENC));
        Encrypt_App[pdrv].StoreDATA1Temp.Data.Type = 0x3B;//应用类型
        num=0;
        Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num++] = (length>>8)&0xff;
        Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num++] = (length)&0xff;
        memcpy(&Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num],&Encrypt_App[pdrv].Enc_rxbuff[4],length);
        num += length;
        Encrypt_App[pdrv].StoreDATA1Temp.status.Length_H = ((num+2+1)>>8)&0xff;
        Encrypt_App[pdrv].StoreDATA1Temp.status.Length_L = ((num+2+1))&0xff;
        Encrypt_StoreIN(pdrv,&Encrypt_App[pdrv].StoreDATA1Temp);
    }
    else
    {
        Encrypt_ReturnFaultInfo(0x1F,Encrypt_App[pdrv].Enc_rxbuff[0],Encrypt_App[pdrv].Enc_rxbuff[1],pdrv);
    }	
}

//长度/长度/总帧数/当前帧序号/证书请求数据
static void Encrypt_AppProcess_M_FUN3C(BYTE pdrv)
{	
    WORD num=0;
    WORD length=0;
    WORD length_n=0;
    WORD num_n;
    BYTE num_max = 0;
    BYTE i;
	
    if(Encrypt_App[pdrv].RxTemp.Data.buff[3] == 1)//第一包清缓冲区
    {
        memset(Encrypt_App[pdrv].CerContent,0,sizeof(Encrypt_App[pdrv].CerContent));
    }

    num_n = 0;
    while(Encrypt_App[pdrv].CerContent[num_n+0] != 0)//查找未存储位置
    {
        if(Encrypt_App[pdrv].RxTemp.Data.buff[3] == Encrypt_App[pdrv].CerContent[num_n])//如果分帧包已存在，则跳出
        {break;}
        length_n = ((Encrypt_App[pdrv].CerContent[num_n+1]<<8)|Encrypt_App[pdrv].CerContent[num_n+2]);
        num_n = num_n+length_n+3;
        num_max++;
    }
    
    length_n = (((Encrypt_App[pdrv].RxTemp.Data.buff[0]<<8)|Encrypt_App[pdrv].RxTemp.Data.buff[1])-2);//存储
    if((num_n +3 + length_n) < sizeof(Encrypt_App[pdrv].CerContent))
    {
        Encrypt_App[pdrv].CerContent[num_n+0] = Encrypt_App[pdrv].RxTemp.Data.buff[3];
        Encrypt_App[pdrv].CerContent[num_n+1] = (length_n>>8)&0xff;
        Encrypt_App[pdrv].CerContent[num_n+2] = length_n&0xff;
        memcpy(&Encrypt_App[pdrv].CerContent[num_n+3],&Encrypt_App[pdrv].RxTemp.Data.buff[4],length_n);
    }
    
    if((num_max+1) == Encrypt_App[pdrv].RxTemp.Data.buff[2])//已存储数量满足要求
    {			
        memset(Encrypt_App[pdrv].Enc_txbuff,0,sizeof(Encrypt_App[pdrv].Enc_txbuff));
        num=0;
        Encrypt_App[pdrv].Enc_txbuff[num++] = 0x55;
        Encrypt_App[pdrv].Enc_txbuff[num++] = 0x80;
        Encrypt_App[pdrv].Enc_txbuff[num++] = 0x0a;
        Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
        Encrypt_App[pdrv].Enc_txbuff[num++] = 0x80;
        for(i=0,length = 0;i<Encrypt_App[pdrv].RxTemp.Data.buff[2];i++)//组包
        {
            num_n = 0;length_n = 0;
            while(Encrypt_App[pdrv].CerContent[num_n+0] != 0)//查找位置
            {
                    length_n = ((Encrypt_App[pdrv].CerContent[num_n+1]<<8)|Encrypt_App[pdrv].CerContent[num_n+2]);
                    if(Encrypt_App[pdrv].CerContent[num_n+0] == (i+1))
                    {
                        memcpy(&Encrypt_App[pdrv].Enc_txbuff[num+2+length],&Encrypt_App[pdrv].CerContent[num_n+3],length_n);
                        break;
                    }
                    num_n = num_n+length_n+3;
                    if(num_n > sizeof(Encrypt_App[pdrv].CerContent))
                    {break;}
            }
            length += length_n;
        }
        Encrypt_App[pdrv].Enc_txbuff[num++] = (length>>8)&0xff;
        Encrypt_App[pdrv].Enc_txbuff[num++] = (length)&0xff;
        num += length;
        Encrypt_App[pdrv].Enc_txbuff[num] = Encrypt_LRC(num-1,&Encrypt_App[pdrv].Enc_txbuff[1]);
        
        Encrypt_ChipCOM(Encrypt_App[pdrv].Enc_txbuff,Encrypt_App[pdrv].Enc_rxbuff);//发送身份验证证书

        if((Encrypt_App[pdrv].Enc_rxbuff[0]==0x90)&&(Encrypt_App[pdrv].Enc_rxbuff[1]==0x00))
        {
            Encrypt_App[pdrv].StoreDATA1Temp.Head.MesType_H = 0x00;
            Encrypt_App[pdrv].StoreDATA1Temp.Head.MesType_L = ((Encrypt_App[pdrv].Object&_ENCRYPT_OBJ)|(0x00&_ENCRYPT_ENC));
            Encrypt_App[pdrv].StoreDATA1Temp.Data.Type = 0x3D;//应用类型
            num=0;
            Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num++] = 0x00;//信息安全扩展区len
            Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num++] = 0x40;//信息安全扩展区len
            memcpy(&Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num],&Encrypt_App[pdrv].Enc_rxbuff[4],0x40);
            num += 0x40;
            Encrypt_App[pdrv].StoreDATA1Temp.status.Length_H = ((num+2+1)>>8)&0xff;
            Encrypt_App[pdrv].StoreDATA1Temp.status.Length_L = (num+2+1)&0xff;
            Encrypt_StoreIN(pdrv,&Encrypt_App[pdrv].StoreDATA1Temp);
        }
        else
        {
            Encrypt_ReturnFaultInfo(0x33,Encrypt_App[pdrv].Enc_rxbuff[0],Encrypt_App[pdrv].Enc_rxbuff[1],pdrv);
        }
    }
    else
    {
        Encrypt_ReturnFaultInfo(0x45,0x90,0x00,pdrv);
    }
}

//长度/长度/证书标识/总帧数/当前帧序号/证书文件N
static void Encrypt_AppProcess_M_FUN3E(BYTE pdrv)
{	
    WORD num=0;
    WORD length=0;
    WORD length_n=0;
    WORD num_n;
    BYTE num_max = 0;
    BYTE i;
	
    if(Encrypt_App[pdrv].RxTemp.Data.buff[4] == 1)//第一包清缓冲区
    {
        memset(Encrypt_App[pdrv].CerContent,0,sizeof(Encrypt_App[pdrv].CerContent));
    }

    num_n = 0;
    while(Encrypt_App[pdrv].CerContent[num_n+0] != 0)//查找未存储位置
    {
        if(Encrypt_App[pdrv].RxTemp.Data.buff[4] == Encrypt_App[pdrv].CerContent[num_n])//如果分帧包已存在，则跳出
        {break;}
        length_n = ((Encrypt_App[pdrv].CerContent[num_n+1]<<8)|Encrypt_App[pdrv].CerContent[num_n+2]);
        num_n = num_n+length_n+3;
        num_max++;
    }
    
    length_n = (((Encrypt_App[pdrv].RxTemp.Data.buff[0]<<8)|Encrypt_App[pdrv].RxTemp.Data.buff[1])-3);//存储
    if((num_n +3 + length_n) < sizeof(Encrypt_App[pdrv].CerContent))
    {
        Encrypt_App[pdrv].CerContent[num_n+0] = Encrypt_App[pdrv].RxTemp.Data.buff[4];
        Encrypt_App[pdrv].CerContent[num_n+1] = (length_n>>8)&0xff;
        Encrypt_App[pdrv].CerContent[num_n+2] = length_n&0xff;
        memcpy(&Encrypt_App[pdrv].CerContent[num_n+3],&Encrypt_App[pdrv].RxTemp.Data.buff[5],length_n);
    }
    
    if((num_max+1) == Encrypt_App[pdrv].RxTemp.Data.buff[3])//已存储数量满足要求
    {			
        memset(Encrypt_App[pdrv].Enc_txbuff,0,sizeof(Encrypt_App[pdrv].Enc_txbuff));
        num=0;
        Encrypt_App[pdrv].Enc_txbuff[num++] = 0x55;
        Encrypt_App[pdrv].Enc_txbuff[num++] = 0x80;
        if(Encrypt_App[pdrv].RxTemp.Data.buff[2] == 0x06)
        {
            Encrypt_App[pdrv].Enc_txbuff[num++] = 0x24;
            Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
        }
        else
        {
            Encrypt_App[pdrv].Enc_txbuff[num++] = 0x22;
            Encrypt_App[pdrv].Enc_txbuff[num++] = Encrypt_App[pdrv].RxTemp.Data.buff[2];
        }
        Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
        for(i=0,length = 0;i<Encrypt_App[pdrv].RxTemp.Data.buff[3];i++)//组包
        {
            num_n = 0;length_n = 0;
            while(Encrypt_App[pdrv].CerContent[num_n+0] != 0)//查找位置
            {
                length_n = ((Encrypt_App[pdrv].CerContent[num_n+1]<<8)|Encrypt_App[pdrv].CerContent[num_n+2]);
                if(Encrypt_App[pdrv].CerContent[num_n+0] == (i+1))
                {
                    memcpy(&Encrypt_App[pdrv].Enc_txbuff[num+2+length],&Encrypt_App[pdrv].CerContent[num_n+3],length_n);
                    break;
                }
                num_n = num_n+length_n+3;
                if(num_n > sizeof(Encrypt_App[pdrv].CerContent))
                {break;}
            }
            length += length_n;
        }
        Encrypt_App[pdrv].Enc_txbuff[num++] = (length>>8)&0xff;
        Encrypt_App[pdrv].Enc_txbuff[num++] = (length)&0xff;
        num += length;
        Encrypt_App[pdrv].Enc_txbuff[num] = Encrypt_LRC(num-1,&Encrypt_App[pdrv].Enc_txbuff[1]);
        
        Encrypt_ChipCOM(Encrypt_App[pdrv].Enc_txbuff,Encrypt_App[pdrv].Enc_rxbuff);//发送证书
        
        Encrypt_ReturnFaultInfo(0x3f,Encrypt_App[pdrv].Enc_rxbuff[0],Encrypt_App[pdrv].Enc_rxbuff[1],pdrv);
    }
    else
    {
        Encrypt_ReturnFaultInfo(0x45,0x90,0x00,pdrv);
    }
}

//长度/长度
static void Encrypt_AppProcess_M_FUN40(BYTE pdrv)
{
    WORD num=0;
    WORD length;

    num=0;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x55;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0xb0;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x81;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x02;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x02;
    Encrypt_App[pdrv].Enc_txbuff[num] = Encrypt_LRC(num-1,&Encrypt_App[pdrv].Enc_txbuff[1]);
    num++;
    Encrypt_ChipCOM(Encrypt_App[pdrv].Enc_txbuff,Encrypt_App[pdrv].Enc_rxbuff);	
    if((Encrypt_App[pdrv].Enc_rxbuff[0]==0x90)&&(Encrypt_App[pdrv].Enc_rxbuff[1]==0x00))
    {
        num=0;
        Encrypt_App[pdrv].Enc_txbuff[num++] = 0x55;
        Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
        Encrypt_App[pdrv].Enc_txbuff[num++] = 0xb0;
        Encrypt_App[pdrv].Enc_txbuff[num++] = 0x81;
        Encrypt_App[pdrv].Enc_txbuff[num++] = 0x02;
        Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
        Encrypt_App[pdrv].Enc_txbuff[num++] = 0x02;
        Encrypt_App[pdrv].Enc_txbuff[num++] = Encrypt_App[pdrv].Enc_rxbuff[4];
        Encrypt_App[pdrv].Enc_txbuff[num++] = Encrypt_App[pdrv].Enc_rxbuff[5];
        Encrypt_App[pdrv].Enc_txbuff[num] = Encrypt_LRC(num-1,&Encrypt_App[pdrv].Enc_txbuff[1]);
        num++;
        Encrypt_ChipCOM(Encrypt_App[pdrv].Enc_txbuff,Encrypt_App[pdrv].Enc_rxbuff);		
        if((Encrypt_App[pdrv].Enc_rxbuff[0]==0x90)&&(Encrypt_App[pdrv].Enc_rxbuff[1]==0x00))
        {
            num=0;
            Encrypt_App[pdrv].Enc_txbuff[num++] = 0x55;
            Encrypt_App[pdrv].Enc_txbuff[num++] = 0x80;
            Encrypt_App[pdrv].Enc_txbuff[num++] = 0x24;
            Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
            Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
            Encrypt_App[pdrv].Enc_txbuff[num++] = Encrypt_App[pdrv].Enc_rxbuff[2];
            Encrypt_App[pdrv].Enc_txbuff[num++] = Encrypt_App[pdrv].Enc_rxbuff[3];
            length = ((Encrypt_App[pdrv].Enc_rxbuff[2]<<8)|Encrypt_App[pdrv].Enc_rxbuff[3]);
            memcpy(&Encrypt_App[pdrv].Enc_txbuff[num],&Encrypt_App[pdrv].Enc_rxbuff[4],length);
            num += length;
            Encrypt_App[pdrv].Enc_txbuff[num] = Encrypt_LRC(num-1,&Encrypt_App[pdrv].Enc_txbuff[1]);
            num++;
        
            Encrypt_ChipCOM(Encrypt_App[pdrv].Enc_txbuff,Encrypt_App[pdrv].Enc_rxbuff);
            Encrypt_ReturnFaultInfo(0x41,Encrypt_App[pdrv].Enc_rxbuff[0],Encrypt_App[pdrv].Enc_rxbuff[1],pdrv);
        }
        else
        {
            Encrypt_ReturnFaultInfo(0x1F,Encrypt_App[pdrv].Enc_rxbuff[0],Encrypt_App[pdrv].Enc_rxbuff[1],pdrv);
        }	
    }
    else
    {
        Encrypt_ReturnFaultInfo(0x1F,Encrypt_App[pdrv].Enc_rxbuff[0],Encrypt_App[pdrv].Enc_rxbuff[1],pdrv);
    }	
}

//长度/长度/证书标识/总帧数/当前帧序号/终端证书N
static void Encrypt_AppProcess_M_FUN42(BYTE pdrv)
{	
    WORD eachnum=0xf0;
    WORD group=0;
    WORD num=0;
    WORD length=0;
    WORD length_n=0;
    BYTE i;
	
    num=0;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x55;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x80;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x30;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x01;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
    Encrypt_App[pdrv].Enc_txbuff[num] = Encrypt_LRC(num-1,&Encrypt_App[pdrv].Enc_txbuff[1]);
    num++;
    Encrypt_ChipCOM(Encrypt_App[pdrv].Enc_txbuff,Encrypt_App[pdrv].Enc_rxbuff);	
        
    if((Encrypt_App[pdrv].Enc_rxbuff[0]==0x90)&&(Encrypt_App[pdrv].Enc_rxbuff[1]==0x00))
    {		
        length = ((Encrypt_App[pdrv].Enc_rxbuff[2]<<8)|Encrypt_App[pdrv].Enc_rxbuff[3]);
        group = (length%eachnum) ? (length/eachnum + 1)  : (length/eachnum);
        Encrypt_StatusReset(pdrv);
        for(i=0;i<group;i++)
        {
            Encrypt_App[pdrv].StoreDATA1Temp.Head.MesType_H = 0x00;
            Encrypt_App[pdrv].StoreDATA1Temp.Head.MesType_L = ((Encrypt_App[pdrv].Object&_ENCRYPT_OBJ)|(0x00&_ENCRYPT_ENC));
            Encrypt_App[pdrv].StoreDATA1Temp.Data.Type = 0x43;//应用类型
            num=0;
            length_n = ((i+1) == group) ? (length%eachnum)  : eachnum;
            Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num++] = ((length_n+3)>>8)&0xff;//信息安全扩展区len
            Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num++] = (length_n+3)&0xff;//信息安全扩展区len
            Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num++] = 6;
            Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num++] = group;
            Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num++] = i+1;
            memcpy(&Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num],&Encrypt_App[pdrv].Enc_rxbuff[4 + i*eachnum],length_n);
            num += length_n;
            Encrypt_App[pdrv].StoreDATA1Temp.status.Length_H = ((num+2+1)>>8)&0xff;
            Encrypt_App[pdrv].StoreDATA1Temp.status.Length_L = (num+2+1)&0xff;
            Encrypt_StoreIN(pdrv,&Encrypt_App[pdrv].StoreDATA1Temp);
        }
    }
    else
    {
            Encrypt_ReturnFaultInfo(0x1f,Encrypt_App[pdrv].Enc_rxbuff[0],Encrypt_App[pdrv].Enc_rxbuff[1],pdrv);
    }
}

//长度/长度/密钥包
static void Encrypt_AppProcess_M_FUN46(BYTE pdrv)
{	
    WORD num=0;
    WORD length=0;

    num=0;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x55;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x80;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x1c;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x02;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
    length = (Encrypt_App[pdrv].RxTemp.Data.buff[0]<<8)|Encrypt_App[pdrv].RxTemp.Data.buff[1];
    Encrypt_App[pdrv].Enc_txbuff[num++] = (length>>8)&0xff;
    Encrypt_App[pdrv].Enc_txbuff[num++] = (length)&0xff;
    memcpy(&Encrypt_App[pdrv].Enc_txbuff[num],&Encrypt_App[pdrv].RxTemp.Data.buff[2],length);
    num += length;
    Encrypt_App[pdrv].Enc_txbuff[num] = Encrypt_LRC(num-1,&Encrypt_App[pdrv].Enc_txbuff[1]);
    num++;	
    Encrypt_ChipCOM(Encrypt_App[pdrv].Enc_txbuff,Encrypt_App[pdrv].Enc_rxbuff);	
    Encrypt_ReturnFaultInfo(0x47,Encrypt_App[pdrv].Enc_rxbuff[0],Encrypt_App[pdrv].Enc_rxbuff[1],pdrv);
}

//长度/长度/数据
static void Encrypt_AppProcess_M_FUN48(BYTE pdrv)
{	
    WORD num=0;
    WORD length=0;

    Encrypt_App[pdrv].StoreDATA1Temp.Head.MesType_H = 0x00;
    Encrypt_App[pdrv].StoreDATA1Temp.Head.MesType_L = ((Encrypt_App[pdrv].Object&_ENCRYPT_OBJ)|(0x08&_ENCRYPT_ENC));
    Encrypt_App[pdrv].StoreDATA1Temp.Data.Type = 0x49;//应用类型

    num=0;
    length = (Encrypt_App[pdrv].RxTemp.Data.buff[0]<<8)|Encrypt_App[pdrv].RxTemp.Data.buff[1];
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x55;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x80;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x26;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x62;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x02;
    Encrypt_App[pdrv].Enc_txbuff[num++] = ((8+8+16+length)>>8)&0xff;
    Encrypt_App[pdrv].Enc_txbuff[num++] = ((8+8+16+length))&0xff;
    memcpy(&Encrypt_App[pdrv].Enc_txbuff[num],Encrypt_App[pdrv].M_ID,8);
    num += 8;
    memcpy(&Encrypt_App[pdrv].Enc_txbuff[num],Encrypt_App[pdrv].IVData,8);
    num += 8;
    memcpy(&Encrypt_App[pdrv].Enc_txbuff[num],Encrypt_App[pdrv].IVData,16);
    num += 16;
    memcpy(&Encrypt_App[pdrv].Enc_txbuff[num],&Encrypt_App[pdrv].RxTemp.Data.buff[2],length);
    num += length;
    Encrypt_App[pdrv].Enc_txbuff[num] = Encrypt_LRC(num-1,&Encrypt_App[pdrv].Enc_txbuff[1]);
    num++;

    Encrypt_ChipCOM(Encrypt_App[pdrv].Enc_txbuff,Encrypt_App[pdrv].Enc_rxbuff);
    
    if((Encrypt_App[pdrv].Enc_rxbuff[0]==0x90)&&(Encrypt_App[pdrv].Enc_rxbuff[1]==0x00))
    {
        length = ((Encrypt_App[pdrv].Enc_rxbuff[2]<<8)|Encrypt_App[pdrv].Enc_rxbuff[3]);
        memcpy(&Encrypt_App[pdrv].StoreDATA1Temp.Data,&Encrypt_App[pdrv].Enc_rxbuff[4],length);
        Encrypt_App[pdrv].StoreDATA1Temp.status.Length_H = ((length+2)>>8)&0xff;
        Encrypt_App[pdrv].StoreDATA1Temp.status.Length_L = (length+2)&0xff;
        Encrypt_StoreIN(pdrv,&Encrypt_App[pdrv].StoreDATA1Temp);
    }
    else
    {
        Encrypt_ReturnFaultInfo(0x1F,Encrypt_App[pdrv].Enc_rxbuff[0],Encrypt_App[pdrv].Enc_rxbuff[1],pdrv);
    }
}

//长度/内容
static void Encrypt_AppProcess_I_FUN00(BYTE pdrv)
{
    Encrypt_App[pdrv].IECLength = Encrypt_App[pdrv].RxTemp.Data.buff[0];//IEC长度 应用数据区
    memcpy(Encrypt_App[pdrv].IECBuf,&Encrypt_App[pdrv].RxTemp.Data.buff[1],Encrypt_App[pdrv].IECLength);///101数据 ->IEC
}

//长度/内容/长度/长度/签名结果/密钥标识
static void Encrypt_AppProcess_I_FUN01(BYTE pdrv)
{
    Encrypt_VerifySign(Encrypt_App[pdrv].Enc_txbuff,Encrypt_App[pdrv].Enc_rxbuff,pdrv);
    
    if((Encrypt_App[pdrv].Enc_rxbuff[0]==0x90)&&(Encrypt_App[pdrv].Enc_rxbuff[1]==0x00))
    {
        Encrypt_TakeRandomNum(Encrypt_App[pdrv].Enc_txbuff,Encrypt_App[pdrv].Enc_rxbuff);//取随机数
        
        if((Encrypt_App[pdrv].Enc_rxbuff[0]==0x90)&&(Encrypt_App[pdrv].Enc_rxbuff[1]==0x00))
        {
            memcpy(Encrypt_App[pdrv].R_NUM,&Encrypt_App[pdrv].Enc_rxbuff[4],8);
            Encrypt_App[pdrv].IECLength = Encrypt_App[pdrv].RxTemp.Data.buff[0];//IEC长度
            memcpy(Encrypt_App[pdrv].IECBuf,&Encrypt_App[pdrv].RxTemp.Data.buff[1],Encrypt_App[pdrv].IECLength);
        }
        else
        {
            Encrypt_ReturnFaultInfo(0x1F,Encrypt_App[pdrv].Enc_rxbuff[0],Encrypt_App[pdrv].Enc_rxbuff[1],pdrv);
        }
    }
    else
    {
        Encrypt_ReturnFaultInfo(0x1F,Encrypt_App[pdrv].Enc_rxbuff[0],Encrypt_App[pdrv].Enc_rxbuff[1],pdrv);
    }
}

//长度/内容/长度/长度/签名结果/密钥标识
static void Encrypt_AppProcess_I_FUN03(BYTE pdrv)
{	    
    Encrypt_VerifySign(Encrypt_App[pdrv].Enc_txbuff,Encrypt_App[pdrv].Enc_rxbuff,pdrv);
    
    if((Encrypt_App[pdrv].Enc_rxbuff[0]==0x90)&&(Encrypt_App[pdrv].Enc_rxbuff[1]==0x00))
    {
        Encrypt_App[pdrv].IECLength = Encrypt_App[pdrv].RxTemp.Data.buff[0];//IEC长度
        memcpy(Encrypt_App[pdrv].IECBuf,&Encrypt_App[pdrv].RxTemp.Data.buff[1],Encrypt_App[pdrv].IECLength);
    }
    else
    {
        Encrypt_ReturnFaultInfo(0x1F,Encrypt_App[pdrv].Enc_rxbuff[0],Encrypt_App[pdrv].Enc_rxbuff[1],pdrv);
    }
}

//长度/内容/长度/长度/签名结果/密钥标识
static void Encrypt_AppProcess_I_FUN05(BYTE pdrv)
{    
    Encrypt_VerifySign(Encrypt_App[pdrv].Enc_txbuff,Encrypt_App[pdrv].Enc_rxbuff,pdrv);///800800 向加密芯片发送数据
    
    if((Encrypt_App[pdrv].Enc_rxbuff[0]==0x90)&&(Encrypt_App[pdrv].Enc_rxbuff[1]==0x00))
    {
        Encrypt_App[pdrv].IECLength = Encrypt_App[pdrv].RxTemp.Data.buff[0];//IEC长度
        memcpy(Encrypt_App[pdrv].IECBuf,&Encrypt_App[pdrv].RxTemp.Data.buff[1],Encrypt_App[pdrv].IECLength);
    }
    else
    {
        Encrypt_ReturnFaultInfo(0x1F,Encrypt_App[pdrv].Enc_rxbuff[0],Encrypt_App[pdrv].Enc_rxbuff[1],pdrv);
    }
}

//长度/内容/长度/长度/签名结果/密钥标识
static void Encrypt_AppProcess_I_FUN07(BYTE pdrv)
{	    
    Encrypt_VerifySign(Encrypt_App[pdrv].Enc_txbuff,Encrypt_App[pdrv].Enc_rxbuff,pdrv);
    
    if((Encrypt_App[pdrv].Enc_rxbuff[0]==0x90)&&(Encrypt_App[pdrv].Enc_rxbuff[1]==0x00))
    {
        Encrypt_App[pdrv].IECLength = Encrypt_App[pdrv].RxTemp.Data.buff[0];//IEC长度
        memcpy(Encrypt_App[pdrv].IECBuf,&Encrypt_App[pdrv].RxTemp.Data.buff[1],Encrypt_App[pdrv].IECLength);
    }
    else
    {
        Encrypt_ReturnFaultInfo(0x1F,Encrypt_App[pdrv].Enc_rxbuff[0],Encrypt_App[pdrv].Enc_rxbuff[1],pdrv);
    }
}

//长度/长度/随机数
static void Encrypt_AppProcess_G_FUN20(BYTE pdrv)
{
    WORD num=0;

    num=0;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x55;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x80;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x16;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x80;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x08;
    memcpy(&Encrypt_App[pdrv].Enc_txbuff[num],&Encrypt_App[pdrv].RxTemp.Data.buff[2],8);
    num += 8;
    Encrypt_App[pdrv].Enc_txbuff[num] = Encrypt_LRC(num-1,&Encrypt_App[pdrv].Enc_txbuff[1]);
    num++;

    Encrypt_ChipCOM(Encrypt_App[pdrv].Enc_txbuff,Encrypt_App[pdrv].Enc_rxbuff);

    if((Encrypt_App[pdrv].Enc_rxbuff[0]==0x90)&&(Encrypt_App[pdrv].Enc_rxbuff[1]==0x00))
    {
        Encrypt_App[pdrv].StoreDATA1Temp.Head.MesType_H = 0x00;
        Encrypt_App[pdrv].StoreDATA1Temp.Head.MesType_L = ((Encrypt_App[pdrv].Object&_ENCRYPT_OBJ)|(0x00&_ENCRYPT_ENC));
        Encrypt_App[pdrv].StoreDATA1Temp.Data.Type = 0x21;//应用类型
        num=0;
        Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num++] = 0x00;//信息安全扩展区len
        Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num++] = 0x40+0x08+1;//信息安全扩展区len
        memcpy(&Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num],&Encrypt_App[pdrv].Enc_rxbuff[4],0x40+0x08);
        num += (0x40+0x08);
        Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num++] = 0x01;//签名密钥标识
        Encrypt_App[pdrv].StoreDATA1Temp.status.Length_H = ((num+2+1)>>8)&0xff;
        Encrypt_App[pdrv].StoreDATA1Temp.status.Length_L = (num+2+1)&0xff;
        Encrypt_StoreIN(pdrv,&Encrypt_App[pdrv].StoreDATA1Temp);
    }
    else
    {
        Encrypt_ReturnFaultInfo(0x1f,Encrypt_App[pdrv].Enc_rxbuff[0],Encrypt_App[pdrv].Enc_rxbuff[1],pdrv);
    }
}

//长度/长度/签名结果/密钥标识
static void Encrypt_AppProcess_G_FUN22(BYTE pdrv)
{
    WORD num=0;

    num=0;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x55;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x80;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x18;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x05;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x40;
    memcpy(&Encrypt_App[pdrv].Enc_txbuff[num],&Encrypt_App[pdrv].RxTemp.Data.buff[2],0x40);
    num += 0x40;
    Encrypt_App[pdrv].Enc_txbuff[num] = Encrypt_LRC(num-1,&Encrypt_App[pdrv].Enc_txbuff[1]);
    num++;

    Encrypt_ChipCOM(Encrypt_App[pdrv].Enc_txbuff,Encrypt_App[pdrv].Enc_rxbuff);

    if((Encrypt_App[pdrv].Enc_rxbuff[0]==0x90)&&(Encrypt_App[pdrv].Enc_rxbuff[1]==0x00))
    {
        Encrypt_App[pdrv].LinkFlag |= _ENCRYPT_G_CERSUCCESS;
    }

    Encrypt_ReturnFaultInfo(0x23,Encrypt_App[pdrv].Enc_rxbuff[0],Encrypt_App[pdrv].Enc_rxbuff[1],pdrv);
}

//长度/长度/随机数
static void Encrypt_AppProcess_B_FUN50(BYTE pdrv)
{	
    WORD num=0;

    num=0;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x55;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x80;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x16;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x80;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x08;
    memcpy(&Encrypt_App[pdrv].Enc_txbuff[num],&Encrypt_App[pdrv].RxTemp.Data.buff[2],8);
    num += 8;
    Encrypt_App[pdrv].Enc_txbuff[num] = Encrypt_LRC(num-1,&Encrypt_App[pdrv].Enc_txbuff[1]);
    num++;

    Encrypt_ChipCOM(Encrypt_App[pdrv].Enc_txbuff,Encrypt_App[pdrv].Enc_rxbuff);

    if((Encrypt_App[pdrv].Enc_rxbuff[0]==0x90)&&(Encrypt_App[pdrv].Enc_rxbuff[1]==0x00))
    {
        Encrypt_App[pdrv].StoreDATA1Temp.Head.MesType_H = 0x00;
        Encrypt_App[pdrv].StoreDATA1Temp.Head.MesType_L = ((Encrypt_App[pdrv].Object&_ENCRYPT_OBJ)|(0x00&_ENCRYPT_ENC));
        Encrypt_App[pdrv].StoreDATA1Temp.Data.Type = 0x51;//应用类型
        num=0;
        Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num++] = 0x00;//信息安全扩展区len
        Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num++] = 0x40+0x08+1;//信息安全扩展区len
        memcpy(&Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num],&Encrypt_App[pdrv].Enc_rxbuff[4],0x40+0x08);
        num += (0x40+0x08);
        Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num++] = 0x01;//签名密钥标识
        Encrypt_App[pdrv].StoreDATA1Temp.status.Length_H = ((num+2+1)>>8)&0xff;
        Encrypt_App[pdrv].StoreDATA1Temp.status.Length_L = (num+2+1)&0xff;
        Encrypt_StoreIN(pdrv,&Encrypt_App[pdrv].StoreDATA1Temp);
    }
    else
    {
        Encrypt_ReturnFaultInfo(0x1f,Encrypt_App[pdrv].Enc_rxbuff[0],Encrypt_App[pdrv].Enc_rxbuff[1],pdrv);
    }
}

//长度/长度/签名结果/密钥标识
static void Encrypt_AppProcess_B_FUN52(BYTE pdrv)
{	
    WORD num=0;

    num=0;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x55;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x80;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x18;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x05;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x40;
    memcpy(&Encrypt_App[pdrv].Enc_txbuff[num],&Encrypt_App[pdrv].RxTemp.Data.buff[2],0x40);
    num += 0x40;
    Encrypt_App[pdrv].Enc_txbuff[num] = Encrypt_LRC(num-1,&Encrypt_App[pdrv].Enc_txbuff[1]);
    num++;

    Encrypt_ChipCOM(Encrypt_App[pdrv].Enc_txbuff,Encrypt_App[pdrv].Enc_rxbuff);

    if((Encrypt_App[pdrv].Enc_rxbuff[0]==0x90)&&(Encrypt_App[pdrv].Enc_rxbuff[1]==0x00))
    {
        Encrypt_App[pdrv].LinkFlag |= _ENCRYPT_G_CERSUCCESS;
    }
    Encrypt_ReturnFaultInfo(0x53,Encrypt_App[pdrv].Enc_rxbuff[0],Encrypt_App[pdrv].Enc_rxbuff[1],pdrv);
}

//长度/长度
static void Encrypt_AppProcess_B_FUN54(BYTE pdrv)
{	
    WORD num=0;

    num=0;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x55;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0xb0;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x99;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x05;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x02;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x08;
    Encrypt_App[pdrv].Enc_txbuff[num] = Encrypt_LRC(num-1,&Encrypt_App[pdrv].Enc_txbuff[1]);
    num++;

    Encrypt_ChipCOM(Encrypt_App[pdrv].Enc_txbuff,Encrypt_App[pdrv].Enc_rxbuff);

    if((Encrypt_App[pdrv].Enc_rxbuff[0]==0x90)&&(Encrypt_App[pdrv].Enc_rxbuff[1]==0x00))
    {
        Encrypt_App[pdrv].StoreDATA1Temp.Head.MesType_H = 0x00;
        Encrypt_App[pdrv].StoreDATA1Temp.Head.MesType_L = ((Encrypt_App[pdrv].Object&_ENCRYPT_OBJ)|(0x00&_ENCRYPT_ENC));
        Encrypt_App[pdrv].StoreDATA1Temp.Data.Type = 0x55;//应用类型
        num=0;
        Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num++] = 0x00;//信息安全扩展区len
        Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num++] = 0x08;//信息安全扩展区len
        memcpy(&Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num],&Encrypt_App[pdrv].Enc_rxbuff[4],0x08);
        num += 0x08;
        Encrypt_App[pdrv].StoreDATA1Temp.status.Length_H = ((num+2+1)>>8)&0xff;
        Encrypt_App[pdrv].StoreDATA1Temp.status.Length_L = (num+2+1)&0xff;
        Encrypt_StoreIN(pdrv,&Encrypt_App[pdrv].StoreDATA1Temp);
    }
    else
    {
        Encrypt_ReturnFaultInfo(0x1F,Encrypt_App[pdrv].Enc_rxbuff[0],Encrypt_App[pdrv].Enc_rxbuff[1],pdrv);
    }	
}

//长度/长度
static void Encrypt_AppProcess_B_FUN60(BYTE pdrv)
{
    BYTE keyversion;
    WORD num=0;

    num=0;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x55;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x80;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x1A;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
    Encrypt_App[pdrv].Enc_txbuff[num] = Encrypt_LRC(num-1,&Encrypt_App[pdrv].Enc_txbuff[1]);
    num++;

    Encrypt_ChipCOM(Encrypt_App[pdrv].Enc_txbuff,Encrypt_App[pdrv].Enc_rxbuff);

    if((Encrypt_App[pdrv].Enc_rxbuff[0]==0x90)&&(Encrypt_App[pdrv].Enc_rxbuff[1]==0x00))
    {
        keyversion = Encrypt_App[pdrv].Enc_txbuff[4];
    
        Encrypt_TakeRandomNum(Encrypt_App[pdrv].Enc_txbuff,Encrypt_App[pdrv].Enc_rxbuff);
    
        if((Encrypt_App[pdrv].Enc_rxbuff[0]==0x90)&&(Encrypt_App[pdrv].Enc_rxbuff[1]==0x00))
        {
            Encrypt_App[pdrv].StoreDATA1Temp.Head.MesType_H = 0x00;
            Encrypt_App[pdrv].StoreDATA1Temp.Head.MesType_L = ((Encrypt_App[pdrv].Object&_ENCRYPT_OBJ)|(0x00&_ENCRYPT_ENC));
            Encrypt_App[pdrv].StoreDATA1Temp.Data.Type = 0x61;//应用类型
            num=0;
            Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num++] = 0x00;//信息安全扩展区len
            Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num++] = 0x09;//信息安全扩展区len
            Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num++] = keyversion;
            memcpy(&Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num],&Encrypt_App[pdrv].Enc_rxbuff[4],0x08);
            num += 0x08;
            Encrypt_App[pdrv].StoreDATA1Temp.status.Length_H = ((num+2+1)>>8)&0xff;
            Encrypt_App[pdrv].StoreDATA1Temp.status.Length_L = (num+2+1)&0xff;
            Encrypt_StoreIN(pdrv,&Encrypt_App[pdrv].StoreDATA1Temp);
        }
        else
        {
            Encrypt_ReturnFaultInfo(0x1F,Encrypt_App[pdrv].Enc_rxbuff[0],Encrypt_App[pdrv].Enc_rxbuff[1],pdrv);
        }
    }
    else
    {
        Encrypt_ReturnFaultInfo(0x1F,Encrypt_App[pdrv].Enc_rxbuff[0],Encrypt_App[pdrv].Enc_rxbuff[1],pdrv);
    }		
}

//长度/长度/密钥包
static void Encrypt_AppProcess_B_FUN62(BYTE pdrv)
{	
    WORD num=0;
    WORD length=0;

    length = ((Encrypt_App[pdrv].RxTemp.Data.buff[0]<<8)|Encrypt_App[pdrv].RxTemp.Data.buff[1]);
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x55;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x80;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x1C;
    if(Encrypt_App[pdrv].RxTemp.Data.buff[2])
    {Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;}
    else
    {Encrypt_App[pdrv].Enc_txbuff[num++] = 0x01;}
    Encrypt_App[pdrv].Enc_txbuff[num++] = Encrypt_App[pdrv].RxTemp.Data.buff[(length-1)+2];//长度-1数组最后一位，-2报文长度
    Encrypt_App[pdrv].Enc_txbuff[num++] = ((length-1)>>8)&0xff;//-1ASKID
    Encrypt_App[pdrv].Enc_txbuff[num++] = ((length-1))&0xff;
    memcpy(&Encrypt_App[pdrv].Enc_txbuff[num],&Encrypt_App[pdrv].RxTemp.Data.buff[2],(length-1));
    num += (length-1);
    Encrypt_App[pdrv].Enc_txbuff[num] = Encrypt_LRC(num-1,&Encrypt_App[pdrv].Enc_txbuff[1]);
    num++;

    Encrypt_ChipCOM(Encrypt_App[pdrv].Enc_txbuff,Encrypt_App[pdrv].Enc_rxbuff);
    
    if((Encrypt_App[pdrv].Enc_rxbuff[0]==0x90)&&(Encrypt_App[pdrv].Enc_rxbuff[1]==0x00))
    {
        Encrypt_App[pdrv].LinkFlag |= _ENCRYPT_B_CERSUCCESS;
    }
    
    Encrypt_ReturnFaultInfo(0x63,Encrypt_App[pdrv].Enc_rxbuff[0],Encrypt_App[pdrv].Enc_rxbuff[1],pdrv);
}

//长度/长度/密钥包
static void Encrypt_AppProcess_B_FUN64(BYTE pdrv)
{
    WORD num=0;
    WORD length=0;

    length = ((Encrypt_App[pdrv].RxTemp.Data.buff[0]<<8)|Encrypt_App[pdrv].RxTemp.Data.buff[1]);
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x55;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x80;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x1C;
    if(Encrypt_App[pdrv].RxTemp.Data.buff[2])
    {Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;}
    else
    {Encrypt_App[pdrv].Enc_txbuff[num++] = 0x01;}
    Encrypt_App[pdrv].Enc_txbuff[num++] = Encrypt_App[pdrv].RxTemp.Data.buff[(length-1)+2];//长度-1数组最后一位，-2报文长度
    Encrypt_App[pdrv].Enc_txbuff[num++] = ((length-1)>>8)&0xff;//-1ASKID
    Encrypt_App[pdrv].Enc_txbuff[num++] = ((length-1))&0xff;
    memcpy(&Encrypt_App[pdrv].Enc_txbuff[num],&Encrypt_App[pdrv].RxTemp.Data.buff[2],(length-1));
    num += (length-1);
    Encrypt_App[pdrv].Enc_txbuff[num] = Encrypt_LRC(num-1,&Encrypt_App[pdrv].Enc_txbuff[1]);
    num++;

    Encrypt_ChipCOM(Encrypt_App[pdrv].Enc_txbuff,Encrypt_App[pdrv].Enc_rxbuff);
    
    Encrypt_ReturnFaultInfo(0x65,Encrypt_App[pdrv].Enc_rxbuff[0],Encrypt_App[pdrv].Enc_rxbuff[1],pdrv);
}

//长度/长度/证书标识/总帧数/当前帧序号/证书文件N
static void Encrypt_AppProcess_B_FUN70(BYTE pdrv)
{
    WORD num=0;
    WORD length=0;
    WORD length_n=0;
    WORD num_n;
    BYTE num_max = 0;
    BYTE i;
	
    Encrypt_VerifySign(Encrypt_App[pdrv].Enc_txbuff,Encrypt_App[pdrv].Enc_rxbuff,pdrv);
    
    if((Encrypt_App[pdrv].Enc_rxbuff[0]==0x90)&&(Encrypt_App[pdrv].Enc_rxbuff[1]==0x00))
    {    
        if(Encrypt_App[pdrv].RxTemp.Data.buff[4] == 1)//第一包清缓冲区
        {
            memset(Encrypt_App[pdrv].CerContent,0,sizeof(Encrypt_App[pdrv].CerContent));
        }

        num_n = 0;
        while(Encrypt_App[pdrv].CerContent[num_n+0] != 0)//查找未存储位置
        {
            if(Encrypt_App[pdrv].RxTemp.Data.buff[4] == Encrypt_App[pdrv].CerContent[num_n])//如果分帧包已存在，则跳出
            {break;}
            length_n = ((Encrypt_App[pdrv].CerContent[num_n+1]<<8)|Encrypt_App[pdrv].CerContent[num_n+2]);
            num_n = num_n+length_n+3;
            num_max++;
        }
        
        length_n = (((Encrypt_App[pdrv].RxTemp.Data.buff[0]<<8)|Encrypt_App[pdrv].RxTemp.Data.buff[1])-3)-6-64-1;//存储
        if((num_n +3 + length_n) < sizeof(Encrypt_App[pdrv].CerContent))
        {
            Encrypt_App[pdrv].CerContent[num_n+0] = Encrypt_App[pdrv].RxTemp.Data.buff[4];
            Encrypt_App[pdrv].CerContent[num_n+1] = (length_n>>8)&0xff;
            Encrypt_App[pdrv].CerContent[num_n+2] = length_n&0xff;
            memcpy(&Encrypt_App[pdrv].CerContent[num_n+3],&Encrypt_App[pdrv].RxTemp.Data.buff[5],length_n);
        }
        
        if((num_max+1) == Encrypt_App[pdrv].RxTemp.Data.buff[3])//已存储数量满足要求
        {			
            memset(Encrypt_App[pdrv].Enc_txbuff,0,sizeof(Encrypt_App[pdrv].Enc_txbuff));
            num=0;
            Encrypt_App[pdrv].Enc_txbuff[num++] = 0x55;
            Encrypt_App[pdrv].Enc_txbuff[num++] = 0x80;
            Encrypt_App[pdrv].Enc_txbuff[num++] = Encrypt_App[pdrv].RxTemp.Data.buff[2];
            Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
            for(i=0,length = 0;i<Encrypt_App[pdrv].RxTemp.Data.buff[3];i++)//组包
            {
                num_n = 0;length_n = 0;
                while(Encrypt_App[pdrv].CerContent[num_n+0] != 0)//查找位置
                {
                    length_n = ((Encrypt_App[pdrv].CerContent[num_n+1]<<8)|Encrypt_App[pdrv].CerContent[num_n+2]);
                    if(Encrypt_App[pdrv].CerContent[num_n+0] == (i+1))
                    {
                        memcpy(&Encrypt_App[pdrv].Enc_txbuff[num+2+length],&Encrypt_App[pdrv].CerContent[num_n+3],length_n);
                        break;
                    }
                    num_n = num_n+length_n+3;
                    if(num_n > sizeof(Encrypt_App[pdrv].CerContent))
                    {break;}
                }
                length += length_n;
            }
            Encrypt_App[pdrv].Enc_txbuff[num++] = (length>>8)&0xff;
            Encrypt_App[pdrv].Enc_txbuff[num++] = (length)&0xff;
            num += length;
            Encrypt_App[pdrv].Enc_txbuff[num] = Encrypt_LRC(num-1,&Encrypt_App[pdrv].Enc_txbuff[1]);
            
            Encrypt_ChipCOM(Encrypt_App[pdrv].Enc_txbuff,Encrypt_App[pdrv].Enc_rxbuff);//发送证书
            
            Encrypt_ReturnFaultInfo(0x71,Encrypt_App[pdrv].Enc_rxbuff[0],Encrypt_App[pdrv].Enc_rxbuff[1],pdrv);
        }
    }
    else
    {
        Encrypt_ReturnFaultInfo(0x1F,Encrypt_App[pdrv].Enc_rxbuff[0],Encrypt_App[pdrv].Enc_rxbuff[1],pdrv);
    }
}

//长度/长度/证书标识/总帧数/当前帧序号/证书文件N
static void Encrypt_AppProcess_B_FUN72(BYTE pdrv)
{	
    WORD num=0;
    WORD length=0;
    WORD length_n=0;
    WORD num_n;
    BYTE num_max = 0;
    BYTE i;
	
    if(Encrypt_App[pdrv].RxTemp.Data.buff[4] == 1)//第一包清缓冲区
    {
        memset(Encrypt_App[pdrv].CerContent,0,sizeof(Encrypt_App[pdrv].CerContent));
    }

    num_n = 0;
    while(Encrypt_App[pdrv].CerContent[num_n+0] != 0)//查找未存储位置
    {
        if(Encrypt_App[pdrv].RxTemp.Data.buff[4] == Encrypt_App[pdrv].CerContent[num_n])//如果分帧包已存在，则跳出
        {break;}
        length_n = ((Encrypt_App[pdrv].CerContent[num_n+1]<<8)|Encrypt_App[pdrv].CerContent[num_n+2]);
        num_n = num_n+length_n+3;
        num_max++;
    }
    
    length_n = (((Encrypt_App[pdrv].RxTemp.Data.buff[0]<<8)|Encrypt_App[pdrv].RxTemp.Data.buff[1])-3);//存储
    if((num_n +3 + length_n) < sizeof(Encrypt_App[pdrv].CerContent))
    {
        Encrypt_App[pdrv].CerContent[num_n+0] = Encrypt_App[pdrv].RxTemp.Data.buff[4];
        Encrypt_App[pdrv].CerContent[num_n+1] = (length_n>>8)&0xff;
        Encrypt_App[pdrv].CerContent[num_n+2] = length_n&0xff;
        memcpy(&Encrypt_App[pdrv].CerContent[num_n+3],&Encrypt_App[pdrv].RxTemp.Data.buff[5],length_n);
    }
    
    if((num_max+1) == Encrypt_App[pdrv].RxTemp.Data.buff[3])//已存储数量满足要求
    {			
        memset(Encrypt_App[pdrv].Enc_txbuff,0,sizeof(Encrypt_App[pdrv].Enc_txbuff));
        num=0;
        Encrypt_App[pdrv].Enc_txbuff[num++] = 0x55;
        Encrypt_App[pdrv].Enc_txbuff[num++] = 0x80;
        Encrypt_App[pdrv].Enc_txbuff[num++] = 0x24;
        Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
        Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
        for(i=0,length = 0;i<Encrypt_App[pdrv].RxTemp.Data.buff[3];i++)//组包
        {
            num_n = 0;length_n = 0;
            while(Encrypt_App[pdrv].CerContent[num_n+0] != 0)//查找位置
            {
                length_n = ((Encrypt_App[pdrv].CerContent[num_n+1]<<8)|Encrypt_App[pdrv].CerContent[num_n+2]);
                if(Encrypt_App[pdrv].CerContent[num_n+0] == (i+1))
                {
                    memcpy(&Encrypt_App[pdrv].Enc_txbuff[num+2+length],&Encrypt_App[pdrv].CerContent[num_n+3],length_n);
                    break;
                }
                num_n = num_n+length_n+3;
                if(num_n > sizeof(Encrypt_App[pdrv].CerContent))
                {break;}
            }
            length += length_n;
        }
        Encrypt_App[pdrv].Enc_txbuff[num++] = (length>>8)&0xff;
        Encrypt_App[pdrv].Enc_txbuff[num++] = (length)&0xff;
        num += length;
        Encrypt_App[pdrv].Enc_txbuff[num] = Encrypt_LRC(num-1,&Encrypt_App[pdrv].Enc_txbuff[1]);
        
        Encrypt_ChipCOM(Encrypt_App[pdrv].Enc_txbuff,Encrypt_App[pdrv].Enc_rxbuff);//发送证书
        
        Encrypt_ReturnFaultInfo(0x73,Encrypt_App[pdrv].Enc_rxbuff[0],Encrypt_App[pdrv].Enc_rxbuff[1],pdrv);
    }
}

//长度/长度/证书标识/总帧数/当前帧序号/终端证书N
static void Encrypt_AppProcess_B_FUN74(BYTE pdrv)
{	
    WORD eachnum=0xf0;
    WORD group=0;
    WORD num=0;
    WORD length=0;
    WORD length_n=0;
    BYTE i;
	
    num=0;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x55;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x80;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x30;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x01;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
    Encrypt_App[pdrv].Enc_txbuff[num++] = 0x00;
    Encrypt_App[pdrv].Enc_txbuff[num] = Encrypt_LRC(num-1,&Encrypt_App[pdrv].Enc_txbuff[1]);
    num++;
    Encrypt_ChipCOM(Encrypt_App[pdrv].Enc_txbuff,Encrypt_App[pdrv].Enc_rxbuff);	
        
    if((Encrypt_App[pdrv].Enc_rxbuff[0]==0x90)&&(Encrypt_App[pdrv].Enc_rxbuff[1]==0x00))
    {		
        length = ((Encrypt_App[pdrv].Enc_rxbuff[2]<<8)|Encrypt_App[pdrv].Enc_rxbuff[3]);
        group = (length%eachnum) ? (length/eachnum + 1)  : (length/eachnum);
        Encrypt_StatusReset(pdrv);
        for(i=0;i<group;i++)
        {
            Encrypt_App[pdrv].StoreDATA1Temp.Head.MesType_H = 0x00;
            Encrypt_App[pdrv].StoreDATA1Temp.Head.MesType_L = ((Encrypt_App[pdrv].Object&_ENCRYPT_OBJ)|(0x00&_ENCRYPT_ENC));
            Encrypt_App[pdrv].StoreDATA1Temp.Data.Type = 0x75;//应用类型
            num=0;
            length_n = ((i+1) == group) ? (length%eachnum)  : eachnum;
            Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num++] = ((length_n+3)>>8)&0xff;//信息安全扩展区len
            Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num++] = (length_n+3)&0xff;//信息安全扩展区len
            Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num++] = 6;
            Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num++] = group;
            Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num++] = i+1;
            memcpy(&Encrypt_App[pdrv].StoreDATA1Temp.Data.buff[num],&Encrypt_App[pdrv].Enc_rxbuff[4 + i*eachnum],length_n);
            num += length_n;
            Encrypt_App[pdrv].StoreDATA1Temp.status.Length_H = ((num+2+1)>>8)&0xff;
            Encrypt_App[pdrv].StoreDATA1Temp.status.Length_L = (num+2+1)&0xff;
            Encrypt_StoreIN(pdrv,&Encrypt_App[pdrv].StoreDATA1Temp);
        }
    }
    else
    {
            Encrypt_ReturnFaultInfo(0x1f,Encrypt_App[pdrv].Enc_rxbuff[0],Encrypt_App[pdrv].Enc_rxbuff[1],pdrv);
    }
}

void Encrypt_AppProcess(BYTE pdrv)
{		
	switch (Encrypt_App[pdrv].RxTemp.Data.Type)
	{
        //IEC    
        case _ENCRYPT_I_FUN00: // 应用类型为0
            Encrypt_AppProcess_I_FUN00(pdrv);
        break;
        
        case _ENCRYPT_I_FUN01: // 应用类型为1
            Encrypt_AppProcess_I_FUN01(pdrv);
        break;
                
        case _ENCRYPT_I_FUN03: // 应用类型为3
            Encrypt_AppProcess_I_FUN03(pdrv);
        break;
        
        case _ENCRYPT_I_FUN05: // 应用类型为5
            Encrypt_AppProcess_I_FUN05(pdrv);
        break;
                
        case _ENCRYPT_I_FUN07: // 应用类型为7
            Encrypt_AppProcess_I_FUN07(pdrv);
        break;
        
        case _ENCRYPT_I_FUN08: // 应用类型为8
//            Encrypt_AppProcess_I_FUN08(pdrv);
        break;
        
        //网关  
        case _ENCRYPT_G_FUN20: // 双向身份认证流程
            Encrypt_AppProcess_G_FUN20(pdrv);
            Encrypt_App[pdrv].LinkFlag &= (~_ENCRYPT_G_CERSUCCESS);
        break;
        
        case _ENCRYPT_G_FUN22: // 对终端认证请求响应
            Encrypt_AppProcess_G_FUN22(pdrv);
        break;

        //主站  
        case _ENCRYPT_B_FUN50: // 双向身份认证流程
            Encrypt_AppProcess_B_FUN50(pdrv);
            Encrypt_App[pdrv].LinkFlag &= (~_ENCRYPT_B_CERSUCCESS);
        break;
        
        case _ENCRYPT_B_FUN52: // 双向身份认证流程
            Encrypt_AppProcess_B_FUN52(pdrv);
        break;
          
        case _ENCRYPT_B_FUN54: // 获取终端芯片序列号
            Encrypt_AppProcess_B_FUN54(pdrv);
        break;
        
        case _ENCRYPT_B_FUN60: // 获取终端密钥版本
            Encrypt_AppProcess_B_FUN60(pdrv);
        break;
        
        case _ENCRYPT_B_FUN62: // 执行远程密钥更新
            Encrypt_AppProcess_B_FUN62(pdrv);
        break;
        
        case _ENCRYPT_B_FUN64: // 执行远程密钥恢复
            Encrypt_AppProcess_B_FUN64(pdrv);
        break;
        
        case _ENCRYPT_B_FUN70: // 证书远程更新
            Encrypt_AppProcess_B_FUN70(pdrv);
        break;
  
        case _ENCRYPT_B_FUN72: // 终端证书远程更新
            Encrypt_AppProcess_B_FUN72(pdrv);
        break;
        
        case _ENCRYPT_B_FUN74: // 提取终端证书
            Encrypt_AppProcess_B_FUN74(pdrv);
        break;
        
        //证书管理工具     
        case _ENCRYPT_M_FUN30: // 下发认证申请
            Encrypt_AppProcess_M_FUN30(pdrv);
            Encrypt_App[pdrv].LinkFlag &= (~_ENCRYPT_B_CERSUCCESS);
        break;

        case _ENCRYPT_M_FUN32: // 对终端认证请求响应
            Encrypt_AppProcess_M_FUN32(pdrv);
        break;
        
        case _ENCRYPT_M_FUN34: // 获取终端密钥版本
            Encrypt_AppProcess_M_FUN34(pdrv);
        break;

        case _ENCRYPT_M_FUN36: // 获取终端密钥版本
            Encrypt_AppProcess_M_FUN36(pdrv);
        break;
        
        case _ENCRYPT_M_FUN38: // 提取终端序列号
            Encrypt_AppProcess_M_FUN38(pdrv);
        break;
        
        case _ENCRYPT_M_FUN3A: // 提取安全芯片公钥
            Encrypt_AppProcess_M_FUN3A(pdrv);
        break;
        
        case _ENCRYPT_M_FUN3C: // 对证书请求文件进行签名
            Encrypt_AppProcess_M_FUN3C(pdrv);
        break;
                    
        case _ENCRYPT_M_FUN3E: // 将证书导入终端
            Encrypt_AppProcess_M_FUN3E(pdrv);
        break;
        
        case _ENCRYPT_M_FUN40: // 终端初始证书回写
            Encrypt_AppProcess_M_FUN40(pdrv);
        break;

        case _ENCRYPT_M_FUN42: // 导出终端证书
            Encrypt_AppProcess_M_FUN42(pdrv);
        break;

        case _ENCRYPT_M_FUN46: // 恢复终端密钥
            Encrypt_AppProcess_M_FUN46(pdrv);
        break;

        case _ENCRYPT_M_FUN48: // 提取发送终端数据处理
            Encrypt_AppProcess_M_FUN48(pdrv);
        break;
        
        default: // 类型标识有错误或不支持
        break;
	}
}

static void Encrypt_DecodeFrame(BYTE pdrv)
{
    Encrypt_App[pdrv].LinkFlag |= _ENCRYPT_SANDABLE;///0x00010000//加密程序可发送数据
    Encrypt_App[pdrv].Object = Encrypt_App[pdrv].RxdBuf[5]&_ENCRYPT_OBJ;///链接对象（主站00、运维工具01、网关10、11备用）
    
    Encrypt_App[pdrv].RxTemp.Head.MesType_H = Encrypt_App[pdrv].RxdBuf[4];///报文类型
    Encrypt_App[pdrv].RxTemp.Head.MesType_L = Encrypt_App[pdrv].RxdBuf[5];

    if (Encrypt_App[pdrv].RxTemp.Head.MesType_L & _ENCRYPT_ENC) // 密文
    {
        if(!Encrypt_Ciphertext(pdrv))
        {return;}
    }
    else  // 明文
    {
        if(!Encrypt_EncryptCheck(pdrv))
        {return;}
    }
    Encrypt_AppProcess(pdrv);///从加密芯片取出的数据进行处理 再发送给加密芯片或者--
}

void Encrypt_SearchValidFrame(BYTE pdrv)
{
    BYTE stop = 0;
    BYTE length = 0;
    
    if(!(Encrypt_App[pdrv].LinkFlag&_ENCRYPT_INITIAL))///如果未初始化 初始化
    {
        Encrypt_App[pdrv].LinkFlag |= _ENCRYPT_INITIAL;///0x00800000加密程序初始化
        Encrypt_StatusReset(pdrv);
    }	
		
    while ((Encrypt_App[pdrv].RxdHead < Encrypt_App[pdrv].RxdTail) && (!stop))
    {
        if (Encrypt_App[pdrv].RxdStatus == RXDSTART)
        {
            while ((Encrypt_App[pdrv].RxdBuf[Encrypt_App[pdrv].RxdHead] != _ENCRYPT_STARTCODE)
                    && (Encrypt_App[pdrv].RxdHead < Encrypt_App[pdrv].RxdTail))
            {
                Encrypt_App[pdrv].RxdHead++;
            }
				///找到启动0xEB
            if (Encrypt_App[pdrv].RxdHead < Encrypt_App[pdrv].RxdTail) // 找到启动字符，并将报文与缓冲区对齐。
            {
                Encrypt_App[pdrv].RxdStatus = RXDHEAD;
                if (Encrypt_App[pdrv].RxdHead != 0)
                {
                    memcpy(Encrypt_App[pdrv].RxdBuf,Encrypt_App[pdrv].RxdBuf+Encrypt_App[pdrv].RxdHead,Encrypt_App[pdrv].RxdTail-Encrypt_App[pdrv].RxdHead);
                    Encrypt_App[pdrv].RxdTail -= Encrypt_App[pdrv].RxdHead;
                    Encrypt_App[pdrv].RxdHead = 0;
                }
            }
        }
        
        Encrypt_App[pdrv].RxdLength = Encrypt_App[pdrv].RxdTail - Encrypt_App[pdrv].RxdHead;///总数据长度
        if (Encrypt_App[pdrv].RxdStatus == RXDHEAD) // 报文够一帧则处理
        {
            switch (Encrypt_App[pdrv].RxdBuf[Encrypt_App[pdrv].RxdHead])///报文头
            {
                case _ENCRYPT_STARTCODE://0xEB
                    if (Encrypt_App[pdrv].RxdLength >= (3+6)) // 报文长度最少9个字节。
                    {
                        if (!Encrypt_CheckFramValid(pdrv)) // 判断是否报文头
                        {
                            Encrypt_App[pdrv].RxdHead++;//不是报文头，返回重新获取
                            Encrypt_App[pdrv].RxdStatus = RXDSTART;
                        }
                        else
                        {
                            Encrypt_App[pdrv].RxdStatus = RXDCONTINUE;///是报文头 则进一步处理
                        }
                    }
                    else
                    {
                        stop = 1;
                    }
                    break;
            }
        }
				
        if (Encrypt_App[pdrv].RxdStatus == RXDCONTINUE)
        {
            switch (Encrypt_App[pdrv].RxdBuf[Encrypt_App[pdrv].RxdHead])
            {
              case _ENCRYPT_STARTCODE:/// EB 长度 长度L EB  ....... CS 16
                length = ((Encrypt_App[pdrv].RxdBuf[Encrypt_App[pdrv].RxdHead+1]<<8)|(Encrypt_App[pdrv].RxdBuf[Encrypt_App[pdrv].RxdHead+2]));
                if (Encrypt_App[pdrv].RxdLength >= length+6) // 报文收全，进行处理
                {
                    if ((Encrypt_App[pdrv].RxdBuf[Encrypt_App[pdrv].RxdHead+length+4] == (Encrypt_CKS(pdrv, Encrypt_App[pdrv].RxdBuf+Encrypt_App[pdrv].RxdHead)&0xFF))//校验正确
                            && (Encrypt_App[pdrv].RxdBuf[Encrypt_App[pdrv].RxdHead+length+4+1] == _ENCRYPT_ENDCODE)) // 结束码正确
 					          {
                        if ( Encrypt_App[pdrv].RxdHead > 0 )
                        {
                            memcpy(Encrypt_App[pdrv].RxdBuf,Encrypt_App[pdrv].RxdBuf+Encrypt_App[pdrv].RxdHead,Encrypt_App[pdrv].RxdTail-Encrypt_App[pdrv].RxdHead);//报文与缓冲区对齐
                            Encrypt_App[pdrv].RxdTail -= Encrypt_App[pdrv].RxdHead;
                            Encrypt_App[pdrv].RxdHead = 0;
                        }
												/* 头指针后移到报文后 */
                        Encrypt_App[pdrv].RxdHead += (Encrypt_App[pdrv].RxdBuf[1]+6);
                        Encrypt_App[pdrv].RxdStatus = RXDSTART;

                        Encrypt_DecodeFrame(pdrv);///数据解析  发给加密芯片数据并得到数据
                    }
                    else
                    {
                        Encrypt_App[pdrv].RxdHead += 6;
                        Encrypt_App[pdrv].RxdStatus = RXDSTART;
                    }
                }
                else // 报文未收全，不处理，继续等待数据。
                {
                    stop = 1;
                }
                break;
            }
        }
    }
}

WORD Encrypt_ReadMISIData(BYTE *pbuf, BYTE pdrv)
{
    WORD count = 0;
    WORD len = 0;
    
    if (Encrypt_App[pdrv].RxdHead < Encrypt_App[pdrv].RxdTail) // 接收缓冲区中有尚未处理的数据
    {
        if (Encrypt_App[pdrv].RxdHead != 0)
        {
            memcpy(Encrypt_App[pdrv].RxdBuf, Encrypt_App[pdrv].RxdBuf+Encrypt_App[pdrv].RxdHead, Encrypt_App[pdrv].RxdTail-Encrypt_App[pdrv].RxdHead);//将未处理的数据移到缓冲区头
            Encrypt_App[pdrv].RxdTail -= Encrypt_App[pdrv].RxdHead;
            Encrypt_App[pdrv].RxdHead = 0;
        }
    }
    else
    {
        Encrypt_App[pdrv].RxdHead = 0;
		Encrypt_App[pdrv].RxdTail = 0;
    }
    ///注释 读数据 直接从串口读出数据
    count = Encrypt_ReadData((BYTE *)&Encrypt_App[pdrv].RxdBuf[Encrypt_App[pdrv].RxdTail], _ENCRYPT_LPDUSIZE, pdrv);
    if (count > 0)
    {
        Encrypt_App[pdrv].RxdTail += count;///尾指针后移
        Encrypt_App[pdrv].IECLength = 0;///IEC数据长度置0
        memset(Encrypt_App[pdrv].IECBuf,0,sizeof(Encrypt_App[pdrv].IECBuf));///IEC接收缓冲区清0
			
        Encrypt_SearchValidFrame(pdrv);///数据判断处理 发给加密芯片数据并读取
			
        len = Encrypt_App[pdrv].IECLength;//IEC长度 101数据
        memcpy(pbuf,Encrypt_App[pdrv].IECBuf,len);///101数据放在 pbuf中
    }
		
		return(len);
}

BYTE Encrypt_ENCSendData(BYTE pdrv)
{
    BYTE res = FALSE;
    WORD length;
		///开始发送给加密芯片数据有误时，1级数据指针++ 或者其他数据交互
    if((Encrypt_App[pdrv].StoreDATA1OUT != Encrypt_App[pdrv].StoreDATA1IN)&&(Encrypt_App[pdrv].LinkFlag&_ENCRYPT_SANDABLE))
    {
        Encrypt_App[pdrv].LinkFlag &= (~_ENCRYPT_SANDABLE);
        length = ((Encrypt_App[pdrv].StoreDATA1OUT->status.Length_H<<8)|Encrypt_App[pdrv].StoreDATA1OUT->status.Length_L);
        memcpy(&Encrypt_App[pdrv].TxTemp,Encrypt_App[pdrv].StoreDATA1OUT,length+2);
        Encrypt_PackSend(pdrv);
        
        memset((BYTE *)Encrypt_App[pdrv].StoreDATA1OUT,0,sizeof(_ENCRYPT_LPDUSIZE));
        if(++Encrypt_App[pdrv].StoreDATA1OUT >= Encrypt_App[pdrv].StoreDATA1Buf + _ENCRYPT_STOREDATA1NUM)
        {
            Encrypt_App[pdrv].StoreDATA1OUT = Encrypt_App[pdrv].StoreDATA1Buf;
        }
        res = TRUE;
    }

    return(res);
}

BYTE Encrypt_AppInit(BYTE pdrv)
{   
    if (Encrypt_App == RT_NULL)
	{
		Encrypt_App = rt_malloc(sizeof(ENCRYPT_APPINFO) * _ENCRYPT_VOLUMES);
	}

    memset (&Encrypt_App[pdrv], 0, sizeof (ENCRYPT_APPINFO));
	  
    return(TRUE);
}

BYTE Encrypt_IECSendData(BYTE *pbuf, WORD count, BYTE type, BYTE pdrv)
{
    BYTE res = FALSE;
    
    if(Encrypt_App[pdrv].LinkFlag&0x00810000)///后台认证()_ENCRYPT_B_CERSUCCESS 0x40000000  change by chen
    {
        Encrypt_App[pdrv].TxTemp.status.Length_H = ((count+1+1+2)>>8)&0xff;///数据长度    没有信息安全扩展区？
        Encrypt_App[pdrv].TxTemp.status.Length_L = ((count+1+1+2))&0xff;
        Encrypt_App[pdrv].TxTemp.Head.MesType_H = 0x00;///报文类型
        Encrypt_App[pdrv].TxTemp.Head.MesType_L = ((Encrypt_App[pdrv].Object&_ENCRYPT_OBJ)|((type>>4)&_ENCRYPT_ENC));
        Encrypt_App[pdrv].TxTemp.Data.Type = type&0x7f;
        Encrypt_App[pdrv].TxTemp.Data.buff[0] = count;
        memcpy(&Encrypt_App[pdrv].TxTemp.Data.buff[1],pbuf,count);
        Encrypt_PackSend(pdrv);
        res = TRUE;
    }

    return(res);
}


