/////////////////////////////////////////////
// create time: 2011/11/5 21:03
// author:	colin chen
// file name:	GYGameCreature
// file type:	cpp
////////////////////////////////////////////

#include "GYGameCreature.h"
#include "GYPosition.h"
#include "GYScene.h"
#include "GYServer.h"

GYVOID GYCreature::SetPosition( const GYPosition& position )
{
	GYPosition currentPosition = GetPosition();
	_SetPosition(position);
	GYScene* pScene = GETLOGICSERVER.GetSceneByID(GetCurrentSceneID());
	GYAssert(GYNULL != pScene);
	if (GYNULL != pScene)
	{
		pScene->OnCreatureChangePosition(*this, currentPosition, position);
	}
}

GYVOID GYCreature::OnEnterScene( GYScene& scene )
{

}

GYVOID GYCreature::OnLeaveScene( GYScene& scene )
{

}

