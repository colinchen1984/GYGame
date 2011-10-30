/////////////////////////////////////////////
// create time: 2011/10/30 21:44
// author:	colin chen
// file name:	GYLogin
// file type:	cpp
////////////////////////////////////////////
#include "GYLogin.h"
#include "GYProtocolDefine.h"
#include "GYSerialization.h"

GYVOID GYLogin::Serializ( GYSerializationInteface& serializer )
{
		serializer << UserID;
	serializer << Name;
	serializer << SceneID;

}

GYVOID GYLogin::CleanUp()
{
		UserID = INVALID_VALUE;
	Name.CleanUp();
	SceneID = INVALID_VALUE;
	
}




