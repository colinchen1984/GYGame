/////////////////////////////////////////////
// create time: 2011/7/12 23:52
// author:	colin chen
// file name:	GYTimeStamp
// file type:	h
////////////////////////////////////////////

#ifndef __GYTIMESTAMP_H__
#define __GYTIMESTAMP_H__
#include "GYCommonDefine.h"
class GYTimeStamp
{
public:
	GYUINT32 m_termTime;

	GYTimeStamp(){m_termTime = 0;}
	~GYTimeStamp(){m_termTime = 0;}
};
#endif