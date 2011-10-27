/////////////////////////////////////////////
// create time: 2011/10/27 22:41
// author:	colin chen
// file name:	GYScene
// file type:	h
////////////////////////////////////////////

#ifndef __GYSCENE_H__
#define __GYSCENE_H__
#include "GYCommonDefine.h"

struct GYSceneDefine;
class GYZone;
class GYArea;

class GYScene
{
private:
	GYZone*	m_zone;			//场景内区域对象
	GYArea* m_Area;			//场景内事件区域对象

public:
	GYScene();
	~GYScene();

	GYINT32 Init(const GYSceneDefine& sceneDefine);

	GYINT32 Release();

};
#endif
