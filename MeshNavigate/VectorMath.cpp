/*
**�ļ����ƣ�VectorMath.cpp
**����������
**�ļ�˵����
**��    �ߣ�����
**����ʱ�䣺2012-02-21
**��    �ģ�
*/

#include <math.h>
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

extern bool IntersectionPointBettwenTwoSegments( const Point* beginSegmentA, const Point* endSegmentA, 
												const Point* beginSegmentB, const Point* endSegementB, 
												Point* intersectionPoint )
{
	//����н��㣬���󽻵㲢�����������ȷ��λ��
	//���ཻ�߶��󽻵�
	//�Ӷ��������p������ �����㷨���Բο�ͼ��ѧ�㷨
	Vector segementA =  {endSegmentA->x - beginSegmentA->x, 0.0f, endSegmentA->z - beginSegmentA->z};
	Vector normalLineofSegementA;
	VectorCrossProduct(&normalLineofSegementA, &segementA, &StandardVector);

	Vector aBegin2BBegin = {beginSegmentB->x - beginSegmentA->x, 0.0f, beginSegmentB->z - beginSegmentA->z};
	Vector aBegin2BEnd = {endSegementB->x - beginSegmentA->x, 0.0f, endSegementB->z - beginSegmentA->z};
	float cosof2BeginPointB = VectorDotProduct(&aBegin2BBegin, &normalLineofSegementA);					
	float cosof2EndPointB = VectorDotProduct(&aBegin2BEnd, &normalLineofSegementA);	

	double tValue =cosof2BeginPointB;
	tValue /= (cosof2BeginPointB - cosof2EndPointB);
	if (tValue < 0.0 || tValue > 1.0)
	{
		return false;
	}

	float insertionX = (float)(beginSegmentB->x + tValue * (endSegementB->x - beginSegmentB->x));
	float insertionZ = (float)(beginSegmentB->z + tValue * (endSegementB->z - beginSegmentB->z));

	//�����Ƿ����߶ε��ӳ�����
	Rect checkRect;
	const Point point = {insertionX, insertionZ};
	MakeRectByPoint(&checkRect, beginSegmentA, endSegmentA);
	if (true != InRect(&checkRect, &point))
	{
		return false;
	}
	if(NULL != intersectionPoint)
	{
		*intersectionPoint = point;
	}
	return true;
}
//���� * Point
//�������(�������ߵ�)
Matrix3x3* MakeTransferMatrix( Matrix3x3* out, const Point* fromPoint, const Point* toPoint )
{
	out->f13 = toPoint->x - fromPoint->x;
	out->f23 = toPoint->z - fromPoint->z;
	out->f33 = 1.0f;
	return out;
}

Matrix3x3* MakeRotateMatrix( Matrix3x3* out, float angle, bool countClockWise)
{
	angle = countClockWise ? angle : -angle;
	out->f11 = cos(angle);
	out->f12 = -sin(angle);
	out->f21 = -out->f12;
	out->f22 = out->f11;
	return out;
}

Point* MritrixPlusPoint(const Matrix3x3* matrix, Point* point)
{
	float x = point->x;
	float z = point->z;
	point->x = x * matrix->f11 + z * matrix->f12 + matrix->f13;
	point->z = x * matrix->f21 + z * matrix->f22 + matrix->f23;
	return point;
}

bool FloatEqual( float a, float b )
{
	bool result = abs(a - b) < EPSILON;
	return result;
}
