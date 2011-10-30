/////////////////////////////////////////////
// create time: 2011/10/30 16:57
// author:	colin chen
// file name:	GYGameHuman
// file type:	cpp
////////////////////////////////////////////
#include "GYGameHuman.h"


GYHuman::GYHuman()
{

}

GYHuman::~GYHuman()
{

}

const GYGUID& GYHuman::GetGUID() const
{
	return m_guid;
}

EM_GAME_OBJECT_TYPE GYHuman::GetObjectType() const
{
	return EM_GAME_OBJECT_TYPE_HUMAN;
}

GYINT32 GYHuman::Init()
{
	return 0;
}

