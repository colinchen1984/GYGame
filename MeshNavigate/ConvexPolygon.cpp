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

bool PointInPolygon(const MeshPolygon* polygon, float x, float z)
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
	if(NULL == polygon)
	{
		return false;
	}
	
	if(polygon->vertexCount == polygon->maxVertexCount)
	{
		return false;
	}

	PushBackPointToPolygon(polygon, &point);
	return true;
}


bool IsConvexPolygon( MeshPolygon* polygon )
{
	if (polygon->vertexCount < 3)
	{
		return false;
	}

	if (3 == polygon->vertexCount)
	{
		return true;
	}
	bool result = true;
	for (int i = 0; i < polygon->vertexCount; ++i)
	{
		
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

const Vector* GetPolygonNormal( const MeshPolygon* polygon )
{
	if(NULL == polygon)
	{
		return 0;
	}

	return polygon->normalVectorList;	
}

