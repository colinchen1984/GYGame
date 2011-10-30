/////////////////////////////////////////////
// create time:2011/10/30 21:44
// author:	colin chen
// file name:	GYLogout
// file type:	h
////////////////////////////////////////////
#ifndef __GYLOGOUT_H__
#define __GYLOGOUT_H__
#include "GYCommonDefine.h"
#include "GYProtocolDefine.h"



class GYSerializationInteface;
//登出协议
class GYLogout : public GYPacketInteface
{
		GYGUID			UserID;			//用户GUID
public:
	GYLogout()
	{
		
	}
	~GYLogout(){};

	virtual GYPACKETID GetPacketID(){return EM_PACKET_ID_CS_LOGOUT;}
	virtual GYCHAR GetPacketFlags(){return 0;}

	GYINLINE	GYVOID			SetUserID(GYGUID value){ UserID = value; };
	GYINLINE	GYGUID			GetUserID() const { return UserID; };


	virtual GYVOID Serializ(GYSerializationInteface& serializer);

	virtual GYVOID CleanUp();

};
#endif
