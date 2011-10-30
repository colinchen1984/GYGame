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

class GYHuman : public GYCreature
{
	//只允许Human pool创建human object
	template<typename HashKeyType, typename T>	friend class GYObjectPool;
public:
	virtual	~GYHuman();

	virtual const GYGUID&	GetGUID() const;

	virtual EM_GAME_OBJECT_TYPE GetObjectType() const;

	virtual GYINT32 Init();

	GYINLINE virtual GYVOID SetPosition(const GYPosition& position){ m_position = position;}

	GYINLINE virtual const GYPosition& GetPosition() const { return m_position;}

protected:
	GYHuman();

private:
	//TODO: object 的数据存放
	//暂时采用下面的方式存放数据
	GYGUID		m_guid;
	GYPosition	m_position;
};

#endif

