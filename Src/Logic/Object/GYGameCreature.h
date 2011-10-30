/////////////////////////////////////////////
// create time: 2011/10/30 16:55
// author:	colin chen
// file name:	GYGameCreature
// file type:	h
////////////////////////////////////////////

#ifndef __GYGAMECREATURE_H__
#define __GYGAMECREATURE_H__
#include "GYGameObject.h"

class GYCreature : public GYGameObject
{
protected:
	GYCreature(){};
	virtual ~GYCreature(){};

public:
	virtual GYVOID OnEnterScene(){};

	virtual GYVOID OnLeaveScene(){};
};
#endif

