/*
**�ļ����ƣ�VectorMath.h
**����������
**�ļ�˵����
**��    �ߣ�����
**����ʱ�䣺2012-02-21
**��    �ģ�
*/

#pragma once

struct Vector;
struct Matrix3x3;

extern void VectorCrossProduct(Vector* result, const Vector* a, const Vector* b);

extern float VectorDotProduct(const Vector* a, const Vector* b);

extern bool IntersectionPointBettwenTwoSegments( const Point* beginSegmentA, const Point* endSegmentA, 
												const Point* beginSegmentB, const Point* endSegementB, 
												Point* intersectionPoint );

extern Matrix3x3* MakeTransferMatrix(Matrix3x3* out, const Point* fromPoint, const Point* toPoint);

//����һ����ת����
//angleΪ����ֵ
extern Matrix3x3* MakeRotateMatrix(Matrix3x3* out, float angle, bool countClockWise);

extern Point* MritrixPlusPoint(const Matrix3x3* matrix, Point* point);

extern bool FloatEqual(float a, float b);

#define FloatEqualZero(a) FloatEqual(a, 0.0f)