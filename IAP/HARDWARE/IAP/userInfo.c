
#include "userInfo.h"



void userInfoInit(void)
{
//	ReturnStatus result;
//	result = AddUserInfo(IapMaster);
//	if(result == ISTURE){
//	}
}

ReturnStatus ApplyForUser(UserInfo *user)
{
	if(user->status == IDLE){
		user->status = PRIVATE;
		return ISTURE;
	}
	return ISFAILURE;
}

UserErrorStatus GetMasterUserInfoStatus(UserInfo *user)
{
	if(user->status == SENDING || user->status == WAITACK){
		return ISWAITINGACK;
	}
	else if(user->status == ISACK){
		return REVACKOK;
	}
	else{
		user->status = IDLE;
		return ISCONNECTFAIL;
	}
}

UserErrorStatus GetSlaveUserInfoStatus(UserInfo *user)
{
	if(user->status == ISMASTERCALL){
		return HAVEMASTERCMD;
	}
	else{
		user->status = IDLE;
		return NOMASTERCMD;
	}
}	
