/*
**文件名称：VectorMath.h
**功能描述：
**文件说明：
**作    者：陈琳
**创建时间：2012-02-21
**修    改：
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

//生成一个旋转矩阵
//angle为弧度值
extern Matrix3x3* MakeRotateMatrix(Matrix3x3* out, float angle, bool countClockWise);

extern Point* MritrixPlusPoint(const Matrix3x3* matrix, Point* point);

extern bool FloatEqual(float a, float b);

#define FloatEqualZero(a) FloatEqual(a, 0.0f)