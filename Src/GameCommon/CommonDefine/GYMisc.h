/////////////////////////////////////////////
// create time: 2011/10/27 23:33
// author:	colin chen
// file name:	GYMisc
// file type:	h
////////////////////////////////////////////

#ifndef __GYMISC_H__
#define __GYMISC_H__

#include "GYCommonDefine.h"

template<typename T>
GYINLINE T& Min(T& a, T& b)
{
	if (a < b)
	{
		return a;
	}

	return b;
}

template<typename T>
GYINLINE T& Min(T& a, T& b, T& c)
{
	return Min(a, Min(b, c));
}

template<typename T>
GYINLINE T& Max(T& a, T& b)
{
	if (a > b)
	{
		return a;
	}

	return b;
}

template<typename T>
GYINLINE T& Max(T& a, T& b, T& c)
{
	return Max(a, Max(b, c));
}
#endif
