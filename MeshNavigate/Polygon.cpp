/*
**文件名称：Polygon.cpp
**功能描述：多边形
**文件说明：
**作    者：陈琳
**创建时间：2012-02-21
**修    改：
*/
#include "BaseStruct.h"
#include "VectorMath.h"
#include "Polygon.h"
#include <string.h>
#include <malloc.h>
#include <math.h>

struct MeshPolygon
{
	//要求顶点顺时针
	Point* 			pointList;
	Vector* 		normalVectorList;
	int 			vertexCount;
	int				maxVertexCount;
};



MeshPolygon* CreatePolygon(int maxVertexCount)
{
	if(maxVertexCount < 3)
	{
		return NULL;
	}
	
	MeshPolygon* p = (MeshPolygon*)malloc(sizeof(MeshPolygon));
	memset(p, 0, sizeof(MeshPolygon));
	
	p->pointList = (Point*)malloc(sizeof(Point) * maxVertexCount);
	memset(p->pointList, 0, sizeof(p->pointList[0]) * maxVertexCount);
	p->normalVectorList = (Vector*)malloc(sizeof(Vector)* maxVertexCount);
	memset(p->normalVectorList, 0, sizeof(p->normalVectorList[0]) * maxVertexCount);
	p->maxVertexCount = maxVertexCount;
	return p;
}

void ReleasePolygon(MeshPolygon* polygon)
{
	if(NULL == polygon)
	{
		return;
	}
	free(polygon->pointList);
	free(polygon->normalVectorList);
	free(polygon);
}

void CleanPolygon(MeshPolygon* polygon)
{
	if(NULL == polygon)
	{
		return;
	}
	memset(polygon->normalVectorList, 0, sizeof(polygon->normalVectorList[0]) * polygon->maxVertexCount);
	memset(polygon->pointList, 0, sizeof(polygon->pointList[0]) * polygon->maxVertexCount);
	polygon->vertexCount = 0;
}

static bool CopyConvexPolygon( MeshPolygon* dest, const MeshPolygon* src)
{
	if (dest->maxVertexCount < src->maxVertexCount)
	{
		return false;
	}
	memcpy(dest->pointList, src->pointList, sizeof(src->pointList[0]) * src->vertexCount);
	memcpy(dest->normalVectorList, src->normalVectorList, sizeof(src->pointList[0]) * src->vertexCount);
	dest->vertexCount = src->vertexCount;
}

MeshPolygon* MakePolygonCopy( MeshPolygon* polygon )
{
	if(NULL == polygon)
	{
		return NULL;
	}
	MeshPolygon* p = (MeshPolygon*)malloc(sizeof(MeshPolygon));
	memcpy(p, polygon, sizeof(MeshPolygon));

	p->pointList = (Point*)malloc(sizeof(Point) * polygon->maxVertexCount);
	memcpy(p->pointList, polygon->pointList, sizeof(p->pointList[0]) * polygon->maxVertexCount);
	p->normalVectorList = (Vector*)malloc(sizeof(Vector)* polygon->maxVertexCount);
	memcpy(p->normalVectorList, polygon->normalVectorList, sizeof(p->normalVectorList[0]) * polygon->maxVertexCount);
	return p;
}

int PointInPolygon(const MeshPolygon* polygon, float x, float z)
{
	if(NULL == polygon)
	{
		return false;
	}
	
	if(polygon->vertexCount < 3)
	{
		return false;
	} 
	
	const Point point = {x, z};
	Vector v;
	v. y = 0;
	int result = 1;
	for(int i = 0; i < polygon->vertexCount; ++i)
	{
		v.x = point.x - polygon->pointList[i].x;
		v.z = point.z - polygon->pointList[i].z;
		float dot = VectorDotProduct(&v, &polygon->normalVectorList[i]);
		if(FloatEqualZero(dot))
		{
			//因为当dot为0的时候,循环break了,没有机会在检查与剩下的边的相交情况
			//所以在这里需要判断点在线段的上海市在线段的延长线上
			//从而确定点在多边形上还是在多边形外
			int nextIndex = i + 1 != polygon->vertexCount ? i + 1 : 0;
			Rect rect;
			MakeRectByPoint(&rect, &polygon->pointList[i], &polygon->pointList[nextIndex]);
			result = InRect(&rect, & point) ? 0 : -1;
			break;
		}
		else if(dot < 0)// || FloatEqualZero(dot))
		{
			result = -1;
			break;
		}
	}
	return result;
}

static void inline PushBackPointToPolygon(MeshPolygon* polygon, const Point* point)
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

bool AddPointToPolygon(MeshPolygon* polygon, float x, float z)
{
	const Point point = {x, z};

	return AddPointToPolygonByPoint(polygon, &point);
}



bool AddPointToPolygonByPoint( MeshPolygon* polygon, const Point* p )
{
	if(NULL == polygon)
	{
		return false;
	}

	if(polygon->vertexCount == polygon->maxVertexCount)
	{
		return false;
	}

	PushBackPointToPolygon(polygon, p);
	return true;
}


bool IsConvexPolygon(const MeshPolygon* polygon )
{
	bool result = false;
	if (polygon->vertexCount < 3)
	{
		result = false;
		return result;
	}
	else if (3 == polygon->vertexCount)
	{
		Vector normal0 = polygon->normalVectorList[0];
		Vector normal1 = polygon->normalVectorList[1];
		float dotaValue = VectorDotProduct(NormalizationVector(&normal0), NormalizationVector(&normal1));
		//all point in one line
		result = FloatEqual(dotaValue, 1.0f) ? false : true;
		return result;
	}
	else if(4 == polygon->vertexCount)
	{
		//求对角线是否有交点
		//有则是凸多边形
		result = IntersectionPointBettwenTwoSegments(&polygon->pointList[0], &polygon->pointList[2], &polygon->pointList[1], &polygon->pointList[3], NULL);
	}
	else
	{
		result = true;
		for (int i = 0; i < polygon->vertexCount; ++i)
		{
			//对每一个点A, 检查其他点是否都 A -> A1同一侧
			if (true != result)
			{
				break;
			}
			int allInOneLine = 0;;
			for (int t = 0; t < polygon->vertexCount; ++t)
			{
				if (t == i)
				{
					continue;
				}
				Vector checkVector = {polygon->pointList[t].x - polygon->pointList[i].x, 0.0f, polygon->pointList[t].z - polygon->pointList[i].z};
				float checkPointDotValue = VectorDotProduct(&checkVector, &polygon->normalVectorList[i]);
				if (checkPointDotValue < 0.0f)
				{
					result = false;
					break;
				}
				if (FloatEqualZero(checkPointDotValue))
				{
					++allInOneLine;
				}
			}
			//all point in one line
			if (allInOneLine == polygon->vertexCount - 1)
			{
				result = false;
			}
		}
	}
	return result;
}


const Point* GetPolygonPointList(const MeshPolygon* polygon)
{
	if(NULL == polygon)
	{
		return NULL;
	}
	
	return polygon->pointList;
}

int GetPolygonPointCount(const MeshPolygon* polygon)
{
	if(NULL == polygon)
	{
		return 0;
	}
	
	return polygon->vertexCount;	
}

int GetPolygonPointMaxCount( const MeshPolygon* polygon )
{
	if(NULL == polygon)
	{
		return 0;
	}

	return polygon->maxVertexCount;	
}


const Vector* GetPolygonNormal( const MeshPolygon* polygon )
{
	if(NULL == polygon)
	{
		return 0;
	}

	return polygon->normalVectorList;	
}


MeshPolygon* CreatePolygonByPoint( const Point* pointList, const int pointCount )
{
	MeshPolygon* polygon = CreatePolygon(pointCount);
	if (NULL == polygon)
	{
		return NULL;
	}

	for (int i = 0; i < pointCount; ++i)
	{
		PushBackPointToPolygon(polygon, &pointList[i]);
	}
	return polygon;
}

bool IsSamePolygon( const MeshPolygon* a, const MeshPolygon* b )
{
	if (a == b)
	{
		return true;
	}
	if (a->vertexCount != b->vertexCount)
	{
		return false;
	}
	bool result = true;
	for (int i = 0; i < a->vertexCount; ++i)
	{
		if(!IsSamePoint(&a->pointList[i], &b->pointList[i]))
		{
			result = false;
			break;
		}
	}
	return result;
}
