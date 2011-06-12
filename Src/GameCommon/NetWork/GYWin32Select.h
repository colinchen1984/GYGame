/////////////////////////////////////////////
// create time: 2011/6/12 16:19
// author:	colin chen
// file name:	GYWin32Select
// file type:	h
////////////////////////////////////////////

#ifndef __GYWIN32SELECT_H__
#define __GYWIN32SELECT_H__
#include "GYCommonDefine.h"
#include "GYNetWorkCommonDefine.h"
#include "GYEvent.h"

class GYReactor;
class GYWin32SelectReactor
{
	friend class GYReactor;
	fd_set		m_workingfdSet[GYNetEventTypeCount];
	fd_set		m_masterfdSet[GYNetEventTypeCount];
	GYReactor*	m_reactor;
private:
	GYWin32SelectReactor();
	~GYWin32SelectReactor();

	GYINT32 _Init(GYINT32 maxEventCount, GYReactor* reactor);

	GYINT32 _Release();

	GYINT32 _AddEvent(GYNetEvent& event);

	GYINT32 _DeleteEvent(GYNetEvent& event);

	GYINT32	_RunOnce();

private:
	GYVOID _CleanUp();
};

#endif