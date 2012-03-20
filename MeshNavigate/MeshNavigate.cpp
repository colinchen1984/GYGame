/*
**文件名称：MeshNavigate.cpp
**功能描述：网格寻路基础数据结构
**文件说明：
**作    者：陈琳
**创建时间：2012-02-21
**修    改：
*/


#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include <math.h>
#include <assert.h>
#include "BaseStruct.h"
#include "ConcavePolygonDecompose.h"
#include "PolygonClipping.h"
#include "Polygon.h"
#include "ObjectMesh.h"
#include "VectorMath.h"

struct MeshPolygon;
struct Zone
{
	Queue*				polygonList;
	Rect				zoneRect;
};


static const int MN_INVALID_VALUE = -1;

struct MeshNavigateSystem
{
	Point*			m_pointList;				// 顶点列表
	int				m_currentPointCount;		// 当前顶点数量
	int				m_terrianVertexsCount;		// 地形顶点列表数量
	int				m_maxPointCount;			// 顶点列表的最大容量

	//场景信息
	int				m_sceneMaxX;				// 场景X最大值
	int				m_sceneMaxZ;				// 场景Z最大值
	int				m_terriaonVertexSpacing;	// 地形的顶点间隔

	//寻路的区域管理信息
	int				m_xZoneCount;				// x轴分割的区域的数量
	int				m_zZoneCount;				// z轴分割的区域的数量
	Zone*			m_gridZoneList;				// 场景区域列表
	int				m_zoneSize;					// 区域的大小
	int				m_zoneCount;				// 整个场景的区域的数量

	MeshPolygon*	m_polygonList;				// 场景内多边形列表

	int				m_triangleCount;			// 场景内多边形数量

	Queue*			m_itemMeshQueue;			// 物体通过计算后得到的多边形
};

MeshNavigateSystem* CreateGridSystem()
{
	return (MeshNavigateSystem*)malloc(sizeof(MeshNavigateSystem));
}

static int GetZoneIDByPosition(MeshNavigateSystem* sys, const float x, const float z)
{
	int xIndex = (int)x;
	int zIndex = (int)z;
	xIndex /= sys->m_zoneSize;
	zIndex /= sys->m_zoneSize;
	int zoneID = xIndex + zIndex * sys->m_zZoneCount;
	return zoneID;
}

static void MakeTerraionMesh(MeshNavigateSystem* sys, const float* heightData, const int dataCount)
{
	int zStep = sys->m_sceneMaxZ / sys->m_terriaonVertexSpacing;
	for (int triangleZ = 0; triangleZ < sys->m_sceneMaxZ - sys->m_terriaonVertexSpacing; triangleZ +=sys->m_terriaonVertexSpacing)
	{
		for(int triangleX = 0; triangleX < sys->m_sceneMaxX - sys->m_terriaonVertexSpacing; triangleX +=sys->m_terriaonVertexSpacing)
		{
			//得到三角形的顶点, 顺时针方向
			/*
				A-----B
				| \   |
				|  \  |
				|   \ |
				D-----C
			*/
			int pointAX = triangleX;
			int pointAZ = triangleZ;
			int pointAHeightIndex = triangleZ * zStep + triangleX;
			int pointBXHeightIndex = (triangleZ) * zStep + triangleX + 1;
			int pointCXHeightIndex = (triangleZ + 1) * zStep + triangleX + 1;
			int pointDXHeightIndex = (triangleZ + 1) * zStep + triangleX;
			Vector pointA = {(float)pointAX, heightData[pointAHeightIndex], (float)pointAZ};
			Vector pointB = {(float)pointAX + 1, heightData[pointBXHeightIndex], (float)pointAZ};
			Vector pointC = {(float)pointAX + 1, heightData[pointCXHeightIndex], (float)pointAZ + 1};
			Vector pointD = {(float)pointAX , heightData[pointDXHeightIndex], (float)pointAZ + 1};
			
			//求三角形ACD是否可以行走
			Vector CD = {pointC.x - pointD.x, pointC.y - pointD.y, pointC.z - pointD.z};
			Vector AD = {pointA.x - pointD.x, pointA.y - pointD.y, pointA.z - pointD.z};
			Vector planeNormalACD;
			VectorCrossProduct(&planeNormalACD, NormalizationVector(&CD), NormalizationVector(&AD));
			float cosValue = VectorDotProduct(NormalizationVector(&planeNormalACD), &StandardVector);
			const static float tag = cos(PI / 4);
			bool planeACDWork = abs(cosValue) < tag ? false : true;

			//求三角形ABC是否可以行走
			Vector AB = {pointA.x - pointB.x, pointA.y - pointB.y, pointA.z - pointB.z};
			Vector CB = {pointC.x - pointB.x, pointC.y - pointB.y, pointC.z - pointB.z};
			Vector planeNormalABC;
			VectorCrossProduct(&planeNormalABC, NormalizationVector(&AB), NormalizationVector(&CB));
			cosValue = VectorDotProduct(NormalizationVector(&planeNormalABC), &StandardVector);
			bool planeABCWork = abs(cosValue) < tag ? false : true;
			MeshPolygon* polygon = NULL;
			if (planeABCWork && planeACDWork)
			{
				polygon = CreatePolygon(4);
				AddPointToPolygon(polygon, pointA.x, pointA.z);
				AddPointToPolygon(polygon, pointB.x, pointB.z);
				AddPointToPolygon(polygon, pointC.x, pointC.z);
				AddPointToPolygon(polygon, pointD.x, pointD.z);
			}
			else if(planeACDWork)
			{
				polygon = CreatePolygon(3);
				AddPointToPolygon(polygon, pointA.x, pointA.z);
				AddPointToPolygon(polygon, pointC.x, pointC.z);
				AddPointToPolygon(polygon, pointD.x, pointD.z);
			}
			else if(planeABCWork)
			{
				polygon = CreatePolygon(3);
				AddPointToPolygon(polygon, pointA.x, pointA.z);
				AddPointToPolygon(polygon, pointB.x, pointB.z);
				AddPointToPolygon(polygon, pointC.x, pointC.z);
			}

			if (NULL != polygon)
			{
				// 向所在的区域注册
				int zoneId = GetZoneIDByPosition(sys, pointA.x, pointA.z);
				assert(zoneId >= 0);
				PushDataToQueue(sys->m_gridZoneList[zoneId].polygonList, (void*)polygon);
			}
		}
	}
}

static void MakeZoneData(MeshNavigateSystem* sys)
{

	//初始化区域管理相关数据
	sys->m_zoneSize = 16;
	sys->m_xZoneCount = sys->m_sceneMaxX / sys->m_zoneSize;
	sys->m_zZoneCount = sys->m_sceneMaxZ / sys->m_zoneSize;
	sys->m_zoneCount = sys->m_xZoneCount * sys->m_zZoneCount; 
	sys->m_gridZoneList = (Zone*)malloc(sizeof(Zone) * sys->m_zoneCount);
	for (int z = 0; z < sys->m_xZoneCount; ++z)
	{
		int zIndex = z * sys->m_zZoneCount;
		for (int x = 0; x < sys->m_xZoneCount; ++x)
		{
			sys->m_gridZoneList[zIndex + x].polygonList = CreateQueue(32);
			Point leftTop = {(float)(x * sys->m_zoneSize), (float)(z * sys->m_zoneSize)};
			Point rightBottom = {(float)((x + 1) * sys->m_zoneSize), (float)((z + 1) * sys->m_zoneSize)};
			int zondIndex = zIndex + x;
			MakeRectByPoint(&sys->m_gridZoneList[zondIndex].zoneRect, &leftTop, &rightBottom);
		}
	}
}

bool InitGridSystem(MeshNavigateSystem* sys, int maxXSize, int maxZSize, int terriaonVertexSpacing, const float* heightData, const int heightDataCount)
{
	if(NULL == sys)
	{
		return false;
	}
	memset(sys, 0, sizeof(*sys));
	if(0 != maxXSize % 2)
	{
		return false;
	}
	
	if(0 != maxZSize % 2)
	{
		return false;
	}
	
	if(0 != maxXSize % terriaonVertexSpacing)
	{
		return false;
	}
	
	if(0 != maxZSize % terriaonVertexSpacing)
	{
		return false;
	}
	
	sys->m_sceneMaxX = maxXSize;
	sys->m_sceneMaxZ = maxZSize;
	sys->m_terriaonVertexSpacing  = terriaonVertexSpacing;
	sys->m_terrianVertexsCount = (maxXSize / terriaonVertexSpacing + 1) * (maxZSize / terriaonVertexSpacing + 1);
	if(heightDataCount != sys->m_terrianVertexsCount)
	{
		return false;
	}
	sys->m_maxPointCount = sys->m_terrianVertexsCount << 1; //初始化是列表的大小是场景地形顶点个数的double
	sys->m_pointList = (Point*)malloc(sizeof(Point) * sys->m_maxPointCount);
	if(NULL == sys->m_pointList)
	{
		return false;
	}
	memset(sys->m_pointList, 0, sizeof(Point) * sys->m_maxPointCount);

	sys->m_itemMeshQueue = CreateQueue(128);
	
	MakeZoneData(sys);
	MakeTerraionMesh(sys, heightData, heightDataCount);
	
	return 0;
}

#define GetDistence(ax, az, bx, bz) ((ax - bx) * (ax - bx) + (az - bz) * (az - bz))
int AddPointToSystem(MeshNavigateSystem* sys, float x, float z, bool checkRange)
{
	if(NULL == sys)
	{
		return -1;
	}
	
	//如果已经装满,重新分配内存
	if(sys->m_currentPointCount + 1 == sys->m_maxPointCount)
	{
		sys->m_maxPointCount = sys->m_maxPointCount << 1;
		sys->m_pointList = (Point*)realloc(sys->m_pointList, sizeof(Point) * sys->m_maxPointCount);
		if(NULL == sys->m_pointList)
		{
			return -1;
		}
	}

	int result = -1;
	if(true == checkRange)
	{
		for(int i = 0; i < sys->m_currentPointCount; ++i)
		{
			const static float minRange = 0.01f * 0.01f;
			if(GetDistence(x, z, sys->m_pointList[i].x, sys->m_pointList[i].z) < minRange)
			{
				result = i;
				break;
			}
		}

	}
	
	if(-1 == result)
	{
		result = sys->m_currentPointCount++;
		Point* p = &sys->m_pointList[result];
		p->x = x;
		p->z = z;
	}
		
	return result;
}

bool AddItemToGridSystem( MeshNavigateSystem* sys, ItemNavigateMesh* item )
{
	if (NULL == item)
	{
		return false;
	}

	if (NULL == sys)
	{
		return false;
	}

	MeshPolygon* itemConvexHullPolygon = GetConvexHullToPolygon(item);
	if (NULL == itemConvexHullPolygon)
	{
		return false;
	}
	ReleaseItemNavigateMesh(item);
	PushDataToQueue(sys->m_itemMeshQueue, (void*)itemConvexHullPolygon);
	return true;
}

static bool UpdateItemPolygon(MeshNavigateSystem* sys)
{
	Queue* resultQueue = CreateQueue(GetDataCountFromQueue(sys->m_itemMeshQueue));
	Queue* clippQueue = CreateQueue(GetDataCountFromQueue(sys->m_itemMeshQueue));
	Queue* decomposeQueue = CreateQueue(GetDataCountFromQueue(sys->m_itemMeshQueue));
	while (MeshPolygon* polygon = (MeshPolygon*)PopDataFromQueue(sys->m_itemMeshQueue))
	{
		int restPolygonCount = GetDataCountFromQueue(sys->m_itemMeshQueue);
		int clippResult = 0;
		MeshPolygon* clppingWindow = NULL;
		int checkIndex = 0;
		for (; checkIndex < restPolygonCount; ++checkIndex)
		{
			clppingWindow = (MeshPolygon*)GetDataFromQueueByIndex(sys->m_itemMeshQueue, checkIndex);
			CleanQueue(clippQueue);
			clippResult = ConvexPolygonClipping(polygon, clppingWindow, clippQueue);
			if(NOT_NEED_CLIPPING != clippResult)
			{
				break;
			}
		}

		if (checkIndex == restPolygonCount)
		{
			//不予任何多边形相交
			//直接放入结果队列
			PushDataToQueue(resultQueue, polygon);
			continue;
		}
		else if(ALL_POINT_IN_CLIPPING_WINDOW == clippResult)
		{
			//被裁剪多边形位于裁剪多边形内部，所以裁剪后完全消失
			ReleasePolygon(polygon);
			continue;
		}
		else if(CLIPPING_WINDOW_ALL_IN_CLIPPED_POLYGON == clippResult)
		{
			//裁剪多边形位于被裁减多边形内部
			PushDataToQueue(resultQueue, polygon);
			//从Queue删除需要删除的多边形
			DeleteDataFromQueue(sys->m_itemMeshQueue, checkIndex);
			ReleasePolygon(clppingWindow);
			continue;
		}
		else if(WRONG_PARAM == clippResult)
		{
			//传入的的参数有误
		}

		assert(0 == clippResult);
		//裁剪成功
		//对裁剪出来的每一个多边形做凹多边形分解
		while (MeshPolygon* convexPolygon = (MeshPolygon*)PopDataFromQueue(clippQueue))
		{
			CleanQueue(decomposeQueue);
			int decomposeResult = ConcavePolygonDecompose(convexPolygon, decomposeQueue);
			if (NO_NEED_DECOMPOSE == decomposeResult)
			{
				PushDataToQueue(sys->m_itemMeshQueue, (void*)convexPolygon);
			}
			else if (0 == decomposeResult)
			{
				ReleasePolygon(convexPolygon);
				while (void* data = PopDataFromQueue(decomposeQueue))
				{
					PushDataToQueue(sys->m_itemMeshQueue, data);
				}
			}
			else
			{
				assert(0);
			}
		}
	}

	while(void* data = PopDataFromQueue(resultQueue))
	{
		PushDataToQueue(sys->m_itemMeshQueue, data);
	}
	ReleaseQueue(resultQueue);
	ReleaseQueue(clippQueue);
	ReleaseQueue(decomposeQueue);
	return true;
}

bool MakeMeshNavigateData( MeshNavigateSystem* sys )
{
	//裁剪, 分解物品凸包
	if(true != UpdateItemPolygon(sys))
	{
		return false;
	}
	
	//

	return true;
}

Queue* GetItemQueue(MeshNavigateSystem* sys)
{
	return sys->m_itemMeshQueue;
}
