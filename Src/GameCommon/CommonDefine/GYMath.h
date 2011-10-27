/////////////////////////////////////////////
// create time: 2011/10/27 23:07
// author:	colin chen
// file name:	GYMath
// file type:	h
////////////////////////////////////////////

#ifndef __GYMATH_H__
#define __GYMATH_H__
#include "GYCommonDefine.h"
#include <math.h>

class GYMath
{
public:
	static GYINLINE GYBOOL IsEqualFloat(const GYFLOAT a, const GYFLOAT b)
	{
		return abs(a - b) < GYFLT_EPSILON ? GYTRUE : GYFALSE;
	}

private:
	GYMath(){};
	~GYMath(){};
};

#endif
