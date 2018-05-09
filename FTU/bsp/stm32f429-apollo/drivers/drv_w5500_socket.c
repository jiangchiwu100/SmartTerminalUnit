/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      drv_socket.c
  * @brief:     The driver of socket by W5500.
  * @version:   V1.0.1 
  * @author:    Lexun
  * @date:      2017-12-06
  * @update:    [2017-12-07][Lexun][make the code cleanup]
  */


/* INCLUDE FILES -------------------------------------------------------------*/
#include "drv_w5500_socket.h"


/* PRIVATE VARIABLES ---------------------------------------------------------*/
static uint16_t sock_any_port = W5500_SOCK_ANY_PORT_NUM;
static uint16_t sock_io_mode = 0;
static uint16_t sock_is_sending = 0;
static uint16_t sock_remained_size[_WIZCHIP_SOCK_NUM_] = {0,0,};
static uint8_t  sock_pack_info[_WIZCHIP_SOCK_NUM_] = {0,};

#if _WIZCHIP_ == 5200
   static uint16_t sock_next_rd[_WIZCHIP_SOCK_NUM_] ={0,};
#endif

#define W5500_CHECK_SOCKNUM()   \
   do{                    \
      if(sn > _WIZCHIP_SOCK_NUM_) return W5500_SOCKERR_SOCKNUM;   \
   }while(0);             \

#define W5500_CHECK_SOCKMODE(mode)  \
   do{                     \
      if((getSn_MR(sn) & 0x0F) != mode) return W5500_SOCKERR_SOCKMODE;  \
   }while(0);              \

#define W5500_CHECK_SOCKINIT()   \
   do{                     \
      if((getSn_SR(sn) != SOCK_INIT)) return W5500_SOCKERR_SOCKINIT; \
   }while(0);              \

#define W5500_CHECK_SOCKDATA()   \
   do{                     \
      if(len == 0) return W5500_SOCKERR_DATALEN;   \
   }while(0);              \


/* PUBLIC FUNCTION PROTOTYPES ------------------------------------------------*/

/**
  * @brief : Create the socket of W5500.
  * @param : [sn]
  * @param : [protocol]
  * @param : [port]
  * @param : [flag]
  * @return: none
  * @updata: [2017-12-07][Lexun][make the code cleanup]
  */  
int8_t w5500_socket(uint8_t sn, uint8_t protocol, uint16_t port, uint8_t flag)
{
    W5500_CHECK_SOCKNUM();
	  switch (protocol)
	  {
        case Sn_MR_TCP :
        case Sn_MR_UDP :
        case Sn_MR_MACRAW :
             break;
   #if ( _WIZCHIP_ < 5200 )
        case Sn_MR_IPRAW :
        case Sn_MR_PPPoE :
             break;
   #endif
        default :
             return W5500_SOCKERR_SOCKMODE;
	  }
	  if ((flag & 0x06) != 0) 
		{
        return W5500_SOCKERR_SOCKFLAG;		
		}
#if _WIZCHIP_ == 5200
    if (flag & 0x10) 
		{
        return SOCKERR_SOCKFLAG;		
		}
#endif
	   
	  if (flag != 0)
	  {
        switch (protocol)
   	    {
   	        case Sn_MR_TCP:
   	            if ((flag & (W5500_SF_TCP_NODELAY|W5500_SF_IO_NONBLOCK))==0) 
							  {
                    return W5500_SOCKERR_SOCKFLAG;							
							  }
   	            break;
   	        case Sn_MR_UDP:
   	            if (flag & W5500_SF_IGMP_VER2)
   	            {
   	                if ((flag & W5500_SF_MULTI_ENABLE)==0) 
										{
										    return W5500_SOCKERR_SOCKFLAG;
										}
   	            }
   	          #if _WIZCHIP_ == 5500
      	        if (flag & W5500_SF_UNI_BLOCK)
      	        {
      	           if ((flag & W5500_SF_MULTI_ENABLE) == 0) 
									 {
									     return W5500_SOCKERR_SOCKFLAG;
									 }
      	        }
   	          #endif
   	            break;
   	        default:
   	            break;
   	     }
    }
	  w5500_close(sn);
	  setSn_MR(sn, (protocol | (flag & 0xF0)));
	  if (!port)
	  {
	      port = sock_any_port++;
	      if (sock_any_port == 0xFFF0) 
			  {
			      sock_any_port = W5500_SOCK_ANY_PORT_NUM;
			  }
	  }
    setSn_PORT(sn,port);	
    setSn_CR(sn,Sn_CR_OPEN);
    while (getSn_CR(sn));
	  sock_io_mode |= ((flag & W5500_SF_IO_NONBLOCK) << sn);   
    sock_is_sending &= ~(1<<sn);
    sock_remained_size[sn] = 0;
    sock_pack_info[sn] = 0;
    while (getSn_SR(sn) == SOCK_CLOSED);
    return (int8_t)sn;
}	   

int8_t w5500_close(uint8_t sn)
{
    W5500_CHECK_SOCKNUM();
	
	  setSn_CR(sn,Sn_CR_CLOSE);
    /* wait to process the command... */
	  while ( getSn_CR(sn) );
	  /* clear all interrupt of the socket. */
	  setSn_IR(sn, 0xFF);
	  sock_is_sending &= ~(1<<sn);
	  sock_remained_size[sn] = 0;
	  sock_pack_info[sn] = 0;
	  while(getSn_SR(sn) != SOCK_CLOSED);
	  return W5500_SOCK_OK;
}

int8_t w5500_listen(uint8_t sn)
{
    W5500_CHECK_SOCKNUM();
	  W5500_CHECK_SOCKMODE(Sn_MR_TCP);
	  W5500_CHECK_SOCKINIT();
	  setSn_CR(sn,Sn_CR_LISTEN);
	  while (getSn_CR(sn));
    while (getSn_SR(sn) != SOCK_LISTEN)
    {
        if (getSn_CR(sn) == SOCK_CLOSED)
        {
            w5500_close(sn);
            return W5500_SOCKERR_SOCKCLOSED;
        }
    }
    return W5500_SOCK_OK;
}


int8_t w5500_connect(uint8_t sn, uint8_t * addr, uint16_t port)
{
    W5500_CHECK_SOCKNUM();
    W5500_CHECK_SOCKMODE(Sn_MR_TCP);
    W5500_CHECK_SOCKINIT();
    //M20140501 : For avoiding fatal error on memory align mismatched
    //if( *((uint32_t*)addr) == 0xFFFFFFFF || *((uint32_t*)addr) == 0) return SOCKERR_IPINVALID;
    {
        uint32_t taddr;
        taddr = ((uint32_t)addr[0] & 0x000000FF);
        taddr = (taddr << 8) + ((uint32_t)addr[1] & 0x000000FF);
        taddr = (taddr << 8) + ((uint32_t)addr[2] & 0x000000FF);
        taddr = (taddr << 8) + ((uint32_t)addr[0] & 0x000000FF);
        if( taddr == 0xFFFFFFFF || taddr == 0) 
				{
				    return W5500_SOCKERR_IPINVALID;
				}
    }
	
	  if(port == 0) return W5500_SOCKERR_PORTZERO;
	  setSn_DIPR(sn,addr);
	  setSn_DPORT(sn,port);
    #if _WIZCHIP_ == 5200   // for W5200 ARP errata 
      setSUBR(0);
    #endif
	  setSn_CR(sn,Sn_CR_CONNECT);
    while (getSn_CR(sn));
    if (sock_io_mode & (1<<sn))
    {
		    return W5500_SOCK_BUSY;
		}			
    while (getSn_SR(sn) != SOCK_ESTABLISHED)
    {   
		    if (getSn_IR(sn) & Sn_IR_TIMEOUT)
		    {
			      setSn_IR(sn, Sn_IR_TIMEOUT);
          #if _WIZCHIP_ == 5200   // for W5200 ARP errata 
            setSUBR((uint8_t*)"\x00\x00\x00\x00");
          #endif
            return W5500_SOCKERR_TIMEOUT;
		    }
	  }
    #if _WIZCHIP_ == 5200   // for W5200 ARP errata 
      setSUBR((uint8_t*)"\x00\x00\x00\x00");
    #endif
		
    return W5500_SOCK_OK;
}

int8_t w5500_disconnect(uint8_t sn)
{
    W5500_CHECK_SOCKNUM();
    W5500_CHECK_SOCKMODE(Sn_MR_TCP);
	  setSn_CR(sn,Sn_CR_DISCON);
	  /* wait to process the command... */
	  while (getSn_CR(sn));
	  sock_is_sending &= ~(1<<sn);
    if (sock_io_mode & (1<<sn))
    {
		    return W5500_SOCK_BUSY;
		}			
	  while (getSn_SR(sn) != SOCK_CLOSED)
	  {
	      if (getSn_IR(sn) & Sn_IR_TIMEOUT)
	      {
	          w5500_close(sn);
	          return W5500_SOCKERR_TIMEOUT;
	      }
	  }
	  return W5500_SOCK_OK;
} 

int32_t w5500_send(uint8_t sn, uint8_t * buf, uint16_t len)
{
    uint8_t tmp=0;
    uint16_t freesize=0;
   
    W5500_CHECK_SOCKNUM();
    W5500_CHECK_SOCKMODE(Sn_MR_TCP);
    W5500_CHECK_SOCKDATA();
    tmp = getSn_SR(sn);
    if (tmp != SOCK_ESTABLISHED && tmp != SOCK_CLOSE_WAIT)
		{
		    return W5500_SOCKERR_SOCKSTATUS;
		}			
    if (sock_is_sending & (1<<sn) )
    { 
        tmp = getSn_IR(sn);
        if (tmp & Sn_IR_SENDOK)
        {
            setSn_IR(sn, Sn_IR_SENDOK);
          #if _WZICHIP_ == 5200
            if (getSn_TX_RD(sn) != sock_next_rd[sn])
            {
                setSn_CR(sn,Sn_CR_SEND);
                while(getSn_CR(sn));
                return SOCKERR_BUSY;
            }
          #endif
            sock_is_sending &= ~(1<<sn);         
        }
        else if(tmp & Sn_IR_TIMEOUT)
        {
            w5500_close(sn);
            return W5500_SOCKERR_TIMEOUT;
        }
        else 
				{
				    return W5500_SOCK_BUSY;
				}
    }
    freesize = getSn_TxMAX(sn);
		
    if (len > freesize) 
		{
		    len = freesize; // check size not to exceed MAX size.
		}
		
    while (1)
    {
        freesize = getSn_TX_FSR(sn);
        tmp = getSn_SR(sn);
        if ((tmp != SOCK_ESTABLISHED) && (tmp != SOCK_CLOSE_WAIT))
        {
            w5500_close(sn);
            return W5500_SOCKERR_SOCKSTATUS;
        }
        if ((sock_io_mode & (1<<sn)) && (len > freesize) )
				{
					  return W5500_SOCK_BUSY;
				}				
        if (len <= freesize)
				{
				    break;
				}					
    }
    wiz_send_data(sn, buf, len);
    #if _WIZCHIP_ == 5200
      sock_next_rd[sn] = getSn_TX_RD(sn) + len;
    #endif
    setSn_CR(sn,Sn_CR_SEND);
    /* wait to process the command... */
    while (getSn_CR(sn));
    sock_is_sending |= (1 << sn);
    return len;
}


int32_t w5500_recv(uint8_t sn, uint8_t * buf, uint16_t len)
{
    uint8_t  tmp = 0;
    uint16_t recvsize = 0;
    W5500_CHECK_SOCKNUM();
    W5500_CHECK_SOCKMODE(Sn_MR_TCP);
    W5500_CHECK_SOCKDATA();
   
    recvsize = getSn_RxMAX(sn);
	
    if (recvsize < len)
		{
		    len = recvsize;
		}
		
    while(1)
    {
        recvsize = getSn_RX_RSR(sn);
        tmp = getSn_SR(sn);
        if (tmp != SOCK_ESTABLISHED)
        {
            if (tmp == SOCK_CLOSE_WAIT)
            {
                if (recvsize != 0) 
								{
								    break;
								}
                else if(getSn_TX_FSR(sn) == getSn_TxMAX(sn))
                {
                    w5500_close(sn);
                    return W5500_SOCKERR_SOCKSTATUS;
                }
            }
            else
            {
                w5500_close(sn);
                return W5500_SOCKERR_SOCKSTATUS;
            }
        }
        if ((sock_io_mode & (1<<sn)) && (recvsize == 0)) 
				{
				    return W5500_SOCK_BUSY;
				}
        if (recvsize != 0)
				{
				    break;
				}					
    };
    if (recvsize < len) 
		{
		    len = recvsize;
		}
    wiz_recv_data(sn, buf, len);
    setSn_CR(sn,Sn_CR_RECV);
    while(getSn_CR(sn));
    return len;
}

int32_t w5500_sendto(uint8_t sn, uint8_t * buf, uint16_t len, uint8_t * addr, uint16_t port)
{
    uint8_t tmp = 0;
    uint16_t freesize = 0;
    W5500_CHECK_SOCKNUM();
    switch (getSn_MR(sn) & 0x0F)
    {
        case Sn_MR_UDP:
        case Sn_MR_MACRAW:
             break;
        default:
             return W5500_SOCKERR_SOCKMODE;
    }
    W5500_CHECK_SOCKDATA();
    //M20140501 : For avoiding fatal error on memory align mismatched
    //if (*((uint32_t*)addr) == 0) return SOCKERR_IPINVALID;
    {
        uint32_t taddr;
        taddr = ((uint32_t)addr[0]) & 0x000000FF;
        taddr = (taddr << 8) + ((uint32_t)addr[1] & 0x000000FF);
        taddr = (taddr << 8) + ((uint32_t)addr[2] & 0x000000FF);
        taddr = (taddr << 8) + ((uint32_t)addr[3] & 0x000000FF);
    }

   if (*((uint32_t*)addr) == 0) 
	 {
	     return W5500_SOCKERR_IPINVALID;
	 }
   if (port == 0)        
	 {
	     return W5500_SOCKERR_PORTZERO;
	 } 
   tmp = getSn_SR(sn);
	 
   if (tmp != SOCK_MACRAW && tmp != SOCK_UDP)
	 {
	     return W5500_SOCKERR_SOCKSTATUS;
	 }		
      
   setSn_DIPR(sn,addr);
   setSn_DPORT(sn,port);      
   freesize = getSn_TxMAX(sn);
   if (len > freesize)
	 {
	     len = freesize; // check size not to exceed MAX size.
	 }		 
   while(1)
   {
       freesize = getSn_TX_FSR(sn);
       if (getSn_SR(sn) == SOCK_CLOSED)
			 {
			     return W5500_SOCKERR_SOCKCLOSED;
			 }				 
       if ((sock_io_mode & (1<<sn)) && (len > freesize) )
			 {
			     return W5500_SOCK_BUSY;
			 }				 
       if (len <= freesize) 
			 {
			     break;
			 }
   };
	 wiz_send_data(sn, buf, len);

   #if _WIZCHIP_ == 5200   // for W5200 ARP errata 
     setSUBR(0);
   #endif

	 setSn_CR(sn,Sn_CR_SEND);
	 /* wait to process the command... */
	 while(getSn_CR(sn));
   #if _WIZCHIP_ == 5200   // for W5200 ARP errata 
     setSUBR((uint8_t*)"\x00\x00\x00\x00");
   #endif
   while(1)
   {
       tmp = getSn_IR(sn);
       if (tmp & Sn_IR_SENDOK)
       {
           setSn_IR(sn, Sn_IR_SENDOK);
           break;
       }
      //M:20131104
      //else if(tmp & Sn_IR_TIMEOUT) return SOCKERR_TIMEOUT;
       else if (tmp & Sn_IR_TIMEOUT)
       {
           setSn_IR(sn, Sn_IR_TIMEOUT);
           return W5500_SOCKERR_TIMEOUT;
       }
      ////////////
    }
	  return len;
}



int32_t w5500_recvfrom(uint8_t sn, uint8_t * buf, uint16_t len, uint8_t * addr, uint16_t *port)
{
    uint8_t  mr;
    uint8_t  head[8];
	  uint16_t pack_len=0;

    W5500_CHECK_SOCKNUM();
    //CHECK_SOCKMODE(Sn_MR_UDP);
    switch ((mr=getSn_MR(sn)) & 0x0F)
    {
        case Sn_MR_UDP:
        case Sn_MR_MACRAW:
             break;
      #if ( _WIZCHIP_ < 5200 )         
        case Sn_MR_IPRAW:
        case Sn_MR_PPPoE:
             break;
      #endif
        default:
             return W5500_SOCKERR_SOCKMODE;
    }
    W5500_CHECK_SOCKDATA();
    if (sock_remained_size[sn] == 0)
    {
        while(1)
        {
            pack_len = getSn_RX_RSR(sn);
            if (getSn_SR(sn) == SOCK_CLOSED)
						{
						    return W5500_SOCKERR_SOCKCLOSED;
						}							
            if ((sock_io_mode & (1<<sn)) && (pack_len == 0))
						{
						    return W5500_SOCK_BUSY;
						}							
            if (pack_len != 0) 
						{
						    break;
						}
        };
    }
    sock_pack_info[sn] = W5500_PACK_COMPLETED;
	  switch (mr & 0x07)
	  {
	      case Sn_MR_UDP :
	          if (sock_remained_size[sn] == 0)
	          {
   			        wiz_recv_data(sn, head, 8);
   			        setSn_CR(sn,Sn_CR_RECV);
   			        while (getSn_CR(sn));
   			        // read peer's IP address, port number & packet length
    			      addr[0] = head[0];
   			        addr[1] = head[1];
   			        addr[2] = head[2];
   			        addr[3] = head[3];
   			        *port = head[4];
   			        *port = (*port << 8) + head[5];
   			        sock_remained_size[sn] = head[6];
   			        sock_remained_size[sn] = (sock_remained_size[sn] << 8) + head[7];
   			        sock_pack_info[sn] = W5500_PACK_FIRST;
   	        }
			      if (len < sock_remained_size[sn])
				    {
				        pack_len = len;
				    }					
			      else 
				    {
				        pack_len = sock_remained_size[sn];
				    }
			      // Need to packet length check (default 1472)
   		      wiz_recv_data(sn, buf, pack_len); // data copy.
			      break;
	      case Sn_MR_MACRAW :
	          if (sock_remained_size[sn] == 0)
	          {
   			        wiz_recv_data(sn, head, 2);
   			        setSn_CR(sn,Sn_CR_RECV);
   			        while (getSn_CR(sn));
   			        // read peer's IP address, port number & packet length
    			      sock_remained_size[sn] = head[0];
   			        sock_remained_size[sn] = (sock_remained_size[sn] <<8) + head[1];
   			        if (sock_remained_size[sn] > 1514) 
   			        {
   			            w5500_close(sn);
   			            return W5500_SOCKFATAL_PACKLEN;
   			        }
   			        sock_pack_info[sn] = W5500_PACK_FIRST;
   	        }
			      if (len < sock_remained_size[sn])
						{
							  pack_len = len;
						}						
			      else 
						{
						    pack_len = sock_remained_size[sn];
						}
			      wiz_recv_data(sn,buf,pack_len);
		        break;
      #if ( _WIZCHIP_ < 5200 )
		    case Sn_MR_IPRAW:
		        if (sock_remained_size[sn] == 0)
		        {
		            wiz_recv_data(sn, head, 6);
   			        setSn_CR(sn,Sn_CR_RECV);
   			        while(getSn_CR(sn));
   			        addr[0] = head[0];
   			        addr[1] = head[1];
   			        addr[2] = head[2];
   			        addr[3] = head[3];
   			        sock_remained_size[sn] = head[4];
   			        sock_remaiend_size[sn] = (sock_remained_size[sn] << 8) + head[5];
   			        sock_pack_info[sn] = PACK_FIRST;
            }
			      // Need to packet length check
			      if (len < sock_remained_size[sn])
						{
								pack_len = len;
						}									
			      else 
						{
								pack_len = sock_remained_size[sn];
						}
   		      wiz_recv_data(sn, buf, pack_len); // data copy.
			      break;
      #endif
        default:
            wiz_recv_ignore(sn, pack_len); // data copy.
            sock_remained_size[sn] = pack_len;
            break;
    }
	  setSn_CR(sn,Sn_CR_RECV);
	  /* wait to process the command... */
	  while (getSn_CR(sn)) ;
	  sock_remained_size[sn] -= pack_len;
	  //M20140501 : replace 0x01 with PACK_REMAINED
	  //if (sock_remained_size[sn] != 0) sock_pack_info[sn] |= 0x01;
	  if (sock_remained_size[sn] != 0) 
		{
		    sock_pack_info[sn] |= W5500_PACK_REMAINED;
		}

   	return pack_len;
}


int8_t  w5500_ctlsocket(uint8_t sn, ctlsock_type cstype, void* arg)
{
    uint8_t tmp = 0;
    W5500_CHECK_SOCKNUM();
    switch (cstype)
    {
        case W5500_CS_SET_IOMODE:
            tmp = *((uint8_t*)arg);
            if (tmp == W5500_SOCK_IO_NONBLOCK) 
						{
						    sock_io_mode |= (1<<sn);
						}							
            else if(tmp == W5500_SOCK_IO_BLOCK)
						{
						    sock_io_mode &= ~(1<<sn);
						}
            else 
						{
						    return W5500_SOCKERR_ARG;
						}
            break;
        case W5500_CS_GET_IOMODE:   
            //M20140501 : implict type casting -> explict type casting
            //*((uint8_t*)arg) = (sock_io_mode >> sn) & 0x0001;
            *((uint8_t*)arg) = (uint8_t)((sock_io_mode >> sn) & 0x0001);

            break;
        case W5500_CS_GET_MAXTXBUF:
            *((uint16_t*)arg) = getSn_TxMAX(sn);
            break;
        case W5500_CS_GET_MAXRXBUF:    
            *((uint16_t*)arg) = getSn_RxMAX(sn);
            break;
        case W5500_CS_CLR_INTERRUPT:
            if ((*(uint8_t*)arg) > W5500_SIK_ALL) 
						{
						    return W5500_SOCKERR_ARG;
						}
            setSn_IR(sn,*(uint8_t*)arg);
            break;
        case W5500_CS_GET_INTERRUPT:
            *((uint8_t*)arg) = getSn_IR(sn);
            break;
        case W5500_CS_SET_INTMASK:  
            if ((*(uint8_t*)arg) > W5500_SIK_ALL)
						{
						    return W5500_SOCKERR_ARG;
						}							
            setSn_IMR(sn,*(uint8_t*)arg);
            break;
        case W5500_CS_GET_INTMASK:   
            *((uint8_t*)arg) = getSn_IMR(sn);
        default:
            return W5500_SOCKERR_ARG;
    }
    return W5500_SOCK_OK;
}

int8_t w5500_setsockopt(uint8_t sn, sockopt_type sotype, void* arg)
{
    // M20131220 : Remove warning
    //uint8_t tmp;
    W5500_CHECK_SOCKNUM();
    switch (sotype)
    {
         case W5500_SO_TTL:
             setSn_TTL(sn,*(uint8_t*)arg);
             break;
         case W5500_SO_TOS:
             setSn_TOS(sn,*(uint8_t*)arg);
             break;
         case W5500_SO_MSS:
             setSn_MSSR(sn,*(uint16_t*)arg);
             break;
         case W5500_SO_DESTIP:
             setSn_DIPR(sn, (uint8_t*)arg);
             break;
         case W5500_SO_DESTPORT:
             setSn_DPORT(sn, *(uint16_t*)arg);
             break;
       #if _WIZCHIP_ != 5100
         case W5500_SO_KEEPALIVESEND:
             W5500_CHECK_SOCKMODE(Sn_MR_TCP);
           #if _WIZCHIP_ > 5200
             if (getSn_KPALVTR(sn) != 0) 
						 {
						     return W5500_SOCKERR_SOCKOPT;
						 }
           #endif
             setSn_CR(sn,Sn_CR_SEND_KEEP);
             while (getSn_CR(sn) != 0)
             {
                  // M20131220
         		      //if ((tmp = getSn_IR(sn)) & Sn_IR_TIMEOUT)
                  if (getSn_IR(sn) & Sn_IR_TIMEOUT)
         		      {
         			        setSn_IR(sn, Sn_IR_TIMEOUT);
                      return W5500_SOCKERR_TIMEOUT;
         		      }
             }
             break;
       #if _WIZCHIP_ > 5200
         case W5500_SO_KEEPALIVEAUTO:
             W5500_CHECK_SOCKMODE(Sn_MR_TCP);
             setSn_KPALVTR(sn,*(uint8_t*)arg);
         break;
       #endif      
     #endif   
         default:
             return W5500_SOCKERR_ARG;
    }   
    return W5500_SOCK_OK;
}

int8_t  w5500_getsockopt(uint8_t sn, sockopt_type sotype, void* arg)
{
    W5500_CHECK_SOCKNUM();
    switch(sotype)
    {
         case W5500_SO_FLAG:
             *(uint8_t*)arg = getSn_MR(sn) & 0xF0;
             break;
         case W5500_SO_TTL:
             *(uint8_t*) arg = getSn_TTL(sn);
             break;
         case W5500_SO_TOS:
             *(uint8_t*) arg = getSn_TOS(sn);
             break;
         case W5500_SO_MSS:   
             *(uint8_t*) arg = getSn_MSSR(sn);
         case W5500_SO_DESTIP:
             getSn_DIPR(sn, (uint8_t*)arg);
             break;
         case W5500_SO_DESTPORT:  
             *(uint16_t*) arg = getSn_DPORT(sn);
             break;
       #if _WIZCHIP_ > 5200   
         case W5500_SO_KEEPALIVEAUTO:
            W5500_CHECK_SOCKMODE(Sn_MR_TCP);
            *(uint16_t*) arg = getSn_KPALVTR(sn);
            break;
       #endif      
         case W5500_SO_SENDBUF:
            *(uint16_t*) arg = getSn_TX_FSR(sn);
         case W5500_SO_RECVBUF:
            *(uint16_t*) arg = getSn_RX_RSR(sn);
         case W5500_SO_STATUS:
            *(uint8_t*) arg = getSn_SR(sn);
            break;
         case W5500_SO_REMAINSIZE:
             if (getSn_MR(sn) == Sn_MR_TCP)
						 {
						     *(uint16_t*)arg = getSn_RX_RSR(sn);
						 }              
             else
						 {
						     *(uint16_t*)arg = sock_remained_size[sn];
						 }               
             break;
         case W5500_SO_PACKINFO:
             W5500_CHECK_SOCKMODE(Sn_MR_TCP);
             *(uint8_t*)arg = sock_pack_info[sn];
         break;
             default:
         return W5500_SOCKERR_SOCKOPT;
    }
    return W5500_SOCK_OK;
}
