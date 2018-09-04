

#include <stdbool.h>



#include "libiec61850_platform_includes.h"
#include "hal_socket.h"
#include "stack_config.h"

#include <sys/socket.h>
//#include <dfs_select.h>


#include "lwip/opt.h"
#include "lwip/api.h"
#include "extern_interface.h"

#define INVALID_SOCKET (~0)


int  lwip_send_ex(int fd, uint8_t* buf, int size, int s)
{
    printf("Uminplent!\n");
    return 0;
}


struct sSocket {
	int fd;
	uint32_t connectTimeout;
    
    
    struct sockaddr fromAddr;   
};
struct sServerSocket {

    int fd;
    int backLog;
    
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
};

struct sHandleSet {
   fd_set handles;
   int  maxHandle;
    
};


static int socketCount = 0;


HandleSet
Handleset_new(void)
{
   
   HandleSet result = (HandleSet) GLOBAL_MALLOC(sizeof(struct sHandleSet));

   if (result != NULL) {
       FD_ZERO(&result->handles);
      result->maxHandle = INVALID_SOCKET;
   }
   return result;  
}

void
Handleset_addSocket(HandleSet self, const Socket sock)
{
   if (self != NULL && sock != NULL && sock->fd != INVALID_SOCKET) {
       FD_SET(sock->fd, &self->handles);

       if ((sock->fd > self->maxHandle) || (self->maxHandle == INVALID_SOCKET))
           self->maxHandle = sock->fd;
   }
}

int
Handleset_waitReady(HandleSet self, unsigned int timeoutMs)
{
   int result;

   if (self != NULL && self->maxHandle >= 0) {
       struct timeval timeout;

       timeout.tv_sec = timeoutMs / 1000;
       timeout.tv_usec = (timeoutMs % 1000) * 1000;
       result = lwip_select(self->maxHandle + 1, &self->handles, NULL, NULL, &timeout);
   } else {
       result = -1;
   }

   
   return result;
}

void
Handleset_destroy(HandleSet self)
{
  GLOBAL_FREEMEM(self);
}


//static void
//activateKeepAlive(SOCKET s)
//{
//	
//}

static void
setSocketNonBlocking(Socket self)
{
    unsigned long mode = 1;    
    if (lwip_ioctl(self->fd, FIONBIO, &mode) != 0) {
        if (DEBUG_SOCKET)
            perror("LWIP_SOCKET: failed to set socket non-blocking!\n");
    }
    
    int tcpNoDelay = 1;
     int error = lwip_setsockopt(self->fd, IPPROTO_TCP, TCP_NODELAY,(const char*)&tcpNoDelay, sizeof(int));
    if (error != 0)
    {
        if (DEBUG_SOCKET)
            perror("setsockopt: IPPROTO_TCP, TCP_NODELAY! %d\n", self->fd);
    }
    
}

static bool
prepareServerAddress(const char* address, int port, struct sockaddr_in* sockaddr)
{

	
    perror("Unimplement\n");
    return true;
}



ServerSocket
TcpServerSocket_create(const char* address, int port)
{
    ServerSocket serverSocket = (ServerSocket) GLOBAL_MALLOC(sizeof(struct sServerSocket));

    if (!serverSocket)
    {
       perror("serverSocket GLOBAL_MALLOC\n");
        return NULL; 
    }
    if ((serverSocket->fd = lwip_socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
    {
        perror("lwip_socket\n");
        return NULL;
    }
    
    int optionReuseAddr = 1;
	lwip_setsockopt(serverSocket->fd, SOL_SOCKET, SO_REUSEADDR, (char *)&optionReuseAddr, sizeof(int));
    
    serverSocket->server_addr.sin_family = AF_INET;
    serverSocket->server_addr.sin_port = htons(port);
    serverSocket->server_addr.sin_addr.s_addr = htonl(INADDR_ANY); 
    rt_memset(&(serverSocket->server_addr.sin_zero), 8, sizeof(serverSocket->server_addr.sin_zero));
    if (lwip_bind(serverSocket->fd, (struct sockaddr *) (&(serverSocket->server_addr)), sizeof(struct sockaddr))== -1)
    {
        perror("Unable to bind\n");
        GLOBAL_FREEMEM(serverSocket);
        return NULL;
    }
    
    serverSocket->backLog = 10;
    setSocketNonBlocking((Socket) serverSocket);
    
    socketCount++;
	return serverSocket;
}

void
ServerSocket_listen(ServerSocket self)
{
	if (lwip_listen(self->fd, self->backLog) == -1)
    {
        perror("Listen error\n");
    }
}

Socket
ServerSocket_accept(ServerSocket self)
{
	int fd;
    uint32_t sin_size = sizeof(struct sockaddr_in);
	Socket conSocket = NULL;
    struct sockaddr fromAddr;
    
	fd = lwip_accept(self->fd, &fromAddr, &sin_size);

	if (fd >= 0) {
		conSocket = TcpSocket_create();
		conSocket->fd = fd;

	    setSocketNonBlocking(conSocket);
               
        MEMCPY(&(conSocket->fromAddr), &fromAddr, sizeof(struct sockaddr));
        if (DEBUG_SOCKET)
        {
            rt_kprintf("Accept new!\n");
        }
	}

	return conSocket;
}

void
ServerSocket_setBacklog(ServerSocket self, int backlog)
{
	self->backLog = backlog;
}

void
ServerSocket_destroy(ServerSocket self)
{
	lwip_close(self->fd);
    socketCount--;
    GLOBAL_FREEMEM(self);
    if (DEBUG_SOCKET)
    {
        rt_kprintf("ServerSocket_destroy\n");
    }
}

Socket
TcpSocket_create()
{
	Socket self = (Socket) GLOBAL_MALLOC(sizeof(struct sSocket));

	self->fd = INVALID_SOCKET;
	self->connectTimeout = 5000;

	socketCount++;

	return self;
}
void
Socket_setConnectTimeout(Socket self, uint32_t timeoutInMs)
{
    self->connectTimeout = timeoutInMs;
}

bool
Socket_connect(Socket self, const char* address, int port)
{
	
     perror("Unimplement\n");
     return false;
}

char*
Socket_getPeerAddress(Socket self)
{
    //Р§зг--- "192.168.10.1:12345"
	char* clientConnection = (char*)GLOBAL_MALLOC(sizeof(char) *  25);
    char* pData = self->fromAddr.sa_data;
    sprintf(clientConnection, "%d.%d.%d.%d:%d", pData[2], pData[3], pData[4], pData[5], pData[0]*256+ pData[1]);  
    //rt_kprintf(clientConnection);
    //rt_kprintf("\r\n");
	return clientConnection;
}

int
Socket_read(Socket self, uint8_t* buf, int size)
{
    if (DEBUG_SOCKET)
    {
        rt_kprintf("Recive:\n");
    }
    int bytes_received = lwip_recv(self->fd, buf, size, 0);
    
    if (bytes_received <=0 )
    {
        bytes_received = 0;
        //perror("bytes_received <=0\n");
    }
//    if (DEBUG_SOCKET)
//    {
//        for (int i = 0; i< bytes_received; i++)
//        {
//            rt_kprintf("%X ", buf[i]);
//        }
//    }
    return bytes_received;
    
}

int
Socket_write(Socket self, uint8_t* buf, int size)
{

    if (DEBUG_SOCKET)
    {
        rt_kprintf("Send:\n");
    }
    int bytes_sent = lwip_send_ex(self->fd, buf, size, 0);
    if (bytes_sent < 0)
    {
        if (bytes_sent == ERR_WOULDBLOCK)
        {
            bytes_sent = 0;
        }
        perror("lwip_send <0, size: %d\n", size);
    }
    
    if (DEBUG_SOCKET)
    {
        rt_kprintf("send size: %d\n", size);
//        for (int i = 0; i< bytes_sent; i++)
//        {
//            rt_kprintf("%X ", buf[i]);
//        }
    }
	return bytes_sent;
}

void
Socket_destroy(Socket self)
{
	 if (self->fd != INVALID_SOCKET)
     {
         lwip_close(self->fd);
     }
     socketCount--;
     GLOBAL_FREEMEM(self);
}


