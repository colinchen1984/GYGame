/*
**文件名称：BaseStruct.h
**功能描述：网格寻路基础数据结构
**文件说明：
**作    者：陈琳
**创建时间：2012-02-21
**修    改：
*/

#pragma once

struct Point
{
	float x, z;
};

struct Vector
{
	float x, y, z;
};

struct Rect
{
	Point leftTop;
	float width;
	float height;
};

extern void MakeRectByPoint(Rect* rect, const Point* a, const Point* b);

extern bool InRect(const Rect* rect, const Point* a);

extern bool IsSamePoint(const Point* a, const Point* b);

extern Vector* NormalizationVector(Vector* v);

#define NULL 0

const Vector StandardVector = {0.0f, 1.0f, 0.0f};

const float EPSILON = 0.000001f;
