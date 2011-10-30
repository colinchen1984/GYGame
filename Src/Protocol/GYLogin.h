/////////////////////////////////////////////
// create time:2011/10/30 21:44
// author:	colin chen
// file name:	GYLogin
// file type:	h
////////////////////////////////////////////
#ifndef __GYLOGIN_H__
#define __GYLOGIN_H__
#include "GYCommonDefine.h"
#include "GYProtocolDefine.h"
#include "GYString.h"



class GYSerializationInteface;
//登录协议
class GYLogin : public GYPacketInteface
{
		GYGUID			UserID;			//用户GUID
	GYString			Name;			//用户姓名
	GYINT32			SceneID;			//场景ID
	
public:
	GYLogin(GYStringManager& stringManager):Name(stringManager)
	{
		
	}
	~GYLogin(){};

	virtual GYPACKETID GetPacketID(){return EM_PACKET_ID_CS_LOGIN;}
	virtual GYCHAR GetPacketFlags(){return 0;}

	GYINLINE	GYVOID			SetUserID(GYGUID value){ UserID = value; };
	GYINLINE	GYGUID			GetUserID() const { return UserID; };
	GYINLINE	GYVOID			SetName(const GYString& value){ Name = value; };
	GYINLINE	const GYString&			GetName() const { return Name; };
	GYINLINE	GYVOID			SetSceneID(GYINT32 value){ SceneID = value; };
	GYINLINE	GYINT32			GetSceneID() const { return SceneID; };


	virtual GYVOID Serializ(GYSerializationInteface& serializer);

	virtual GYVOID CleanUp();

};
#endif
