/*
**文件名称：BaseStruct.h
**功能描述：网格寻路基础数据结构
**文件说明：
**作    者：陈琳
**创建时间：2012-02-21
**修    改：
*/

#pragma once

struct Queue;
struct Point
{
	float x, z;
	void* userData;
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

extern Queue* CreateQueue(int maxCount);

extern void PushDataToQueue(Queue* queue, void* data);

extern void* PopDataFromQueue(Queue* queue);

extern void* DeleteDataFromQueue(Queue* queue, int dataIndex);

extern void CleanQueue(Queue* queue);

extern int GetDataCountFromQueue(Queue* queue);

extern void* GetDataFromQueueByIndex(Queue* queue, int index);

extern void ReleaseQueue(Queue* queue);

#define NULL 0

const Vector StandardVector = {0.0f, 1.0f, 0.0f};

const float EPSILON = 0.000001f;

const float PI = 3.1415f;