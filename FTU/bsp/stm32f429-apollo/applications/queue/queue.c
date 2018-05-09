/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      queue.c
  * @brief:     This file is part of systerm.
  * @version:   V1.0.1 
  * @author:    Lexun
  * @date:      2015.02.08
  * @update:    [2017-12-06][Lexun][make code cleanup]
  */
	
	
#define  QUEUE_GLOBALS


/* INCLUDE FILES -------------------------------------------------------------*/
#include "queue.h"
#include "stm32f4xx_hal.h"


/* PUBLIC FUNCTION PROTOTYPES ------------------------------------------------*/
/**
  * @brief : Create the queue.
  * @param : [Buf] the address of queue.
  * @param : [SizeOfBuf] size of the queue.
  * @param : [ReadEmpty] callback function of reading empty.
  * @param : [WriteFull] the callback function of writing full.
  * @return: [QUEUE_OK] sucess.
  * @return: [QUEUE_ERR] error.
  * @updata: [2017-12-06][Lexun][make code cleanup]
  */  
QUEUE_U8 QueueCreate(void *BufPCB, void *Buf, QUEUE_U32 SizeOfBuf, QUEUE_U8 (* ReadEmpty)(), QUEUE_U8 (* WriteFull)())
{
    DataQueue *Queue;

    if (Buf != NULL && SizeOfBuf >= (sizeof(DataQueue)))     
    {
        Queue = (DataQueue *)BufPCB;
        
        Queue->Buf = Buf;
        Queue->MaxData = SizeOfBuf;            
        Queue->End = Queue->Buf+Queue->MaxData;            
        Queue->Out = Queue->Buf;
        Queue->In  = Queue->Buf;
        Queue->ReadEmpty = ReadEmpty;
        Queue->WriteFull = WriteFull;

        return (QUEUE_OK);
    }
    else
    {
        return (QUEUE_ERR);
    }
}

/**
  * @brief : The callback function of writefull.
  * @param : [Buf] the address of queue.
  * @param : [Data] data.
  * @param : [WriteMode] the writing mode.
  * @return: [QUEUE_OK] sucess.
  * @return: [QUEUE_ERR] error.
  * @updata: [2017-12-06][Lexun][make code cleanup]
  */  
#ifndef EN_QUEUE_FULL
#define EN_QUEUE_FULL        1
#endif /* END EN_QUEUE_FULL */

#if EN_QUEUE_FULL

void WriteFull(void *Buf, QUEUE_U8 Data, QUEUE_U8 WriteMode)
{
    DataQueue *Queue;
    
    if (Buf != NULL)                                              
    {                                                              
        Queue = (DataQueue *)Buf;

		    switch(WriteMode)
		    {
		    case Q_WRITE_MODE:
			    if (Queue->NData >= Queue->MaxData)
          {
            Queue->In = Queue->Buf;
//				    Queue->NData = Queue->MaxData;
				    Queue->NData = 0;
						
            Queue->In[0] = Data;                                        
            Queue->In++;												
            if (Queue->In >= Queue->End)
            {
                Queue->In = Queue->Buf;
            }
            Queue->NData++;                                           
//            err = QUEUE_OK;				
          }
			    break;

			  case Q_WRITE_FRONT_MODE:
			    break;

			  default:
			    break;
		    }
    }
}
#endif /* END EN_QUEUE_FULL */

/**
  * @brief : Read the queue.
  * @param : [Buf] the address of queue.
  * @param : [Ret] the address of message.
  * @param : [Buf] the writing mode.
  * @return: [QUEUE_OK] sucess.
  * @return: [QUEUE_ERR] error.
  * @return: [QUEUE_EMPTY] NULL.
  * @updata: [2017-12-06][Lexun][make code cleanup]
  */  
QUEUE_U8 QueueRead(QUEUE_U8 *Ret, void *Buf)
{
    QUEUE_U8 err;
    DataQueue *Queue;
   
    err = QUEUE_ERR;
    if (Buf != NULL)                                            
    {                                                        
        Queue = (DataQueue *)Buf;
                
//        if (Queue->NData > 0)  
				if(Queue->Out != Queue->In)
        {                                                      
            *Ret = Queue->Out[0];                             
            Queue->Out++;                                       
            if (Queue->Out >= Queue->End)
            {
                Queue->Out = Queue->Buf;
            }
 //           Queue->NData--;                                    
            err = QUEUE_OK;
        }
        else
        {                                                      
            err = QUEUE_EMPTY;
            if (Queue->ReadEmpty != NULL)                      
            {
                err = Queue->ReadEmpty(Ret, Queue);
            }
        }
    }

    return (err);
}

/**
  * @brief : Write the queue by FIFO mode.
  * @param : [Data] the message data writing to queue.
  * @param : [Buf] the point of the message queue.
  * @return: [QUEUE_OK] sucess.
  * @return: [QUEUE_ERR] error.
  * @return: [QUEUE_FULL] FULL.
  * @updata: [2017-12-06][Lexun][make code cleanup]
  */  
#ifndef EN_QUEUE_WRITE
#define EN_QUEUE_WRITE      1
#endif /* END EN_QUEUE_WRITE */

#if EN_QUEUE_WRITE

QUEUE_U8 QueueWrite(void *Buf, QUEUE_U8 Data)
{
    QUEUE_U8 err;
    DataQueue *Queue;
   
    err = QUEUE_ERR;
    if (Buf != NULL)                                                   
    {
        Queue = (DataQueue *)Buf;
                
//        if (Queue->NData < Queue->MaxData)                              
//        {                                                              
            Queue->In[0] = Data;                                        
            Queue->In++;												
            if (Queue->In >= Queue->End)
            {
                Queue->In = Queue->Buf;
            }
//            Queue->NData++;                                           
            err = QUEUE_OK;
//        }
//        else
//        {                                                               
//            err = QUEUE_FULL;
//            if (Queue->WriteFull != NULL)                              
//            {
//                err = Queue->WriteFull(Queue, Data, Q_WRITE_MODE);
//            }
//			      else
//			      {
//			          WriteFull(Queue, Data, Q_WRITE_MODE);
//			      }
//        }
    }

    return (err);
}

/**
  * @brief : Write block data to queue by FIFO mode.
  * @param : [Data] the message data writing to queue.
  * @param : [Buf] the point of the message queue.
  * @return: [QUEUE_OK] sucess.
  * @return: [QUEUE_ERR] error.
  * @return: [QUEUE_FULL] FULL.
  * @updata: [2017-12-06][Lexun][make code cleanup]
  */  
QUEUE_U8 QueueWriteBlock(void *Buf, QUEUE_U8 *pData, QUEUE_U16 len)
{
    QUEUE_U8 err;
    QUEUE_U16 i = 0;
	
    for (i = 0; i < len; i++)
    {
        err = QueueWrite(Buf, pData[i]);
        if (err != QUEUE_OK)
        {
            break;
        }
    }
		
    return (err);
}

#endif /* END EN_QUEUE_WRITE */

/**
  * @brief : Write data to queue by LIFO mode.
  * @param : [Data] the message data writing to queue.
  * @param : [Buf] the point of the message queue.
  * @return: [QUEUE_OK] sucess.
  * @return: [QUEUE_FULL] FULL.
  * @updata: [2017-12-06][Lexun][make code cleanup]
  */  
#if EN_QUEUE_WRITE_FRONT
QUEUE_U8 QueueWriteFront(void *Buf, QUEUE_U8 Data)
{
    QUEUE_U8 err;
    DataQueue *Queue;

    err = QUEUE_ERR;
    if (Buf != NULL)                                                    
    {
        Queue = (DataQueue *)Buf;
                
        if (Queue->NData < Queue->MaxData)                              
        {                                                             
            Queue->Out--;                                              
            if (Queue->Out < Queue->Buf)
            {
                Queue->Out = Queue->End-1;
            }
            Queue->Out[0] = Data;                                     
            Queue->NData++;                                            
            err = QUEUE_OK;
        }
        else
        {                                                               
            err = QUEUE_FULL;
            if (Queue->WriteFull != NULL)                              
            {
                err = Queue->WriteFull(Queue, Data, Q_WRITE_FRONT_MODE);
            }
        }
    }
    return (err);
}
#endif /* END EN_QUEUE_WRITE_FRONT */

/**
  * @brief : Get the data length of queue, by BYTE.
  * @param : [Buf] the point of the message queue.
  * @return: [temp] Return the length of data in the queue.
  * @updata: [2017-12-06][Lexun][make code cleanup]
  */  
#if EN_QUEUE_NDATA
unsigned short QueueNData(void *Buf)
{
    unsigned short temp;
   
    temp = 0;                                              
    if (Buf != NULL)
    {
        temp = ((DataQueue *)Buf)->NData;
    }
    return (temp);
}

#endif /* END EN_QUEUE_NDATA */

/**
  * @brief : Get the length of queue, by BYTE.
  * @param : [Buf] the point of the message queue.
  * @return: [temp] Return the length of queue.
  * @updata: [2017-12-06][Lexun][make code cleanup]
  */  
#ifndef EN_QUEUE_SIZE
#define EN_QUEUE_SIZE    1
#endif /* END EN_QUEUE_SIZE */
 
#if EN_QUEUE_SIZE
unsigned short QueueSize(void *Buf)
{
    unsigned short temp;
   
    temp = 0;                                                   
    if (Buf != NULL)
    {
        temp = ((DataQueue *)Buf)->MaxData;
    }

    return (unsigned short)(temp);
}

#endif /* END EN_QUEUE_SIZE */

/**
  * @brief : Empty the queue.
  * @param : none.
  * @return: none.
  * @updata: [2017-12-06][Lexun][make code cleanup]
  */  
#ifndef EN_QUEUE_FLUSH
#define EN_QUEUE_FLUSH    0
#endif /* END EN_QUEUE_FLUSH */

#if EN_QUEUE_FLUSH

void QueueFlush(void *Buf)
{
    DataQueue *Queue;
    
    if (Buf != NULL)                                              
    {                                                              
        Queue = (DataQueue *)Buf;
        Queue->Out = Queue->Buf;
        Queue->In = Queue->Buf;
        Queue->NData = 0;                                          
    }
}
#endif /* END EN_QUEUE_FLUSH */


/* END OF FILE ----------------------------------------------------------------*/ 



