/*
**�ļ����ƣ�BaseStruct.h
**��������������Ѱ·�������ݽṹ
**�ļ�˵����
**��    �ߣ�����
**����ʱ�䣺2012-02-21
**��    �ģ�
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

struct Matrix3x3
{
	union
	{
		float point[3][3];
		struct
		{
			float f11, f12, f13;
			float f21, f22, f23;
			float f31, f32, f33;
		};
	};
};

extern void MakeRectByPoint(Rect* rect, const Point* a, const Point* b);

extern bool InRect(const Rect* rect, const Point* a);

extern bool IsSamePoint(const Point* a, const Point* b);

extern Vector* NormalizationVector(Vector* v);

extern Matrix3x3* IdentityMatrix(Matrix3x3* matrix);

#define NULL 0

const Vector StandardVector = {0.0f, 1.0f, 0.0f};

const float EPSILON = 0.000001f;

const float PI = 3.1415f;