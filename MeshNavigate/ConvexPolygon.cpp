/*
**文件名称：ConvexPolygon.cpp
**功能描述：凸多边形
**文件说明：
**作    者：陈琳
**创建时间：2012-02-21
**修    改：
*/
#include "BaseStruct.h"
#include "VectorMath.h"
#include "ConvexPolygon.h"
#include <string.h>
#include <malloc.h>
#include <math.h>

struct ConvexPolygon
{
	//要求顶点顺时针, 所有边的法线朝内
	Point* 			pointList;
	Vector* 		normalVectorList;
	int 			vertexCount;
	int				maxVertexCount;
};



ConvexPolygon* CreateConvexPolygon(int maxVertexCount)
{
	if(maxVertexCount < 3)
	{
		return NULL;
	}
	
	ConvexPolygon* p = (ConvexPolygon*)malloc(sizeof(ConvexPolygon));
	memset(p, 0, sizeof(ConvexPolygon));
	
	p->pointList = (Point*)malloc(sizeof(Point) * maxVertexCount);
	memset(p->pointList, 0, sizeof(p->pointList[0]) * maxVertexCount);
	p->normalVectorList = (Vector*)malloc(sizeof(Vector)* maxVertexCount);
	memset(p->normalVectorList, 0, sizeof(p->normalVectorList[0]) * maxVertexCount);
	p->maxVertexCount = maxVertexCount;
	return p;
}

void ReleaseConvexPolygon(ConvexPolygon* polygon)
{
	if(NULL == polygon)
	{
		return;
	}
	free(polygon->pointList);
	free(polygon->normalVectorList);
	free(polygon);
}

void CleanConvexPolygon(ConvexPolygon* polygon)
{
	if(NULL == polygon)
	{
		return;
	}
	memset(polygon->normalVectorList, 0, sizeof(polygon->normalVectorList[0]) * polygon->maxVertexCount);
	memset(polygon->pointList, 0, sizeof(polygon->pointList[0]) * polygon->maxVertexCount);
	polygon->vertexCount = 0;
}

static bool CopyConvexPolygon( ConvexPolygon* dest, const ConvexPolygon* src)
{
	if (dest->maxVertexCount < src->maxVertexCount)
	{
		return false;
	}
	memcpy(dest->pointList, src->pointList, sizeof(src->pointList[0]) * src->vertexCount);
	memcpy(dest->normalVectorList, src->normalVectorList, sizeof(src->pointList[0]) * src->vertexCount);
	dest->vertexCount = src->vertexCount;
}

ConvexPolygon* MakeConvexPolygonCopy( ConvexPolygon* polygon )
{
	if(NULL == polygon)
	{
		return NULL;
	}
	ConvexPolygon* p = (ConvexPolygon*)malloc(sizeof(ConvexPolygon));
	memcpy(p, polygon, sizeof(ConvexPolygon));

	p->pointList = (Point*)malloc(sizeof(Point) * polygon->maxVertexCount);
	memcpy(p->pointList, polygon->pointList, sizeof(p->pointList[0]) * polygon->maxVertexCount);
	p->normalVectorList = (Vector*)malloc(sizeof(Vector)* polygon->maxVertexCount);
	memcpy(p->normalVectorList, polygon->normalVectorList, sizeof(p->normalVectorList[0]) * polygon->maxVertexCount);
	return p;
}

bool PointInPolygon(const ConvexPolygon* polygon, float x, float z)
{
	if(NULL == polygon)
	{
		return false;
	}
	
	const Point point = {x, z};
	if(polygon->vertexCount < 3)
	{
		return false;
	}
	
	Vector v;
	v. y = 0;
	bool result = true;
	for(int i = 0; i < polygon->vertexCount; ++i)
	{
		v.x = point.x - polygon->pointList[i].x;
		v.z = point.z - polygon->pointList[i].z;
		float dot = VectorDotProduct(&v, &polygon->normalVectorList[i]);
		if(dot < 0)
		{
			result = false;
			break;
		}
	}
	return result;
}

static void inline PushBackPointToPolygon(ConvexPolygon* polygon, const Point* point)
{
	Vector v;
	int targetIndex = polygon->vertexCount;
	if(polygon->vertexCount >= 1)
	{
		const Point* lastPoint = &polygon->pointList[targetIndex - 1];
		v.y = 0;
		v.x = point->x - lastPoint->x;
		v.z = point->z - lastPoint->z;
		VectorCrossProduct(&polygon->normalVectorList[targetIndex - 1], &v, &StandardVector);
	}
	
	if(polygon->vertexCount >= 2)
	{
		v.y = 0;
		v.x = polygon->pointList[0].x - point->x;
		v.z = polygon->pointList[0].z - point->z;		
		VectorCrossProduct(&polygon->normalVectorList[targetIndex], &v, &StandardVector);
	}
	polygon->pointList[targetIndex] = *point;
	++polygon->vertexCount;
		
}

static inline void InsertPointToPolygon(ConvexPolygon* polygon, int targetPosition, const Point* point)
{
	memmove(&polygon->pointList[targetPosition + 2], &polygon->pointList[targetPosition + 1], sizeof(polygon->pointList[0]) * (polygon->vertexCount - targetPosition - 1));
	memmove(&polygon->normalVectorList[targetPosition + 2], &polygon->normalVectorList[targetPosition + 1], sizeof(polygon->normalVectorList[0]) * (polygon->vertexCount - targetPosition - 1));
	polygon->pointList[targetPosition + 1] = *point;
	++polygon->vertexCount;
	Vector v = {point->x - polygon->pointList[targetPosition].x, 0.0f, point->z - polygon->pointList[targetPosition].z};
	VectorCrossProduct(&polygon->normalVectorList[targetPosition], &v, &StandardVector);
	v.x = polygon->pointList[targetPosition + 2].x - point->x;
	v.z = polygon->pointList[targetPosition + 2].z - point->z;
	VectorCrossProduct(&polygon->normalVectorList[targetPosition + 1], &v, &StandardVector);
	if (targetPosition + 2 == polygon->vertexCount - 1)
	{
		v.x =polygon->pointList[0].x - polygon->pointList[targetPosition + 2].x;
		v.z =polygon->pointList[0].z - polygon->pointList[targetPosition + 2].z;
		VectorCrossProduct(&polygon->normalVectorList[targetPosition + 2], &v, &StandardVector);
	}
}

static bool IsRightInsertPosition(ConvexPolygon* polygon, int targetPosition, const Point* point)
{
	//组成Rect, 检测是否在rect内
	int beginIndex = targetPosition;
	int endIndex = targetPosition + 1 == polygon->vertexCount ? 0 : targetPosition + 1;
	if(abs(point->x - polygon->pointList[beginIndex].x) + abs(point->x - polygon->pointList[endIndex].x) > abs(polygon->pointList[beginIndex].x - polygon->pointList[endIndex].x))
	{
		return false;
	}

	if(abs(point->z - polygon->pointList[beginIndex].z) + abs(point->z - polygon->pointList[endIndex].z) > abs(polygon->pointList[beginIndex].z - polygon->pointList[endIndex].z))
	{
		return false;
	}
	Vector fromTargetToPoint = {point->x - polygon->pointList[beginIndex].x, 0.0f, point->z - polygon->pointList[beginIndex].z};
	Vector normalVectorForfromTargetToPointLine;
	VectorCrossProduct(&normalVectorForfromTargetToPointLine, &fromTargetToPoint, &StandardVector);
	bool result = true;
	int sameSize = 0; //0尚未找到开始比较  -1 点成小于0   1点成大于0
	for(int i = 0; i < polygon->vertexCount; ++i)
	{
		if (i == beginIndex)
		{
			continue;
		}

		Vector lineFromPointToEnd = {polygon->pointList[i].x - point->x, 0.0f, polygon->pointList[i].z - point->z};
		//VectorCrossProduct(&v, &lineFromPointToEnd, &StandardVector);

		float dot = VectorDotProduct(&lineFromPointToEnd, &normalVectorForfromTargetToPointLine);

		if(0 == sameSize)
		{
			sameSize = dot < 0 ? -1 : 1;
		}
		else
		{
			int size = dot < 0 ? -1 : 1;
			if(size != sameSize)
			{
				result = false;
				break;
			}
		}
	}
	return result;
}



bool AddPointToPolygon(ConvexPolygon* polygon, float x, float z, bool check)
{
	const Point point = {x, z};
	if(NULL == polygon)
	{
		return false;
	}
	
	if(polygon->vertexCount == polygon->maxVertexCount)
	{
		return false;
	}

	bool result = false;
	if(false == check)// || polygon->vertexCount < 3)
	{
		PushBackPointToPolygon(polygon, &point);
		result = true;
	}
	else
	{
		do
		{
			if(false != PointInPolygon(polygon, point.x, point.z))
			{
				break;
			}
			
			int targetPosition = -1;
			for(int i = 0; i < polygon->vertexCount; ++i)
			{
				if(IsRightInsertPosition(polygon, i, &point))
				{
					targetPosition = i;
					break;
				}
			}
			
			if(-1 != targetPosition)
			{
				if(targetPosition + 1 < polygon->vertexCount)
				{
					InsertPointToPolygon(polygon, targetPosition, &point);
				}
				else
				{
					PushBackPointToPolygon(polygon, &point);
				}
			}
			else
			{
				break;
			}

			result = true;
		}while(0);
	}
	return result;
}

const Point* GetPolygonPointList(const ConvexPolygon* polygon)
{
	if(NULL == polygon)
	{
		return NULL;
	}
	
	return polygon->pointList;
}

int GetPolygonPointCount(const ConvexPolygon* polygon)
{
	if(NULL == polygon)
	{
		return 0;
	}
	
	return polygon->vertexCount;	
}

const Vector* GetPolygonNormal( const ConvexPolygon* polygon )
{
	if(NULL == polygon)
	{
		return 0;
	}

	return polygon->normalVectorList;	
}

