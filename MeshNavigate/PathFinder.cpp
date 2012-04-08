/*
**文件名称：PathFinder.cpp
**功能描述：网格寻路基础数据结构
**文件说明：
**作    者：陈琳
**创建时间：2012-02-21
**修    改：
*/

#include "stdafx.h"
#include "PathFinder.h"
#include "Heap.h"
#include "BaseStruct.h"
#include "Polygon.h"
#include "VectorMath.h"
#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include <math.h>

struct UBI_MNHeap;

struct UBI_MNZone
{
	UBI_MNPolygon**	  	m_gridList;
	int					m_maxGridListCount;
	int					m_zoneID;
};

static const int MN_INVALID_VALUE = -1;

struct UBI_MeshNavigateSystem
{
	UBI_MNPoint*	m_pointList;
	int				m_pointCount;
	int				m_sceneMaxX;
	int				m_sceneMaxZ;
	int				m_xZoneCount;
	int				m_zZoneCount;
	int				m_zoneSize;
	UBI_MNZone*		m_gridZoneList;
	int				m_zoneCount;
	int				m_gridZoneCount;	
	UBI_MNPolygon*	m_polygonLIst;
	int				m_triangleCount;
	UBI_MNHeap*		m_heap;
};

//所有的点都是顺时针的顺序
static void CalculateNormalVector(UBI_MNVector2D* normalVector, const UBI_MNPoint* beginPoint, const UBI_MNPoint* endPoint)
{
	normalVector->m_x = beginPoint->m_z - endPoint->m_z;
	normalVector->m_z = endPoint->m_x - beginPoint->m_x;
}

static bool IsInTriangle(const UBI_MeshNavigateSystem* sys, const UBI_MNPolygon* polygon, const UBI_MNPoint* point)
{
	UBI_MNVector2D vector;
	for(int i = 0; i < polygon->m_pointCount; ++i)
	{
		vector.m_x = point->m_x - sys->m_pointList[polygon->m_pointIndex[i]].m_x;
		vector.m_z = point->m_z - sys->m_pointList[polygon->m_pointIndex[i]].m_z;
		float cos = vector.m_x * polygon->m_normalVectors[i].m_x + vector.m_z * polygon->m_normalVectors[i].m_z;
		if(cos < 0)
		{
			return false;
		}
	}
	return true;
}

inline static void InitTriangle(UBI_MeshNavigateSystem* sys, UBI_MNPolygon* polygon, const int* pointIndexList, const UBI_MNVector2D* normalLine, const int pointCount, int specialLogic)
{
	polygon->m_pointCount = pointCount;
	polygon->m_pointIndex = (int*)malloc(sizeof(int) * pointCount);
	memcpy(polygon->m_pointIndex, pointIndexList, sizeof(polygon->m_pointIndex[0]) * pointCount);;
	polygon->m_triangleSpecalLogic = specialLogic;
	polygon->m_normalVectors = (UBI_MNVector2D*)malloc(sizeof(UBI_MNVector2D) * polygon->m_pointCount);
	memcpy(polygon->m_normalVectors, normalLine, sizeof(polygon->m_normalVectors[0]) * pointCount);;
}

static UBI_MNPolygon* GetTriangleByPosition(const UBI_MeshNavigateSystem* sys, UBI_MNPoint* point)
{
	if (point->m_x > sys->m_sceneMaxX)
	{
		return NULL;
	}

	if (point->m_z > sys->m_sceneMaxZ)
	{
		return NULL;
	}

	int xIndex = (int)(point->m_x / (float)sys->m_zoneSize);
	if (xIndex >= sys->m_xZoneCount)
	{
		return NULL;
	}

	int zIndex = (int)(point->m_z / (float)sys->m_zoneSize);
	if (zIndex >= sys->m_zZoneCount)
	{
		return NULL;
	}

	UBI_MNZone* zone = &sys->m_gridZoneList[xIndex + zIndex * sys->m_zZoneCount];
	if (NULL == zone->m_gridList)
	{
		return NULL;
	}

	UBI_MNPolygon* result = NULL;
	for(int i = 0; i < zone->m_maxGridListCount; ++i)
	{
		if (IsInTriangle(sys, zone->m_gridList[i], point))
		{
			result = zone->m_gridList[i];
			break;
		}
	}
	return result;
}

inline static void InitPathFindData(UBI_MeshNavigateSystem* sys)
{
	for(int i = 0; i < sys->m_triangleCount; ++i)
	{	
		memset(&sys->m_polygonLIst[i].m_pathFindData, 0, sizeof(UBI_PathFindData));
		sys->m_polygonLIst[i].m_pathFindData.m_parentToSelfAdjacentIndex = MN_INVALID_VALUE;
	}
}

inline static void UpdateTrianglePortalPosition(UBI_MeshNavigateSystem* sys, UBI_MNPolygon* frompolygon, int bondaryIndex, UBI_MNPolygon* toPolygon)
{
	int leftPointIndex = MN_INVALID_VALUE;
	int rightPointIndex = MN_INVALID_VALUE;
	if (frompolygon->m_pointCount - 1 != bondaryIndex)
	{
		leftPointIndex = frompolygon->m_pointIndex[bondaryIndex];
		rightPointIndex = frompolygon->m_pointIndex[bondaryIndex + 1];
	}
	else
	{
		leftPointIndex = frompolygon->m_pointIndex[bondaryIndex];
		rightPointIndex = frompolygon->m_pointIndex[0];
	}
	toPolygon->m_pathFindData.m_pos.m_x = (sys->m_pointList[leftPointIndex].m_x + sys->m_pointList[rightPointIndex].m_x) * 0.5f;
	toPolygon->m_pathFindData.m_pos.m_z = (sys->m_pointList[leftPointIndex].m_z + sys->m_pointList[rightPointIndex].m_z) * 0.5f;
}

#define Distence(a, b) ((a.m_x - b.m_x) * (a.m_x - b.m_x) + (a.m_z - b.m_z) * (a.m_z - b.m_z))

#define GetCost(from, to) Distence(from->m_pathFindData.m_pos, to->m_pathFindData.m_pos)

UBI_MeshNavigateSystem* CreatePathFinder()
{
	UBI_MeshNavigateSystem* sys = (UBI_MeshNavigateSystem*)malloc(sizeof(UBI_MeshNavigateSystem));
	return sys;
}

#define __CLIENT__

inline static void ReleaseZoneList(UBI_MeshNavigateSystem* sys)
{
	for (int i = 0; i < sys->m_gridZoneCount; ++i)
	{
		free(sys->m_gridZoneList[i].m_gridList);
	}
	free(sys->m_gridZoneList);
}

bool InitPathFinder(UBI_MeshNavigateSystem* sys, const char* configFile)
{
	if(NULL == sys || NULL == configFile)
	{
		return false;
	}
#ifdef __CLIENT__
	FILE* f = NULL;
	fopen_s(&f, configFile, "rb");
#else
	FILE* f = fopen(configFile, "rb");
#endif
	if(NULL == f)
	{
		return false;
	}

	//读取版本号
	int version = 0;
	fread(&version, sizeof(version), 1, f);

	//读取场景数据
	fread(&sys->m_sceneMaxX, sizeof(sys->m_sceneMaxX), 1, f);
	fread(&sys->m_sceneMaxZ, sizeof(sys->m_sceneMaxZ), 1, f);
	fread(&sys->m_zoneSize, sizeof(sys->m_zoneSize), 1, f);
	sys->m_xZoneCount = (sys->m_sceneMaxX / sys->m_zoneSize);
	sys->m_zZoneCount = (sys->m_sceneMaxZ / sys->m_zoneSize);
	sys->m_zoneCount = sys->m_xZoneCount * sys->m_zZoneCount;
	//读取Zone数据
	sys->m_gridZoneList = (UBI_MNZone*)malloc(sizeof(UBI_MNZone)* sys->m_zoneCount);
	memset(sys->m_gridZoneList, 0, sizeof(UBI_MNZone) * sys->m_zoneCount);


	int pointCount = 0;
	fread(&pointCount, sizeof(pointCount), 1, f);
	if(pointCount <= 0)
	{
		fclose(f);
		ReleaseZoneList(sys);
		return false;
	}

	//读取顶点数据
	sys->m_pointList = (UBI_MNPoint*)malloc(sizeof(UBI_MNPoint) *pointCount);
	sys->m_pointCount = pointCount;
	fread(sys->m_pointList, sizeof(UBI_MNPoint) * pointCount, 1, f);

	//读取triangle数据
	int polygonCount = 0;
	fread(&polygonCount, sizeof(polygonCount), 1, f);
	if(polygonCount <= 0)
	{
		fclose(f);
		ReleaseZoneList(sys);
		free(sys->m_pointList);
		return false;
	}

	sys->m_polygonLIst = (UBI_MNPolygon*)malloc(sizeof(UBI_MNPolygon) * polygonCount);
	sys->m_triangleCount = polygonCount;
	int trianglePointIndex[32];
	UBI_MNVector2D normalLineData[32];
	int SpecalLogic;
	int registedZoneCount = 0;
	int ZoneID = 0;

	for(int i = 0; i < polygonCount; ++i)
	{	
		//读取顶点数据,特殊逻辑,用以初始化triangle
		int pointPolygonPointCount;
		fread(&pointPolygonPointCount, sizeof(pointPolygonPointCount), 1, f);
		fread(trianglePointIndex, sizeof(trianglePointIndex[0]) * pointPolygonPointCount, 1, f);
		fread(normalLineData, sizeof(normalLineData[0]) * pointPolygonPointCount, 1, f);
		fread(&SpecalLogic, sizeof(SpecalLogic), 1, f);
		sys->m_polygonLIst[i].m_id = i;
		InitTriangle(sys, &sys->m_polygonLIst[i], trianglePointIndex, normalLineData, pointPolygonPointCount, SpecalLogic);
	}

	//按照统计数据分配空间存储需要在该zone注册的triangle
	for (int i = 0; i < sys->m_zoneCount; ++i)
	{
		fread(&sys->m_gridZoneList[i].m_maxGridListCount, sizeof(sys->m_gridZoneList[i].m_maxGridListCount), 1, f);
		if(0 != sys->m_gridZoneList[i].m_maxGridListCount)
		{
			sys->m_gridZoneList[i].m_gridList = (UBI_MNPolygon**)malloc(sizeof(UBI_MNPolygon**) * sys->m_gridZoneList[i].m_maxGridListCount);
			memset(sys->m_gridZoneList[i].m_gridList, 0, sizeof(UBI_MNPolygon**) * sys->m_gridZoneList[i].m_maxGridListCount);
			int polygonID = -1;
			for (int t = 0; t < sys->m_gridZoneList[i].m_maxGridListCount; ++t)
			{
				fread(&polygonID, sizeof(polygonID), 1, f);
				sys->m_gridZoneList[i].m_gridList[t] = &sys->m_polygonLIst[polygonID];
			}
		}
		else
		{
			sys->m_gridZoneList[i].m_gridList = NULL;
		}
		sys->m_gridZoneList[i].m_zoneID = i;
	}


	//读取polygon的相邻数据
	int	adjacentpolygonIndex[32];
	for(int i = 0; i < sys->m_triangleCount; ++i)
	{
		fread(adjacentpolygonIndex, sizeof(adjacentpolygonIndex[0]) * sys->m_polygonLIst[i].m_pointCount, 1, f);
		sys->m_polygonLIst[i].m_adjacentPolygon = (UBI_MNPolygon**)malloc(sizeof(UBI_MNPolygon) * sys->m_polygonLIst[i].m_pointCount);
		for(int t = 0; t < sys->m_polygonLIst[i].m_pointCount; ++t)
		{
			if(MN_INVALID_VALUE == adjacentpolygonIndex[t])
			{
				sys->m_polygonLIst[i].m_adjacentPolygon[t] = NULL;
			}
			else if(adjacentpolygonIndex[t] >=0 && adjacentpolygonIndex[t] < sys->m_triangleCount)
			{
				sys->m_polygonLIst[i].m_adjacentPolygon[t] = &sys->m_polygonLIst[adjacentpolygonIndex[t]];
			}
			else
			{

				fclose(f);
				ReleaseZoneList(sys);
				free(sys->m_pointList);
				free(sys->m_polygonLIst);
				return false;
			}
		}
	}
	sys->m_heap = CreateHeap(512);
	return true;
}

void ReleasePathFinder( UBI_MeshNavigateSystem* sys )
{
	if (NULL == sys)
	{
		return;
	}
	ReleaseHeap(sys->m_heap);
	ReleaseZoneList(sys);
	free(sys->m_pointList);
	free(sys->m_polygonLIst);
	free(sys);
}

const int IN_OPEN_LIST = 1;
const int IN_CLOSE_LIST = 2;

static int XXXXXXXX(UBI_MeshNavigateSystem* sys, const UBI_MNPoint* pathPoint, const int leftPoint, const int rightPoint, const int newLeftPoint, const int newRightPoint)
{
	static MeshPolygon* testPolygon = CreatePolygon(3);
	CleanPolygon(testPolygon);
	Vector pathPointToLeftPoint = {sys->m_pointList[newLeftPoint].m_x - pathPoint->m_x, 0.0f, sys->m_pointList[newLeftPoint].m_z- pathPoint->m_z};
	Vector leftToRight = {sys->m_pointList[newRightPoint].m_x - sys->m_pointList[newLeftPoint].m_x, 0.0f, sys->m_pointList[newRightPoint].m_z - sys->m_pointList[newLeftPoint].m_z};
	Vector rormalLineForLeftToRight;
	VectorCrossProduct(&rormalLineForLeftToRight, &leftToRight, &StandardVector);
	if (VectorDotProduct(&rormalLineForLeftToRight, &pathPointToLeftPoint) > 0)
	{
		AddPointToPolygon(testPolygon, sys->m_pointList[newLeftPoint].m_x, sys->m_pointList[newLeftPoint].m_z);
		AddPointToPolygon(testPolygon, sys->m_pointList[newRightPoint].m_x, sys->m_pointList[newRightPoint].m_z);
		AddPointToPolygon(testPolygon, pathPoint->m_x, pathPoint->m_z);
	}
	else
	{
		AddPointToPolygon(testPolygon, sys->m_pointList[newLeftPoint].m_x, sys->m_pointList[newLeftPoint].m_z);
		AddPointToPolygon(testPolygon, pathPoint->m_x, pathPoint->m_z);
		AddPointToPolygon(testPolygon, sys->m_pointList[newRightPoint].m_x, sys->m_pointList[newRightPoint].m_z);
	}


	int leftPointInPolygonTest = PointInPolygon(testPolygon, sys->m_pointList[leftPoint].m_x, sys->m_pointList[leftPoint].m_z);
	int rightPointInPolygonTest = PointInPolygon(testPolygon, sys->m_pointList[rightPoint].m_x, sys->m_pointList[rightPoint].m_z);
	if (leftPointInPolygonTest >= 0 && rightPointInPolygonTest >=0)
	{
		return MN_INVALID_VALUE;
	}
	else if (leftPointInPolygonTest < 0 && rightPointInPolygonTest >= 0)
	{
		return rightPoint;
	}

	else if (leftPointInPolygonTest >= 0 && rightPointInPolygonTest < 0)
	{
		return leftPoint;
	}

	//同时都在polygon外
	float distenceLeftToNewLift = Distence(sys->m_pointList[leftPoint], sys->m_pointList[newLeftPoint]);
	float distenceLeftToNewright = Distence(sys->m_pointList[rightPoint], sys->m_pointList[newLeftPoint]);
	if (leftPoint < rightPoint)
	{
		return leftPoint;
	}
	else
	{
		return rightPoint;
	}
	return MN_INVALID_VALUE;
}

int findPath(UBI_MeshNavigateSystem* sys, float* pathBuffer, const int bufferSize, const float beginPosX, const float beginPosZ, const float targetX, const float targetZ)
{
	UBI_MNPoint begin;
	begin.m_x = beginPosX;
	begin.m_z = beginPosZ;
	UBI_MNPolygon* beginTriangle = GetTriangleByPosition(sys, &begin);
	if (NULL == beginTriangle)
	{
		return PathFindErrorBeginPositionNoInMesh;
	}

	UBI_MNPoint targetPoint;
	targetPoint.m_x = targetX;
	targetPoint.m_z = targetZ;
	UBI_MNPolygon* targetTriangle = GetTriangleByPosition(sys, &targetPoint);
	if (NULL == targetTriangle)
	{
		return PathFindErrorTargetPositionNoInMesh;
	}

	if (beginTriangle == targetTriangle)
	{

		return true;
	}

	InitPathFindData(sys);
	beginTriangle->m_pathFindData.m_parent = NULL;
	beginTriangle->m_pathFindData.m_pos = begin;
	beginTriangle->m_pathFindData.m_flag = IN_OPEN_LIST;
	beginTriangle->m_pathFindData.m_totalCost = Distence(begin, targetPoint);
	MNHeapClear(sys->m_heap);
	MNHeapPush(sys->m_heap, beginTriangle);
	UBI_MNPolygon* lastBestTriangle = NULL;
	float lastBestCost = beginTriangle->m_pathFindData.m_totalCost;
	//A*寻路, 每个mesh的加权值为三部分之和
	//1: 父mesh的加权值
	//2: 父mesh到自己的加权值
	//3: 自己到最终目标的加权值
	//每一个加权值都是两点之间的距离
	//但是为了减少计算了,没有进行sqrt的计算
	//如果后期计算需要计算如草地,河流等逻辑数据对
	//路径的印象,可以修改相关计算方法
	//目前所有的加权计算都放在了
	//Distence 和 GetCost两个宏内
	while (!MNHeapEmpty(sys->m_heap))
	{
		UBI_MNPolygon* bestPolygon = MNHeapPop(sys->m_heap);
		bestPolygon->m_pathFindData.m_flag &= ~IN_OPEN_LIST;
		bestPolygon->m_pathFindData.m_flag |= IN_CLOSE_LIST;

		if (bestPolygon == targetTriangle)
		{
			lastBestTriangle = bestPolygon;
			break;
		}

		for(int i = 0; i < bestPolygon->m_pointCount; ++i)
		{
			if(NULL == bestPolygon->m_adjacentPolygon[i])
			{
				continue;
			}

			if(bestPolygon->m_pathFindData.m_parent == bestPolygon->m_adjacentPolygon[i])
			{
				continue;
			}

			UBI_MNPolygon* neighbour = bestPolygon->m_adjacentPolygon[i];
			if(0 == neighbour->m_pathFindData.m_flag)
			{
				UpdateTrianglePortalPosition(sys, bestPolygon, i, neighbour);
			}

			float cost = 0.0f;
			float heuristic = 0.0f;

			cost = GetCost(bestPolygon, neighbour);
			cost += bestPolygon->m_pathFindData.m_totalCost;
			heuristic = Distence(neighbour->m_pathFindData.m_pos, targetPoint);

			const float total = cost + heuristic;

			if (neighbour->m_pathFindData.m_flag & IN_OPEN_LIST || neighbour->m_pathFindData.m_flag & IN_CLOSE_LIST)
			{
				if (total >= neighbour->m_pathFindData.m_totalCost)
				{
					continue;
				}
			}

			neighbour->m_pathFindData.m_totalCost = total;
			neighbour->m_pathFindData.m_parent = bestPolygon;
			neighbour->m_pathFindData.m_parentToSelfAdjacentIndex = i;
			if(neighbour->m_pathFindData.m_flag & IN_OPEN_LIST)
			{
				MNHeapModify(sys->m_heap, neighbour, true);
			}
			else
			{
				neighbour->m_pathFindData.m_flag |= IN_OPEN_LIST;
				MNHeapPush(sys->m_heap, neighbour);
			}

			if (lastBestCost > heuristic)
			{
				lastBestCost = heuristic;
				lastBestTriangle = neighbour;
			}
		}
	}

	if (lastBestTriangle != targetTriangle)
	{
		return PathFindErrorTargetPositionUnarrivable;
	}

	//处理寻路结果的链表
	lastBestTriangle->m_pathFindData.m_pathFindList = NULL;
	while (lastBestTriangle->m_pathFindData.m_parent)
	{
		lastBestTriangle->m_pathFindData.m_parent->m_pathFindData.m_pathFindList = lastBestTriangle;
		lastBestTriangle = lastBestTriangle->m_pathFindData.m_parent;
	}
	UBI_MNPolygon* const pBegin = lastBestTriangle;
	//路点平滑计算
	//目前没有做平滑计算
	int meshCount = 1;
	UBI_MNPoint currentPathPoint = lastBestTriangle->m_pathFindData.m_pos;
	int outAdjacentIndex = lastBestTriangle->m_pathFindData.m_pathFindList->m_pathFindData.m_parentToSelfAdjacentIndex;
	int leftPointIndex = lastBestTriangle->m_pointIndex[outAdjacentIndex];
	int rightPointIndex = lastBestTriangle->m_pointIndex[outAdjacentIndex + 1 != lastBestTriangle->m_pointCount ? outAdjacentIndex + 1 : 0];
	while(lastBestTriangle)
	{
		//通过下一个路径多边形得到出口边
		if (NULL == lastBestTriangle->m_pathFindData.m_pathFindList)
		{
			//添加最后一个路点
			break;
		}

		outAdjacentIndex = lastBestTriangle->m_pathFindData.m_pathFindList->m_pathFindData.m_parentToSelfAdjacentIndex;
		int newLeftPointIndex = lastBestTriangle->m_pointIndex[outAdjacentIndex];
		int newRightPointIndex = lastBestTriangle->m_pointIndex[outAdjacentIndex + 1 != lastBestTriangle->m_pointCount ? outAdjacentIndex + 1 : 0];

		int result = XXXXXXXX(sys, &currentPathPoint, leftPointIndex, rightPointIndex, newLeftPointIndex, newRightPointIndex);
		if (MN_INVALID_VALUE != result)
		{
			//添加当前路点
			++meshCount;
		}
		leftPointIndex = newLeftPointIndex;
		rightPointIndex = newRightPointIndex;
		lastBestTriangle = lastBestTriangle->m_pathFindData.m_pathFindList;
	}
	//检查是否有足够的空间存放路点
	if(bufferSize < meshCount << 1)
	{
		//没有足够的空间存放路点
		return meshCount << 1;
	}

	lastBestTriangle = pBegin;
	meshCount = 1;
	currentPathPoint = lastBestTriangle->m_pathFindData.m_pos;
	outAdjacentIndex = lastBestTriangle->m_pathFindData.m_pathFindList->m_pathFindData.m_parentToSelfAdjacentIndex;
	leftPointIndex = lastBestTriangle->m_pointIndex[outAdjacentIndex];
	rightPointIndex = lastBestTriangle->m_pointIndex[outAdjacentIndex + 1 != lastBestTriangle->m_pointCount ? outAdjacentIndex + 1 : 0];
	while(lastBestTriangle)
	{
		//通过下一个路径多边形得到出口边
		if (NULL == lastBestTriangle->m_pathFindData.m_pathFindList)
		{
			//添加最后一个路点
			pathBuffer[0] = lastBestTriangle->m_pathFindData.m_pos.m_x;
			pathBuffer[1] = lastBestTriangle->m_pathFindData.m_pos.m_z;
			break;
		}

		outAdjacentIndex = lastBestTriangle->m_pathFindData.m_pathFindList->m_pathFindData.m_parentToSelfAdjacentIndex;
		int newLeftPointIndex = lastBestTriangle->m_pointIndex[outAdjacentIndex];
		int newRightPointIndex = lastBestTriangle->m_pointIndex[outAdjacentIndex + 1 != lastBestTriangle->m_pointCount ? outAdjacentIndex + 1 : 0];

		int result = XXXXXXXX(sys, &currentPathPoint, leftPointIndex, rightPointIndex, newLeftPointIndex, newRightPointIndex);
		if (MN_INVALID_VALUE != result)
		{
			//添加当前路点
			pathBuffer[0] = currentPathPoint.m_x;
			pathBuffer[1] = currentPathPoint.m_z;
			pathBuffer += 2;
			currentPathPoint = sys->m_pointList[result];
			++meshCount;
		}
		leftPointIndex = newLeftPointIndex;
		rightPointIndex = newRightPointIndex;
		lastBestTriangle = lastBestTriangle->m_pathFindData.m_pathFindList;
	}

	return meshCount << 1;
}
