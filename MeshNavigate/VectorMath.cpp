/*
**�ļ����ƣ�VectorMath.cpp
**����������
**�ļ�˵����
**��    �ߣ�����
**����ʱ�䣺2012-02-21
**��    �ģ�
*/

#include "Math.h"
#include "BaseStruct.h"

void VectorCrossProduct(Vector* result, const Vector* a, const Vector* b)
{
	result->x = a->y * b->z - a->z * b->y;
	result->y = a->z * b->x - a->x * b->z;
	result->z = a->x * b->y - a->y * b->x;
}

float VectorDotProduct(const Vector* a, const Vector* b)
{
	float result = a->x * b->x + a->y * b->y + a->z * b->z;
	return result;
}

float GetTValue( const Point* beginPoint, const Point* endPoint, const Point* middlePoint )
{
	float t = 0.0f;
	float deltX = endPoint->x - beginPoint->x;
	if(abs(deltX) < EPSILON)
	{
		t = (middlePoint->z - beginPoint->z) / (endPoint->z - beginPoint->z);
	}
	else
	{
		t = (middlePoint->x - beginPoint->x) / (endPoint->x - beginPoint->x);
	}
	return t;
}
