/////////////////////////////////////////////
// create time: 2011/10/30 16:56
// author:	colin chen
// file name:	GYGameHuman
// file type:	h
////////////////////////////////////////////

#ifndef __GYGAMEHUMAN_H__
#define __GYGAMEHUMAN_H__
#include "GYCommonDefine.h"
#include "GYGameCreature.h"
#include "GYPosition.h"

class GYGatewaySession;

class GYHuman : public GYCreature
{
	//只允许Human pool创建human object
	template<typename HashKeyType, typename T>	friend class GYObjectPool;

private:
	//TODO: object 的数据存放
	//暂时采用下面的方式存放数据
	GYGUID		m_guid;
	GYPosition	m_position;
	GYINT32		m_currentSceneID;
	GYGatewaySession* m_pSession;
public:
	virtual	~GYHuman();

	GYINLINE virtual const GYGUID&	GetGUID() const { return m_guid;};

	GYINLINE virtual GYVOID	SetGUID(const GYGUID& guid){ m_guid = guid; };

	GYINLINE virtual EM_GAME_OBJECT_TYPE GetObjectType() const{return EM_GAME_OBJECT_TYPE_HUMAN;};

	GYINT32 Init(GYGatewaySession& gatewaySession);

	GYINLINE virtual const GYPosition& GetPosition() const { return m_position;}

	virtual GYVOID Tick(GYUINT32 frameTime);

	virtual GYVOID OnEnterScene(GYScene& scene);

	virtual GYVOID OnLeaveScene(GYScene& scene);

	GYINLINE virtual GYINT32 GetCurrentSceneID() const {return m_currentSceneID;};

	virtual GYVOID SendPacket(const GYPacketInteface& packet);

private:
	GYINLINE virtual GYVOID _SetPosition(const GYPosition& position){ m_position = position;}

protected:
	GYHuman();

};

#endif

