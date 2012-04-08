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
#include <Windows.h>

struct MeshPolygon;
struct Zone
{
	Queue*				walkblePolygon;

	Rect				zoneRect;

	Point*				pointListForZone;
	int					maxPointCount;
	int					currentPointCount;
	int					beginIndex;
	int					zoneID;
	static int			vertexCount;
};

int Zone::vertexCount = 0;


struct AdjacentData
{
	struct _AdjacentData
	{
		int pointIndex;
		MeshPolygon*	polygon;
	};
	_AdjacentData*	data;
	int				dataCount;
	int				maxDataCount;
};

struct MeshNavigateGenerator
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

	int				m_triangleCount;			// 场景内多边形数量

	Queue*			m_itemMeshQueue;			// 物体通过计算后得到的多边形
	
	Queue*			m_unwalkblePolygon;
	Queue*			m_walkblePolygon;

	AdjacentData*	m_adjacentData;
	int				m_adjacentDataCount;			//

	FILE*			m_logFile;			


	Queue*			 m_test;
};

#define BEGINPROFILE unsigned int time = GetTickCount();
#define ENDPROFILE(sys) time = GetTickCount() - time; fprintf(sys->m_logFile, "%s\t%u\n", __FUNCTION__, time); fflush(sys->m_logFile);
MeshNavigateGenerator* CreateMeshNavigateGenarator()
{
	
	void* p = malloc(sizeof(MeshNavigateGenerator));
	memset(p, 0, sizeof(MeshNavigateGenerator));
	return (MeshNavigateGenerator*)p;
}

static int GetZoneIDByPosition(MeshNavigateGenerator* sys, const float x, const float z)
{
	int xIndex = (int)x;
	int zIndex = (int)z;
	xIndex = (xIndex) / sys->m_zoneSize;
	zIndex = (zIndex) / sys->m_zoneSize;
	xIndex == sys->m_xZoneCount ? --xIndex : 0;
	zIndex == sys->m_zZoneCount ? --zIndex : 0;
	int zoneID = xIndex + zIndex * sys->m_zZoneCount;
	return zoneID;
}

static void MakeTerraionMesh(MeshNavigateGenerator* sys, const float* heightData, const int dataCount)
{
	BEGINPROFILE;
	int zStep = sys->m_sceneMaxZ / sys->m_terriaonVertexSpacing + 1;
	for (int triangleZ = 0; triangleZ <= sys->m_sceneMaxZ - sys->m_terriaonVertexSpacing; triangleZ +=sys->m_terriaonVertexSpacing)
	{
		for(int triangleX = 0; triangleX <= sys->m_sceneMaxX - sys->m_terriaonVertexSpacing; triangleX +=sys->m_terriaonVertexSpacing)
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
			Point pA = {pointA.x, pointA.z};
			pA.userData.floatUserData = pointA.y;
			Point pB = {pointB.x, pointB.z};
			pB.userData.floatUserData = pointB.y;
			Point pC = {pointC.x, pointC.z};
			pC.userData.floatUserData = pointC.y;
			Point pD = {pointD.x, pointD.z};
			pD.userData.floatUserData = pointD.y;

  			if (!planeABCWork && !planeACDWork)
  			{
  				polygon = CreatePolygon(4);
  				AddPointToPolygonByPoint(polygon, &pA);
  				AddPointToPolygonByPoint(polygon, &pB);
  				AddPointToPolygonByPoint(polygon, &pC);
  				AddPointToPolygonByPoint(polygon, &pD);
  			}
			else if(!planeACDWork)
			{
				polygon = CreatePolygon(3);
				AddPointToPolygonByPoint(polygon, &pA);
				AddPointToPolygonByPoint(polygon, &pC);
				AddPointToPolygonByPoint(polygon, &pD);
			}
			else if(!planeABCWork)
			{
				polygon = CreatePolygon(3);
				AddPointToPolygonByPoint(polygon, &pA);
				AddPointToPolygonByPoint(polygon, &pB);
				AddPointToPolygonByPoint(polygon, &pC);
			}

  			if (NULL != polygon)
  			{
  				// 向所在的区域注册不能行走的多边形区域
  				int zoneId = GetZoneIDByPosition(sys, pointA.x, pointA.z);
  				assert(zoneId >= 0);
				PushDataToQueue(sys->m_unwalkblePolygon, (void*)polygon);
  			}
		}
	}
	ENDPROFILE(sys);
}

static void MakeZoneData(MeshNavigateGenerator* sys)
{
	BEGINPROFILE;
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
			sys->m_gridZoneList[zIndex + x].walkblePolygon = CreateQueue(32);
			sys->m_gridZoneList[zIndex + x].currentPointCount = 0;
			sys->m_gridZoneList[zIndex + x].beginIndex = 0;
			sys->m_gridZoneList[zIndex + x].maxPointCount = 32;
			sys->m_gridZoneList[zIndex + x].zoneID = zIndex + x;
			sys->m_gridZoneList[zIndex + x].pointListForZone = (Point*)malloc(sizeof(Point) * sys->m_gridZoneList[zIndex + x].maxPointCount);
			Point leftTop = {(float)(x * sys->m_zoneSize), (float)(z * sys->m_zoneSize)};
			Point rightBottom = {(float)((x + 1) * sys->m_zoneSize), (float)((z + 1) * sys->m_zoneSize)};
			int zondIndex = zIndex + x;
			MakeRectByPoint(&sys->m_gridZoneList[zondIndex].zoneRect, &leftTop, &rightBottom);
		}
	}
	ENDPROFILE(sys);
}

bool InitMeshNavigateGenarator(MeshNavigateGenerator* sys, int maxXSize, int maxZSize, int terriaonVertexSpacing, const float* heightData, const int heightDataCount)
{
	BEGINPROFILE;
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
	sys->m_unwalkblePolygon = CreateQueue(128);
	sys->m_walkblePolygon = CreateQueue(128);
	sys->m_currentPointCount = 0;
	sys->m_maxPointCount = 0;
	sys->m_pointList = NULL;

	sys->m_itemMeshQueue = CreateQueue(128);
	sys->m_logFile = fopen("meshnavigate.log", "wb");
	MakeZoneData(sys);
	MakeTerraionMesh(sys, heightData, heightDataCount);
	sys->m_test = CreateQueue(128);
	ENDPROFILE(sys);
	return 0;
}

static void FreeZone(MeshNavigateGenerator* sys)
{
	for (int i = 0; i < sys->m_zoneCount; ++i)
	{
		ReleaseQueue(sys->m_gridZoneList[i].walkblePolygon);
		free(sys->m_gridZoneList[i].pointListForZone);
	}
	free(sys->m_gridZoneList);
}

void ReleaseMeshNavigateGenarator(MeshNavigateGenerator* sys)
{
	if(NULL == sys)
	{
		return;
	}


	
	//释放zone
	FreeZone(sys);
	//释放顶点
	free(sys->m_pointList);

	//释放物体mesh
	while(MeshPolygon* polygon = (MeshPolygon*)PopDataFromQueue(sys->m_itemMeshQueue))
	{
		ReleasePolygon(polygon);
	}
	ReleaseQueue(sys->m_itemMeshQueue);

	while(MeshPolygon* polygon = (MeshPolygon*)PopDataFromQueue(sys->m_unwalkblePolygon))
	{
		ReleasePolygon(polygon);
	}
	ReleaseQueue(sys->m_unwalkblePolygon);

	while(MeshPolygon* polygon = (MeshPolygon*)PopDataFromQueue(sys->m_walkblePolygon))
	{
		ReleasePolygon(polygon);
	}
	ReleaseQueue(sys->m_walkblePolygon);

	//释放log文件
	fclose(sys->m_logFile);

	while(Point* p = (Point*)PopDataFromQueue(sys->m_test))
	{
		free(p);
	}
	ReleaseQueue(sys->m_test);

	free(sys);
}

bool AddItemToGridSystem(MeshNavigateGenerator* sys, ItemNavigateMesh* item)
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


static void ClipUnwokableAreaWithItem(MeshNavigateGenerator* sys)
{
}

//处理物体之间的叠加
static bool UpdateItemPolygon(MeshNavigateGenerator* sys)
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
				ShiftQueueData(sys->m_itemMeshQueue, decomposeQueue);
			}
			else
			{
				assert(0);
			}
		}
	}

	ShiftQueueData(sys->m_itemMeshQueue, resultQueue);
	ReleaseQueue(resultQueue);
	ReleaseQueue(clippQueue);
	ReleaseQueue(decomposeQueue);
	return true;
}

static int FindPointInZone(const Zone* zone, const Point* point)
{
	if (NULL == zone)
	{
		return -1;
	}

	int result = -1;	
	for (int i = 0; i < zone->currentPointCount; ++i)
	{
		if (IsSamePoint(&zone->pointListForZone[i], point))
		{
			result = i;
		}
	}
	return result;
}

//为一个zone内的顶点分配Index
static void ProcessVertexInZone(Zone* zone, const Zone* upZone, const Zone* leftZone)
{
	int meshCount = GetDataCountFromQueue(zone->walkblePolygon);
	for (int mesh = 0; mesh < meshCount; ++mesh)
	{
		MeshPolygon* meshPolygon = (MeshPolygon*)GetDataFromQueueByIndex(zone->walkblePolygon, mesh);
		int pointCount = GetPolygonPointCount(meshPolygon);
		Point* pointList = (Point*)GetPolygonPointList(meshPolygon);
		for (int p = 0; p < pointCount; ++p)
		{
			Point* point = &pointList[p];
			if(!InRect(&zone->zoneRect, point))
			{
				continue;
			}

			int pointID = -1;
			do 
			{
				//首先尝试在UpZone内查找
				pointID = FindPointInZone(zone, point);
				if (-1 != pointID)
				{
					pointID += zone->beginIndex;
					break;
				}
				//再尝试在UpZone内查找
				pointID = FindPointInZone(upZone, point);
				if (-1 != pointID)
				{
					pointID += upZone->beginIndex;
					break;
				}

				//然后尝试在leftZone内查找
				pointID = FindPointInZone(leftZone, point);
				if (-1 != pointID)
				{
					pointID += leftZone->beginIndex;
					break;
				}
				
				//最后添加到自己的zone内
				if(zone->currentPointCount + 1 == zone->maxPointCount)
				{
					zone->maxPointCount = zone->maxPointCount << 1;
					zone->pointListForZone = (Point*)realloc(zone->pointListForZone, sizeof(Point) * zone->maxPointCount);
				}
				if (0 == zone->currentPointCount)
				{
					zone->beginIndex = Zone::vertexCount;
				}
				zone->pointListForZone[zone->currentPointCount++] = *point;
				zone->pointListForZone[zone->currentPointCount - 1].userData.floatUserData = 20.0f;
				pointID = Zone::vertexCount++;
			} while (false);
			point->userData.intUserData = pointID;	
		}
	}

}

static void ProcessVertex(MeshNavigateGenerator* sys)
{
	BEGINPROFILE;
	Zone::vertexCount = 0;
	for (int z = 0; z < sys->m_zZoneCount; ++z)
	{
		int zStep = z * sys->m_xZoneCount;
		for (int x = 0; x < sys->m_xZoneCount; ++x)
		{
			int zoneID = x + zStep;
			const Zone* upZone = z > 0 ? &sys->m_gridZoneList[zoneID - sys->m_zZoneCount] : NULL;
			const Zone* leftZone = x > 0 ? &sys->m_gridZoneList[zoneID - 1] : NULL;
			ProcessVertexInZone(&sys->m_gridZoneList[zoneID], upZone, leftZone);
		}
	}
	sys->m_currentPointCount = 0;
	sys->m_maxPointCount = Zone::vertexCount;
	sys->m_pointList = (Point*)malloc(sizeof(Point) * sys->m_maxPointCount);
	for (int i = 0; i < sys->m_zoneCount; ++i)
	{
		memcpy(&sys->m_pointList[sys->m_currentPointCount], sys->m_gridZoneList[i].pointListForZone, sizeof(Point) * sys->m_gridZoneList[i].currentPointCount);
		sys->m_currentPointCount += sys->m_gridZoneList[i].currentPointCount;
	}
	ENDPROFILE(sys);
}

//处理多边形的相邻关系
static void ProcessAdjacent(MeshNavigateGenerator* sys, MeshPolygon* polygon)
{
	int pointCount = GetPolygonPointCount(polygon);
	Point* pointList = (Point*)GetPolygonPointList(polygon);
	for (int p = 0; p < pointCount; ++p)
	{
		Point* current = &pointList[p];
		Point* nextPoint = p + 1 != pointCount ? current + 1 : pointList;
		int smallIndex = -1;
		int bigIndex = -1;
		//所有的邻边在处理时都按点的index从小到大处理
		//这里不需要在意clockwise或者countwiseclock

		if (current->userData.intUserData <= nextPoint->userData.intUserData)
		{
			smallIndex = current->userData.intUserData;
			bigIndex = nextPoint->userData.intUserData;
		}
		else
		{
			smallIndex = nextPoint->userData.intUserData;
			bigIndex = current->userData.intUserData;
		}
		//如果需要,分配内纯
		if (smallIndex >= sys->m_adjacentDataCount)
		{
			int currentCount = sys->m_adjacentDataCount;
			sys->m_adjacentDataCount = smallIndex << 1;
			sys->m_adjacentData = (AdjacentData*)realloc(sys->m_adjacentData, sizeof(AdjacentData) * sys->m_adjacentDataCount);
			memset(&sys->m_adjacentData[currentCount], 0, sizeof(sys->m_adjacentData[0]) * (sys->m_adjacentDataCount - currentCount));
		}
		
		AdjacentData* pPointAdjcentData = &sys->m_adjacentData[smallIndex];
		if (0 == pPointAdjcentData->maxDataCount)
		{
			//如果初次访问,分配内存
			pPointAdjcentData->maxDataCount = 4;
			pPointAdjcentData->data = (AdjacentData::_AdjacentData*)(malloc(sizeof(AdjacentData::_AdjacentData) * pPointAdjcentData->maxDataCount));
			memset(pPointAdjcentData->data, 0, sizeof(pPointAdjcentData->data[0]) * pPointAdjcentData->maxDataCount);
			pPointAdjcentData->dataCount = 0;
		}

		bool processed = false;
		for (int dataIndex = 0; dataIndex < pPointAdjcentData->dataCount; ++dataIndex)
		{
			//在该index对应的数据内查找是否有邻边
			//如果已经有了,则通知两个polygon添加邻边数据
			//并且将该内存标记为为使用
			if (bigIndex == pPointAdjcentData->data[dataIndex].pointIndex)
			{
				AddAdjacentData(polygon, smallIndex, bigIndex, pPointAdjcentData->data[dataIndex].polygon);
				AddAdjacentData(pPointAdjcentData->data[dataIndex].polygon, smallIndex, bigIndex, polygon);
				pPointAdjcentData->data[dataIndex] = pPointAdjcentData->data[--pPointAdjcentData->dataCount];
				processed = true;
				break;
			}
		}

		if (processed)
		{
			continue;
		}
		
		//检查是否需要分配内存
		if (pPointAdjcentData->dataCount == pPointAdjcentData->maxDataCount)
		{
			pPointAdjcentData->maxDataCount = pPointAdjcentData->maxDataCount << 1;
			pPointAdjcentData->data = (AdjacentData::_AdjacentData*)realloc(pPointAdjcentData->data, sizeof(pPointAdjcentData->data[0]) * pPointAdjcentData->maxDataCount);
			memset(&pPointAdjcentData->data[pPointAdjcentData->dataCount], 0, sizeof(pPointAdjcentData->data[0]) * pPointAdjcentData->dataCount);
		}
		//如果该邻边是初次添加则直接添加
		pPointAdjcentData->data[pPointAdjcentData->dataCount].pointIndex = bigIndex;
		pPointAdjcentData->data[pPointAdjcentData->dataCount].polygon = polygon;
		++pPointAdjcentData->dataCount;
	}
}

static bool BuildEdgeInfo(MeshNavigateGenerator* sys)
{
	BEGINPROFILE;
	if (NULL != sys->m_adjacentData)
	{
		return false;
	}

	sys->m_adjacentDataCount = sys->m_maxPointCount >> 2;
	sys->m_adjacentData = (AdjacentData*)malloc(sizeof(AdjacentData) * sys->m_adjacentDataCount);
	memset(sys->m_adjacentData, 0, sizeof(sys->m_adjacentData[0]) * sys->m_adjacentDataCount);

	int meshCount = GetDataCountFromQueue(sys->m_walkblePolygon);
	for (int mesh = 0; mesh < meshCount; ++mesh)
	{
		MeshPolygon* meshPolygon = (MeshPolygon*)GetDataFromQueueByIndex(sys->m_walkblePolygon, mesh);
		ProcessAdjacent(sys, meshPolygon);
	}

	for (int i = 0; i < sys->m_adjacentDataCount; ++i)
	{
		free(sys->m_adjacentData[i].data);
	}
	free(sys->m_adjacentData);
	sys->m_adjacentData = NULL;
	ENDPROFILE(sys);
	return true;
}


//求三角形的外接圆
//抄自计算几何 有兴趣自己去看书吧
static void BuildPolygonCircle(Circle* c, MeshPolygon* polygon)
{
	const Point* pointList = GetPolygonPointList(polygon);
	float A = pointList[1].x - pointList[0].x;
	float B = pointList[1].z - pointList[0].z;
	float C = pointList[2].x - pointList[0].x;
	float D = pointList[2].z - pointList[0].z;

	float E = A * (pointList[0].x + pointList[1].x) + B * (pointList[0].z + pointList[1].z);
	float F = C * (pointList[0].x + pointList[2].x) + D * (pointList[0].z + pointList[2].z);

	float G = 2.0f * (A * (pointList[2].z - pointList[1].z) - B * (pointList[2].x - pointList[1].x));

	c->center.x = (D * E - B * F) / G;
	c->center.z = (A * F - C * E) / G;
	c->radiusSquare = (pointList[0].x - c->center.x) * (pointList[0].x - c->center.x) + (pointList[0].z - c->center.z) * (pointList[0].z - c->center.z);
}

static Queue* UniqueEdga(Queue* queue)
{
	Queue* result = CreateQueue(GetDataCountFromQueue(queue));
	int edgaCount = GetDataCountFromQueue(queue);
	for (int i = 0; i < edgaCount; ++i)
	{
		Edga* e = (Edga*)GetDataFromQueueByIndex(queue, i);
		bool unique = true;
		for (int t = 0; t < edgaCount; ++t)
		{
			if (i == t)
			{
				continue;
			}

			if(IsSameEdga(e, (Edga*)GetDataFromQueueByIndex(queue, t)))
			{
				unique = false;
				break;
			}
		}
		if (!unique)
		{
			continue;
		}

		Edga* newEdga = (Edga*)malloc(sizeof(Edga));
		memcpy(newEdga, GetDataFromQueueByIndex(queue, i), sizeof(*newEdga));
		PushDataToQueue(result, (void*)newEdga);
	}

	while (void* p = PopDataFromQueue(queue))
	{
		free(p);
	}
	ShiftQueueData(queue, result);
	ReleaseQueue(result);
	return queue;
}

static MeshPolygon* MakeTriangle(MeshPolygon* polygon, const Point* a, const Point* b, const Point* c)
{
	if (IsSamePoint(a, b) || IsSamePoint(a, c) || IsSamePoint(c, b))
	{
		return NULL;
	}
	if (NULL != polygon)
	{
		CleanPolygon(polygon);
	}
	else
	{
		polygon = CreatePolygon(3);
		Circle* c = (Circle*)malloc(sizeof(Circle));
		c->center.x = 0.0f;
		c->center.z = 0.0f;
		c->radiusSquare = 0.0f;
		SetPolygonUserData(polygon, (void*)c);
	}
	Vector a2b = {b->x - a->x, 0.0f, b->z - a->z};
	Vector normalLine;
	VectorCrossProduct(&normalLine, &a2b, &StandardVector);
	Vector a2c =  {c->x - a->x, 0.0f, c->z - a->z};
	float cos = VectorDotProduct(&a2c, &normalLine);
	if (cos >= 0.0f)
	{
		AddPointToPolygonByPoint(polygon, a);
		AddPointToPolygonByPoint(polygon, b);
		AddPointToPolygonByPoint(polygon, c);
	}
	else
	{
		AddPointToPolygonByPoint(polygon, b);
		AddPointToPolygonByPoint(polygon, a);
		AddPointToPolygonByPoint(polygon, c);
	}
	BuildPolygonCircle((Circle*)GetPolygonUserData(polygon), polygon);
	return polygon;
}

static void AddPointToTriangleList(Queue* polygonList, const Point* point, const MeshPolygon* polygon, bool workable)
{
	Queue* needProcessPolygon = CreateQueue(GetDataCountFromQueue(polygonList));
	Queue* noneedProcessPolygon = CreateQueue(GetDataCountFromQueue(polygonList));
	while(MeshPolygon* p = (MeshPolygon*)PopDataFromQueue(polygonList))
	{
		Circle* c = (Circle*)GetPolygonUserData(p);
		if (!InCircle(c, point))
		{
			PushDataToQueue(noneedProcessPolygon, (void*)p);
		}
		else
		{
			PushDataToQueue(needProcessPolygon, (void*)p);
		}
	}

	int needProcessCount = GetDataCountFromQueue(needProcessPolygon);
	Queue* edgaQeue = CreateQueue(128);
	for (int i = 0; i < needProcessCount; ++i)
	{
		MeshPolygon* polygon = (MeshPolygon*)GetDataFromQueueByIndex(needProcessPolygon, i);
		const Point* pointList = GetPolygonPointList(polygon);
		const int pointCount = GetPolygonPointCount(polygon);
		for (int pI = 0; pI < pointCount; ++pI)
		{
			int nextIndex = pI + 1 != pointCount ? pI + 1 : 0;
			Edga* e = (Edga*)malloc(sizeof(Edga));
			e->begin = pointList[pI];
			e->end = pointList[nextIndex];
			PushDataToQueue(edgaQeue, (void*)e);
		}
	}

	UniqueEdga(edgaQeue);

	//构建新的三角形`
	while (Edga* e = (Edga*)PopDataFromQueue(edgaQeue))
	{
		if (false == workable)
		{
			if (0 == PointInPolygon(polygon, e->begin.x, e->begin.z)
				&& 0 == PointInPolygon(polygon, e->end.x, e->end.z)
				&& 0 == PointInPolygon(polygon, point->x, point->z))
			{
				//如果构成的三角形的三个点都在同一个一个不可走的多边形上
				//不添加该三角形
				free(e);
				continue;
			}
		}
		MeshPolygon* old = (MeshPolygon*)PopDataFromQueue(needProcessPolygon);
		void* newP = (void*)MakeTriangle(old, &e->begin, &e->end, point);
		if (NULL == newP)
		{
			if (NULL != old)
			{
				PushDataToQueue(needProcessPolygon, old);
			}
		}
		else
			PushDataToQueue(noneedProcessPolygon, newP);
		free(e);
	}
	ReleaseQueue(edgaQeue);
	while (MeshPolygon* p = (MeshPolygon*)PopDataFromQueue(needProcessPolygon))
	{
		free(GetPolygonUserData(p));
		ReleasePolygon(p);
	}

	
	ShiftQueueData(polygonList, noneedProcessPolygon);
	ReleaseQueue(noneedProcessPolygon);
	ReleaseQueue(needProcessPolygon);
}

static void MakeWorkArea(MeshNavigateGenerator* sys)
{
	BEGINPROFILE;
	//需要处理的点来自两部分
	//地形引起的不可行走区域
	//阻挡物引起的不可行走区域
	//首先构建全局的三角形
	MeshPolygon* sceneLeft = CreatePolygon(3);
	AddPointToPolygon(sceneLeft, 0.0f, 0.0f);
	AddPointToPolygon(sceneLeft, (float)sys->m_sceneMaxX, (float)sys->m_sceneMaxZ);
	AddPointToPolygon(sceneLeft, 0.0f, (float)sys->m_sceneMaxZ);

	MeshPolygon* sceneRight = CreatePolygon(3);
	AddPointToPolygon(sceneRight, 0.0f, 0.0f);
	AddPointToPolygon(sceneRight, (float)sys->m_sceneMaxX, 0.0f);
	AddPointToPolygon(sceneRight, (float)sys->m_sceneMaxX, (float)sys->m_sceneMaxZ);

	Circle* leftCircle = (Circle*)malloc(sizeof(Circle));
	BuildPolygonCircle(leftCircle, sceneLeft);
	SetPolygonUserData(sceneLeft, (void*)leftCircle);

	Circle* rightCircle = (Circle*)malloc(sizeof(Circle));
	BuildPolygonCircle(rightCircle, sceneRight);
	SetPolygonUserData(sceneRight, (void*)rightCircle);
	
	Queue* walkablePolygQueue = sys->m_walkblePolygon;
	PushDataToQueue(walkablePolygQueue, sceneLeft);
	PushDataToQueue(walkablePolygQueue, sceneRight);
	
	Queue* backQueue = CreateQueue(GetDataCountFromQueue(sys->m_unwalkblePolygon));
	while(MeshPolygon* p = (MeshPolygon*)PopDataFromQueue(sys->m_unwalkblePolygon))
	{
		const Point* poinstList = GetPolygonPointList(p);
		const int pointCount = GetPolygonPointCount(p);
		for (int i = 0;i < pointCount; ++i)
		{
			AddPointToTriangleList(walkablePolygQueue, &poinstList[i], p, false);
		}
		PushDataToQueue(backQueue, (void*)p);
	}
	ShiftQueueData(sys->m_unwalkblePolygon, backQueue);

	while(MeshPolygon* p = (MeshPolygon*)PopDataFromQueue(sys->m_itemMeshQueue))
	{
		const Point* poinstList = GetPolygonPointList(p);
		const int pointCount = GetPolygonPointCount(p);
		for (int i = 0;i < pointCount; ++i)
		{
			AddPointToTriangleList(walkablePolygQueue, &poinstList[i], p, false);
		}
		PushDataToQueue(backQueue, (void*)p);
	}
	ShiftQueueData(sys->m_itemMeshQueue, backQueue);
	
	int waklablePolygonCount = GetDataCountFromQueue(walkablePolygQueue);
	for (int i = 0; i < waklablePolygonCount; ++i)
	{
		MeshPolygon* p = (MeshPolygon*)GetDataFromQueueByIndex(walkablePolygQueue, i);
		//释放Circle内存
		free(GetPolygonUserData(p));
		//设置多边形ID
		SetPolygonUserData(p, (void*)i);
		//向区域注册
		const Point* pointList = GetPolygonPointList(p);
		float minx = min(min(pointList[0].x, pointList[1].x), pointList[2].x);
		float minz = min(min(pointList[0].z, pointList[1].z), pointList[2].z);
		float maxx = max(max(pointList[0].x, pointList[1].x), pointList[2].x);
		float maxz = max(max(pointList[0].z, pointList[1].z), pointList[2].z);

		int xIndexBegin = (int)(minx) / sys->m_zoneSize;
		xIndexBegin == sys->m_xZoneCount ? --xIndexBegin : 0;

		int zIndexBegin = (int)(minz) / sys->m_zoneSize;
		zIndexBegin == sys->m_xZoneCount ? --zIndexBegin : 0;

		int xIndexEnd = (int)(maxx) / sys->m_zoneSize;
		xIndexEnd == sys->m_xZoneCount ? xIndexEnd : ++xIndexEnd;

		int zIndexEnd = (int)(maxz) / sys->m_zoneSize;
		zIndexEnd == sys->m_xZoneCount ? zIndexEnd : ++zIndexEnd;
		
		for (int z = zIndexBegin; z < zIndexEnd; ++z)
		{
			for (int x = xIndexBegin; x < xIndexEnd; ++x)
			{
				int zoneId = x + sys->m_zZoneCount * z;
				PushDataToQueue(sys->m_gridZoneList[zoneId].walkblePolygon, (void*)p);
			}
		}
	}

	ENDPROFILE(sys);
}

bool MakeMeshNavigateData(MeshNavigateGenerator* sys)
{
	//裁剪, 分解物品凸包
	if(true != UpdateItemPolygon(sys))
	{
		return false;
	}
	
	//用阻挡物裁剪地形上不可走的区域
	ClipUnwokableAreaWithItem(sys);

	MakeWorkArea(sys);

	//将所有的点的坐标导入定点列表,并换成index
	ProcessVertex(sys);

	//生成多边形的相邻关系
	BuildEdgeInfo(sys);

	return true;
}

Queue* GetItemQueue(MeshNavigateGenerator* sys)
{
	return NULL != sys ? sys->m_itemMeshQueue : NULL;
}

int GetZoneCount(MeshNavigateGenerator* sys)
{
	return NULL != sys ? sys->m_zoneCount : NULL;
}

int GetMeshCount(MeshNavigateGenerator* sys)
{
	if (NULL == sys)
	{
		return 0;
	}

	return GetDataCountFromQueue(sys->m_walkblePolygon);
}

MeshPolygon* GetMeshPolygon(MeshNavigateGenerator* sys, int meshIndex)
{
	if (NULL == sys)
	{
		return NULL;
	}

	int meshCount = GetDataCountFromQueue(sys->m_walkblePolygon);
	if (meshIndex >= meshCount)
	{
		return NULL;
	}

	return (MeshPolygon*)GetDataFromQueueByIndex(sys->m_walkblePolygon, meshIndex);
}

int GetVertexCount(MeshNavigateGenerator* sys)
{
	if (NULL == sys)
	{
		return 0;
	}
	return sys->m_currentPointCount;
}

const Point* GetVertexList(MeshNavigateGenerator* sys)
{
	if (NULL == sys)
	{
		return NULL;
	}
	return sys->m_pointList;
}

bool WriteDataToFile( MeshNavigateGenerator* sys, const char* fileName )
{
	BEGINPROFILE;
	if (NULL == sys || NULL == fileName)
	{
		return false;
	}

	FILE* file = fopen(fileName, "wb");
	if(NULL == file)
	{
		return false;
	}

	//版本号
	int version = 0;
	fwrite(&version, sizeof(version), 1, file);

	//场景数据
	fwrite(&sys->m_sceneMaxX, sizeof(sys->m_sceneMaxX), 1, file);
	fwrite(&sys->m_sceneMaxZ, sizeof(sys->m_sceneMaxZ), 1, file);
	fwrite(&sys->m_zoneSize, sizeof(sys->m_zoneSize), 1, file);
	
	//坐标点数据
	fwrite(&sys->m_currentPointCount, sizeof(sys->m_currentPointCount), 1, file);
	for (int i = 0; i < sys->m_currentPointCount; ++i)
	{
		fwrite(&sys->m_pointList[i].x, sizeof(sys->m_pointList[i].x), 1, file);
		fwrite(&sys->m_pointList[i].z, sizeof(sys->m_pointList[i].z), 1, file);
	}

	//polygon数据
 	int meshCount = GetDataCountFromQueue(sys->m_walkblePolygon);
	fwrite(&meshCount, sizeof(meshCount), 1, file);
	for (int mesh = 0; mesh < meshCount; ++mesh)
	{
		MeshPolygon* meshPolygon = (MeshPolygon*)GetDataFromQueueByIndex(sys->m_walkblePolygon, mesh);
		int pointCount = GetPolygonPointCount(meshPolygon);
		Point* pointList = (Point*)GetPolygonPointList(meshPolygon);
		fwrite(&pointCount, sizeof(pointCount), 1, file);
		for (int p = 0; p < pointCount; ++p)
		{
			fwrite(&pointList[p].userData.intUserData, sizeof(pointList[p].userData.intUserData), 1, file);
		}

		const Vector* pVector = GetPolygonNormal(meshPolygon);
		for (int p = 0; p < pointCount; ++p)
		{
			fwrite(&pVector[p].x, sizeof(pVector[p].x), 1, file);
			fwrite(&pVector[p].z, sizeof(pVector[p].z), 1, file);
		}

		int specialLogic = 0;
		fwrite(&specialLogic, sizeof(specialLogic), 1, file);
	}

	//zone 数据
	for (int z = 0; z < sys->m_zZoneCount; ++z)
	{
		int zStep = z * sys->m_xZoneCount;
		for (int x = 0; x < sys->m_xZoneCount; ++x)
		{
			int zoneID = x + zStep;
			Zone* zone = &sys->m_gridZoneList[zoneID];
			int polygonCountInZone = GetDataCountFromQueue(zone->walkblePolygon);
			fwrite(&polygonCountInZone, sizeof(polygonCountInZone), 1, file);
			for (int mesh = 0; mesh < polygonCountInZone; ++mesh)
			{
				MeshPolygon* meshPolygon = (MeshPolygon*)GetDataFromQueueByIndex(zone->walkblePolygon, mesh);
				int polygonID = (int)GetPolygonUserData(meshPolygon);
				fwrite(&polygonID, sizeof(polygonID), 1, file);
			}
		}
	}

	//相邻数据
	for (int mesh = 0; mesh < meshCount; ++mesh)
	{
		MeshPolygon* meshPolygon = (MeshPolygon*)GetDataFromQueueByIndex(sys->m_walkblePolygon, mesh);
		int pointCount = GetPolygonPointCount(meshPolygon);
		for (int p = 0; p < pointCount; ++p)
		{
			MeshPolygon* adjacentPolygon = GetAdjacentData(meshPolygon, p);
			int adjacentPolygonID = NULL != adjacentPolygon ? (int)GetPolygonUserData(adjacentPolygon) : -1;
			fwrite(&adjacentPolygonID, sizeof(adjacentPolygonID), 1, file);
		}
	}
	fclose(file);
	ENDPROFILE(sys);
	return true;
}

extern void AddPointForTest( MeshNavigateGenerator* sys, float x, float z )
{
	Point* p = (Point*)malloc(sizeof(Point));
	p->x = x;
	p->z = z;
	PushDataToQueue(sys->m_test, p);
}

extern void BeginTest( MeshNavigateGenerator* sys )
{
	while(Point* p = (Point*)PopDataFromQueue(sys->m_test))
	{
		free(p);
	}
}

extern Queue* Endtest(MeshNavigateGenerator* sys, float maxx, float maxz)
{
	MeshPolygon* sceneLeft = CreatePolygon(3);
	AddPointToPolygon(sceneLeft, 0.0f, 0.0f);
	AddPointToPolygon(sceneLeft, maxx, (float)maxz);
	AddPointToPolygon(sceneLeft, 0.0f, (float)maxz);

	MeshPolygon* sceneRight = CreatePolygon(3);
	AddPointToPolygon(sceneRight, 0.0f, 0.0f);
	AddPointToPolygon(sceneRight, (float)maxx, 0.0f);
	AddPointToPolygon(sceneRight, (float)maxx, maxz);

	Circle* leftCircle = (Circle*)malloc(sizeof(Circle));
	BuildPolygonCircle(leftCircle, sceneLeft);
	SetPolygonUserData(sceneLeft, (void*)leftCircle);

	Circle* rightCircle = (Circle*)malloc(sizeof(Circle));
	BuildPolygonCircle(rightCircle, sceneRight);
	SetPolygonUserData(sceneRight, (void*)rightCircle);

	Queue* walkablePolygQueue = CreateQueue(128);
	PushDataToQueue(walkablePolygQueue, sceneLeft);
	PushDataToQueue(walkablePolygQueue, sceneRight);

	int pointCount = GetDataCountFromQueue(sys->m_test);
	for (int i = 0; i < pointCount; ++i)
	{
		Point* p = (Point*)GetDataFromQueueByIndex(sys->m_test, i);
		AddPointToTriangleList(walkablePolygQueue, p, NULL, true);
	}

	int resultcount = GetDataCountFromQueue(walkablePolygQueue);
	for (int i = 0; i < resultcount; ++i)
	{
		MeshPolygon* p = (MeshPolygon*)GetDataFromQueueByIndex(walkablePolygQueue, i);
		free(GetPolygonUserData(p));
	}
	return walkablePolygQueue;
}