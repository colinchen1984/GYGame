/////////////////////////////////////////////
// create time: 2011/10/30 21:44
// author:	colin chen
// file name:	GYLogout
// file type:	cpp
////////////////////////////////////////////
#include "GYLogout.h"
#include "GYProtocolDefine.h"
#include "GYSerialization.h"

GYVOID GYLogout::Serializ( GYSerializationInteface& serializer )
{
		serializer << UserID;

}

GYVOID GYLogout::CleanUp()
{
		UserID = INVALID_VALUE;
	
}




