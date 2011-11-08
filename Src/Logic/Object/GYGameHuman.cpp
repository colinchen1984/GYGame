/////////////////////////////////////////////
// create time: 2011/10/30 16:57
// author:	colin chen
// file name:	GYGameHuman
// file type:	cpp
////////////////////////////////////////////
#include "GYGameHuman.h"
#include "GYScene.h"

GYHuman::GYHuman()
{

}

GYHuman::~GYHuman()
{

}

GYINT32 GYHuman::Init()
{
	return 0;
}

GYVOID GYHuman::Tick( GYUINT32 frameTime )
{

}

GYVOID GYHuman::OnEnterScene( GYScene& scene )
{
	m_currentSceneID = scene.GetSceneID();
}

GYVOID GYHuman::OnLeaveScene( GYScene& scene )
{
	m_currentSceneID = INVALID_VALUE;
}

