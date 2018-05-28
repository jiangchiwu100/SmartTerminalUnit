#ifndef _COMMUN_H
#define _COMMUN_H
#include "sys.h"

enum communState{
	IsNoUse,
	
	IsRequestDebug,
	WaitDebugBack,
	RxDebugBackOk,
	RxDebugBackOver,
	
	IsSendCurrentState,
	WaitSendStateBack,
	RxSendStateOk,
	RxSendStateOver,
	
	IsModifyState,
	WaitModfiyBack,
	RxModfiyBackOk,
	RxModfiyBackOver,
	
};
void communInit(void);
enum communState CommunicationMian(void);
#endif
