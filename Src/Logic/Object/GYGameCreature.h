﻿/////////////////////////////////////////////
// create time: 2011/10/30 16:55
// author:	colin chen
// file name:	GYGameCreature
// file type:	h
////////////////////////////////////////////

#ifndef __GYGAMECREATURE_H__
#define __GYGAMECREATURE_H__
#include "GYGameObject.h"
#include "GYCommonDefine.h"

//提供移动,战斗,存放物品,使用物品的相关接口

class GYScene;
class GYPacketInteface;
struct GYPosition;

class GYCreature : public GYGameObject
{
protected:
	GYCreature(){};

	virtual ~GYCreature(){};

public:
	virtual GYVOID OnEnterScene(GYScene& scene);

	virtual GYVOID OnLeaveScene(GYScene& scene);

	GYVOID SetPosition(const GYPosition& position);

	virtual const GYPosition& GetPosition() const  = 0;

	virtual GYINT32 GetCurrentSceneID() const  = 0;

	virtual GYVOID Tick(GYUINT32 frameTime) = 0;

	GYINLINE virtual GYVOID SendPacket(const GYPacketInteface& packet) {};

private:
	virtual GYVOID _SetPosition(const GYPosition& position) = 0;

};
#endif

