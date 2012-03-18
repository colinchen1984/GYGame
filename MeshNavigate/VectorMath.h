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

extern void VectorCrossProduct(Vector* result, const Vector* a, const Vector* b);

extern float VectorDotProduct(const Vector* a, const Vector* b);

extern float GetTValue(const Point* beginPoint, const Point* endPoint, const Point* middlePoint);
