

#include <stdbool.h>



#include "libiec61850_platform_includes.h"
#include "hal_socket.h"
#include "stack_config.h"



HandleSet
Handleset_new(void)
{
   HandleSet result ;
   
   return result;
}

void
Handleset_addSocket(HandleSet self, const Socket sock)
{
  
}

int
Handleset_waitReady(HandleSet self, unsigned int timeoutMs)
{
   int result;

  

   return result;
}

void
Handleset_destroy(HandleSet self)
{
  ;
}

static bool wsaStartupCalled = false;
static int socketCount = 0;

//static void
//activateKeepAlive(SOCKET s)
//{
//	
//}

static void
setSocketNonBlocking(Socket self)
{
   
}

static bool
prepareServerAddress(const char* address, int port, struct sockaddr_in* sockaddr)
{

	

    return true;
}

static bool wsaStartUp()
{
	
		return true;
}

static void wsaShutdown()
{
	
}

ServerSocket
TcpServerSocket_create(const char* address, int port)
{
	
	return NULL;
}

void
ServerSocket_listen(ServerSocket self)
{
	
}

Socket
ServerSocket_accept(ServerSocket self)
{
	
	Socket conSocket = NULL;

	

	return conSocket;
}

void
ServerSocket_setBacklog(ServerSocket self, int backlog)
{
	
}

void
ServerSocket_destroy(ServerSocket self)
{
	
}

Socket
TcpSocket_create()
{
	Socket self ;

	return self;
}

void
Socket_setConnectTimeout(Socket self, uint32_t timeoutInMs)
{
    
}

bool
Socket_connect(Socket self, const char* address, int port)
{
	
        return true;
}

char*
Socket_getPeerAddress(Socket self)
{
	

	char* clientConnection;

	return clientConnection;
}

int
Socket_read(Socket self, uint8_t* buf, int size)
{
 
    return -1;
    
}

int
Socket_write(Socket self, uint8_t* buf, int size)
{
   

	return -1;
}

void
Socket_destroy(Socket self)
{
	
}
