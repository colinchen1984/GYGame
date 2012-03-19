/*
**�ļ����ƣ�BaseStruct.h
**��������������Ѱ·�������ݽṹ
**�ļ�˵����
**��    �ߣ�����
**����ʱ�䣺2012-02-21
**��    �ģ�
*/

#include "BaseStruct.h"
#include <math.h>

void MakeRectByPoint(Rect* rect, const Point* a, const Point* b)
{
	rect->leftTop.x = a->x < b->x ? a->x : b->x;
	rect->leftTop.z = a->z < b->z ? a->z : b->z;
	
	rect->width = abs(a->x - b->x);
	rect->height = abs(a->z - b->z);
}

inline bool InRect(const Rect* rect, const Point* a)
{
	bool xreslut = rect->leftTop.x <= a->x && rect->leftTop.x + rect->width >= a->x;
	bool zreslut = rect->leftTop.z <= a->z && rect->leftTop.z + rect->height >= a->z;
	return xreslut && zreslut;
}

bool IsSamePoint( const Point* a, const Point* b )
{
	double xSquar = (a->x - b->x);
	xSquar *= xSquar;
	double zSquar = (a->z - b->z);
	zSquar *= zSquar;
	const static double SQUAR_EPSILON = EPSILON * EPSILON;
	bool result = xSquar + zSquar < SQUAR_EPSILON;
	return result;
}

extern Vector* NormalizationVector( Vector* v )
{
	float t = v->x * v->x + v->y * v->y + v->z * v->z;
	t = sqrt(t);
	v->x /= t;
	v->y /= t;
	v->z /= t;
	return v;
}

