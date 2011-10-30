/////////////////////////////////////////////
// create time: 2011/10/26 18:16
// author:	colin chen
// file name:	GYGameObject
// file type:	h
////////////////////////////////////////////

#ifndef __GYGAMEOBJECT_H__
#define __GYGAMEOBJECT_H__
#include "GYObject.h"
#include "GYObjectCommenDefine.h"

class GYGameObject : public GYObject
{
protected:
	GYGameObject(){};
	virtual ~GYGameObject(){};
public:
	virtual const GYGUID&	GetGUID() const = 0;
	virtual EM_GAME_OBJECT_TYPE GetObjectType() const = 0;
	virtual GYINT32 Init() = 0;
};

#endif

