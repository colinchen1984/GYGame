/*
**�ļ����ƣ�MeshNavigate.cpp
**��������������Ѱ·�������ݽṹ
**�ļ�˵����
**��    �ߣ�����
**����ʱ�䣺2012-02-21
**��    �ģ�
*/

#include "stdafx.h"
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

	Point*				pointListForZone;
	int					maxPointCount;
	int					currentPointCount;
	int					beginIndex;
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
	Point*			m_pointList;				// �����б�
	int				m_currentPointCount;		// ��ǰ��������
	int				m_terrianVertexsCount;		// ���ζ����б�����
	int				m_maxPointCount;			// �����б���������

	//������Ϣ
	int				m_sceneMaxX;				// ����X���ֵ
	int				m_sceneMaxZ;				// ����Z���ֵ
	int				m_terriaonVertexSpacing;	// ���εĶ�����

	//Ѱ·�����������Ϣ
	int				m_xZoneCount;				// x��ָ�����������
	int				m_zZoneCount;				// z��ָ�����������
	Zone*			m_gridZoneList;				// ���������б�
	int				m_zoneSize;					// ����Ĵ�С
	int				m_zoneCount;				// �������������������

	int				m_triangleCount;			// �����ڶ��������

	Queue*			m_itemMeshQueue;			// ����ͨ�������õ��Ķ����
	
	AdjacentData*	m_adjacentData;
	int				m_adjacentDataCount;			//
	int				m_polygonCount;

	FILE*			m_logFile;						
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
	xIndex /= sys->m_zoneSize;
	zIndex /= sys->m_zoneSize;
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
			//�õ������εĶ���, ˳ʱ�뷽��
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
			
			//��������ACD�Ƿ��������
			Vector CD = {pointC.x - pointD.x, pointC.y - pointD.y, pointC.z - pointD.z};
			Vector AD = {pointA.x - pointD.x, pointA.y - pointD.y, pointA.z - pointD.z};
			Vector planeNormalACD;
			VectorCrossProduct(&planeNormalACD, NormalizationVector(&CD), NormalizationVector(&AD));
			float cosValue = VectorDotProduct(NormalizationVector(&planeNormalACD), &StandardVector);
			const static float tag = cos(PI / 4);
			bool planeACDWork = abs(cosValue) < tag ? false : true;

			//��������ABC�Ƿ��������
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

  			if (planeABCWork && planeACDWork)
  			{
  				polygon = CreatePolygon(4);
  				AddPointToPolygonByPoint(polygon, &pA);
  				AddPointToPolygonByPoint(polygon, &pB);
  				AddPointToPolygonByPoint(polygon, &pC);
  				AddPointToPolygonByPoint(polygon, &pD);
  			}
			else if(planeACDWork)
			{
				polygon = CreatePolygon(3);
				AddPointToPolygonByPoint(polygon, &pA);
				AddPointToPolygonByPoint(polygon, &pC);
				AddPointToPolygonByPoint(polygon, &pD);
			}
			else if(planeABCWork)
			{
				polygon = CreatePolygon(3);
				AddPointToPolygonByPoint(polygon, &pA);
				AddPointToPolygonByPoint(polygon, &pB);
				AddPointToPolygonByPoint(polygon, &pC);
			}

  			if (NULL != polygon)
  			{
  				// �����ڵ�����ע��
  				int zoneId = GetZoneIDByPosition(sys, pointA.x, pointA.z);
  				assert(zoneId >= 0);
  				PushDataToQueue(sys->m_gridZoneList[zoneId].polygonList, (void*)polygon);
  			}
		}
	}
	ENDPROFILE(sys);
}

static void MakeZoneData(MeshNavigateGenerator* sys)
{
	BEGINPROFILE;
	//��ʼ����������������
	sys->m_zoneSize = 32;
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
			sys->m_gridZoneList[zIndex + x].currentPointCount = 0;
			sys->m_gridZoneList[zIndex + x].beginIndex = 0;
			sys->m_gridZoneList[zIndex + x].maxPointCount = 32;
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

	sys->m_currentPointCount = 0;
	sys->m_maxPointCount = 0;
	sys->m_pointList = NULL;

	sys->m_itemMeshQueue = CreateQueue(128);
	sys->m_logFile = fopen("meshnavigate.log", "wb");
	MakeZoneData(sys);
	MakeTerraionMesh(sys, heightData, heightDataCount);
	ENDPROFILE(sys);
	return 0;
}

static void FreeTerraionMesh(MeshNavigateGenerator* sys)
{
	for (int i = 0; i < sys->m_zoneCount; ++i)
	{
		while(MeshPolygon* polygon = (MeshPolygon*)(PopDataFromQueue(sys->m_gridZoneList[i].polygonList)))
		{
			ReleasePolygon(polygon);
		}
	}
}

static void FreeZone(MeshNavigateGenerator* sys)
{
	for (int i = 0; i < sys->m_zoneCount; ++i)
	{
		ReleaseQueue(sys->m_gridZoneList[i].polygonList);
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
	//�ͷ�����mesh
	while(MeshPolygon* polygon = (MeshPolygon*)PopDataFromQueue(sys->m_itemMeshQueue))
	{
		ReleasePolygon(polygon);
	}
	ReleaseQueue(sys->m_itemMeshQueue);
	
	//�ͷŵ���mesh
	FreeTerraionMesh(sys);
	//�ͷ�zone
	FreeZone(sys);
	//�ͷŶ���
	free(sys->m_pointList);
	//�ͷ�log�ļ�
	fclose(sys->m_logFile);
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

//��������֮��ĵ���
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
			//�����κζ�����ཻ
			//ֱ�ӷ���������
			PushDataToQueue(resultQueue, polygon);
			continue;
		}
		else if(ALL_POINT_IN_CLIPPING_WINDOW == clippResult)
		{
			//���ü������λ�ڲü�������ڲ������Բü�����ȫ��ʧ
			ReleasePolygon(polygon);
			continue;
		}
		else if(CLIPPING_WINDOW_ALL_IN_CLIPPED_POLYGON == clippResult)
		{
			//�ü������λ�ڱ��ü�������ڲ�
			PushDataToQueue(resultQueue, polygon);
			//��Queueɾ����Ҫɾ���Ķ����
			DeleteDataFromQueue(sys->m_itemMeshQueue, checkIndex);
			ReleasePolygon(clppingWindow);
			continue;
		}
		else if(WRONG_PARAM == clippResult)
		{
			//����ĵĲ�������
		}

		assert(0 == clippResult);
		//�ü��ɹ�
		//�Բü�������ÿһ���������������ηֽ�
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

//�����Χ�����͹�����ü���������
static void ProceeZoneMeshWithItemMesh(Zone* zone, const MeshPolygon* itemMesh)
{
	Queue* resultQueue = CreateQueue(GetDataCountFromQueue(zone->polygonList));
	Queue* clippQueue = CreateQueue(GetDataCountFromQueue(zone->polygonList));
	Queue* decomposeQueue = CreateQueue(GetDataCountFromQueue(zone->polygonList));
	while (MeshPolygon* polygon = (MeshPolygon*)PopDataFromQueue(zone->polygonList))
	{
		const MeshPolygon* clppingWindow = itemMesh;
		CleanQueue(clippQueue);
		int clippResult = ConvexPolygonClipping(polygon, clppingWindow, clippQueue);

		if (NOT_NEED_CLIPPING == clippResult)
		{
			//�����κζ�����ཻ
			//ֱ�ӷ���������
			PushDataToQueue(resultQueue, polygon);
			continue;
		}
		else if(ALL_POINT_IN_CLIPPING_WINDOW == clippResult)
		{
			//���ü������λ�ڲü�������ڲ������Բü�����ȫ��ʧ
			ReleasePolygon(polygon);
			continue;
		}
		else if(CLIPPING_WINDOW_ALL_IN_CLIPPED_POLYGON == clippResult)
		{
			//�ü������λ�ڱ��ü�������ڲ�
			PushDataToQueue(resultQueue, polygon);
			continue;
		}
		else if(WRONG_PARAM == clippResult)
		{
			//����ĵĲ�������
		}

		assert(0 == clippResult);
		//�ü��ɹ�
		//�Բü�������ÿһ���������������ηֽ�
		while (MeshPolygon* convexPolygon = (MeshPolygon*)PopDataFromQueue(clippQueue))
		{
			CleanQueue(decomposeQueue);
			int decomposeResult = ConcavePolygonDecompose(convexPolygon, decomposeQueue);
			if (NO_NEED_DECOMPOSE == decomposeResult)
			{
				PushDataToQueue(resultQueue, (void*)convexPolygon);
			}
			else if (0 == decomposeResult)
			{
				ReleasePolygon(convexPolygon);
				ShiftQueueData(resultQueue, decomposeQueue);
			}
			else
			{
				assert(0);
			}
		}
	}

	ShiftQueueData(zone->polygonList, resultQueue);
	ReleaseQueue(resultQueue);
	ReleaseQueue(clippQueue);
	ReleaseQueue(decomposeQueue);
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

//Ϊһ��zone�ڵĶ������Index
static void ProcessVertexInZone(Zone* zone, const Zone* upZone, const Zone* leftZone)
{
	int meshCount = GetDataCountFromQueue(zone->polygonList);
	for (int mesh = 0; mesh < meshCount; ++mesh)
	{
		MeshPolygon* meshPolygon = (MeshPolygon*)GetDataFromQueueByIndex(zone->polygonList, mesh);
		int pointCount = GetPolygonPointCount(meshPolygon);
		Point* pointList = (Point*)GetPolygonPointList(meshPolygon);
		for (int p = 0; p < pointCount; ++p)
		{
			Point* point = &pointList[p];
			int pointID = -1;
			do 
			{
				//���ȳ�����UpZone�ڲ���
				pointID = FindPointInZone(zone, point);
				if (-1 != pointID)
				{
					pointID += zone->beginIndex;
					break;
				}
				//�ٳ�����UpZone�ڲ���
				pointID = FindPointInZone(upZone, point);
				if (-1 != pointID)
				{
					pointID += upZone->beginIndex;
					break;
				}

				//Ȼ������leftZone�ڲ���
				pointID = FindPointInZone(leftZone, point);
				if (-1 != pointID)
				{
					pointID += leftZone->beginIndex;
					break;
				}
				
				//�����ӵ��Լ���zone��
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
				zone->pointListForZone[zone->currentPointCount - 1].userData.floatUserData += 2.0f;
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

//�������ε����ڹ�ϵ
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
		//���е��ڱ��ڴ���ʱ�������index��С������
		//���ﲻ��Ҫ����clockwise����countwiseclock

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
		//�����Ҫ,�����ڴ�
		if (smallIndex >= sys->m_adjacentDataCount)
		{
			int currentCount = sys->m_adjacentDataCount;
			sys->m_adjacentDataCount = currentCount << 1;
			sys->m_adjacentData = (AdjacentData*)realloc(sys->m_adjacentData, sizeof(AdjacentData) * sys->m_adjacentDataCount);
			memset(&sys->m_adjacentData[currentCount], 0, sizeof(sys->m_adjacentData[0]) * currentCount);
		}
		
		AdjacentData* pPointAdjcentData = &sys->m_adjacentData[smallIndex];
		if (0 == pPointAdjcentData->maxDataCount)
		{
			//������η���,�����ڴ�
			pPointAdjcentData->maxDataCount = 4;
			pPointAdjcentData->data = (AdjacentData::_AdjacentData*)(malloc(sizeof(AdjacentData::_AdjacentData) * pPointAdjcentData->maxDataCount));
			memset(pPointAdjcentData->data, 0, sizeof(pPointAdjcentData->data[0]) * pPointAdjcentData->maxDataCount);
			pPointAdjcentData->dataCount = 0;
		}

		bool processed = false;
		for (int dataIndex = 0; dataIndex < pPointAdjcentData->dataCount; ++dataIndex)
		{
			//�ڸ�index��Ӧ�������ڲ����Ƿ����ڱ�
			//����Ѿ�����,��֪ͨ����polygon����ڱ�����
			//���ҽ����ڴ���ΪΪʹ��
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
		
		//����Ƿ���Ҫ�����ڴ�
		if (pPointAdjcentData->dataCount == pPointAdjcentData->maxDataCount)
		{
			pPointAdjcentData->maxDataCount = pPointAdjcentData->maxDataCount << 1;
			pPointAdjcentData->data = (AdjacentData::_AdjacentData*)realloc(pPointAdjcentData->data, sizeof(pPointAdjcentData->data[0]) * pPointAdjcentData->maxDataCount);
			memset(&pPointAdjcentData->data[pPointAdjcentData->dataCount], 0, sizeof(pPointAdjcentData->data[0]) * pPointAdjcentData->dataCount);
		}
		//������ڱ��ǳ��������ֱ�����
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

	for (int z = 0; z < sys->m_zZoneCount; ++z)
	{
		int zStep = z * sys->m_xZoneCount;
		for (int x = 0; x < sys->m_xZoneCount; ++x)
		{
			int zoneID = x + zStep;
			Zone* zone = &sys->m_gridZoneList[zoneID];
			int meshCount = GetDataCountFromQueue(zone->polygonList);
			for (int mesh = 0; mesh < meshCount; ++mesh)
			{
				MeshPolygon* meshPolygon = (MeshPolygon*)GetDataFromQueueByIndex(zone->polygonList, mesh);
				ProcessAdjacent(sys, meshPolygon);
			}
		}
	}

	for (int i = 0; i < sys->m_adjacentDataCount; ++i)
	{
		free(sys->m_adjacentData[i].data);
	}
	free(sys->m_adjacentData);
	ENDPROFILE(sys);
	return true;
}


bool MakeMeshNavigateData(MeshNavigateGenerator* sys)
{
	//�ü�, �ֽ���Ʒ͹��
	if(true != UpdateItemPolygon(sys))
	{
		return false;
	}
	
	//���ÿһ����Ʒmesh
	//���mesh����һ����ĳһ������,���ø���Ʒmesh�ü����������еĵ���mesh
	//���������zone�㹻��,�������һ��zone��ȫ��һ��item ��͹���ڵ����
	//��������������,�ᵼ�¸�zone�ڵ����е�terrian��������ڵ�item ͹�����вü�
	Queue* pItemQueue = CreateQueue(GetDataCountFromQueue(sys->m_itemMeshQueue));
	while(MeshPolygon* itemMesh = (MeshPolygon*)PopDataFromQueue(sys->m_itemMeshQueue))
	{
		const Point* itemPointList = GetPolygonPointList(itemMesh);
		const int itemPointCount = GetPolygonPointCount(itemMesh);
		for (int zoneID = 0; zoneID < sys->m_zoneCount; ++zoneID)
		{
			for (int i = 0; i < itemPointCount; ++i)
			{
				if(InRect(&sys->m_gridZoneList[zoneID].zoneRect, &itemPointList[i]))
				{
					//���ø���Ʒ mesh �����zone�����еĶ����
					ProceeZoneMeshWithItemMesh(&sys->m_gridZoneList[zoneID], itemMesh);
					break;
				}
			}
		}
		PushDataToQueue(pItemQueue, (void*)itemMesh);
	}

	ShiftQueueData(sys->m_itemMeshQueue, pItemQueue);

	ReleaseQueue(pItemQueue);
	//�����еĵ�����굼�붨���б�,������index
	ProcessVertex(sys);

	//���ɶ���ε����ڹ�ϵ
	BuildEdgeInfo(sys);

	//���еĶ��������ID
	int polygonID = 0;
	for (int z = 0; z < sys->m_zZoneCount; ++z)
	{
		int zStep = z * sys->m_xZoneCount;
		for (int x = 0; x < sys->m_xZoneCount; ++x)
		{
			int zoneID = x + zStep;
			Zone* zone = &sys->m_gridZoneList[zoneID];
			int meshCount = GetDataCountFromQueue(zone->polygonList);
			for (int mesh = 0; mesh < meshCount; ++mesh)
			{
				MeshPolygon* meshPolygon = (MeshPolygon*)GetDataFromQueueByIndex(zone->polygonList, mesh);
				SetPolygonUserData(meshPolygon, (void*)polygonID++);
			}
		}
	}
	sys->m_polygonCount = polygonID;

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

extern int GetMeshCountInZone(MeshNavigateGenerator* sys, int index)
{
	if (NULL == sys || index >= sys->m_zoneCount)
	{
		return 0;
	}

	return GetDataCountFromQueue(sys->m_gridZoneList[index].polygonList);
}

MeshPolygon* GetMeshPolygonInZone(MeshNavigateGenerator* sys, int zoneIndex, int meshIndex)
{
	if (NULL == sys || zoneIndex >= sys->m_zoneCount)
	{
		return NULL;
	}

	int meshCountInZone = GetDataCountFromQueue(sys->m_gridZoneList[zoneIndex].polygonList);
	if (meshIndex >= meshCountInZone)
	{
		return NULL;
	}

	return (MeshPolygon*)GetDataFromQueueByIndex(sys->m_gridZoneList[zoneIndex].polygonList, meshIndex);
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

	//�汾��
	int version = 0;
	fwrite(&version, sizeof(version), 1, file);

	//��������
	fwrite(&sys->m_sceneMaxX, sizeof(sys->m_sceneMaxX), 1, file);
	fwrite(&sys->m_sceneMaxZ, sizeof(sys->m_sceneMaxZ), 1, file);
	fwrite(&sys->m_zoneSize, sizeof(sys->m_zoneSize), 1, file);
	
	//zone ����
	for (int z = 0; z < sys->m_zZoneCount; ++z)
	{
		int zStep = z * sys->m_xZoneCount;
		for (int x = 0; x < sys->m_xZoneCount; ++x)
		{
			int zoneID = x + zStep;
			Zone* zone = &sys->m_gridZoneList[zoneID];
			int polygonCountInZone = GetDataCountFromQueue(zone->polygonList);
			fwrite(&polygonCountInZone, sizeof(polygonCountInZone), 1, file);
		}
	}

	//���������
	fwrite(&sys->m_currentPointCount, sizeof(sys->m_currentPointCount), 1, file);
	for (int i = 0; i < sys->m_currentPointCount; ++i)
	{
		fwrite(&sys->m_pointList[i].x, sizeof(sys->m_pointList[i].x), 1, file);
		fwrite(&sys->m_pointList[i].z, sizeof(sys->m_pointList[i].z), 1, file);
	}

	//polygon����
	fwrite(&sys->m_polygonCount, sizeof(sys->m_polygonCount), 1, file);
	int id = -1;
	for (int z = 0; z < sys->m_zZoneCount; ++z)
	{
		int zStep = z * sys->m_xZoneCount;
		for (int x = 0; x < sys->m_xZoneCount; ++x)
		{
			int zoneID = x + zStep;
			Zone* zone = &sys->m_gridZoneList[zoneID];
			int meshCount = GetDataCountFromQueue(zone->polygonList);
			for (int mesh = 0; mesh < meshCount; ++mesh)
			{
				MeshPolygon* meshPolygon = (MeshPolygon*)GetDataFromQueueByIndex(zone->polygonList, mesh);
				assert(++id == (int)GetPolygonUserData(meshPolygon));
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
				int zoneCount = 1;
				fwrite(&zoneCount, sizeof(zoneCount), 1, file);
				fwrite(&zoneID, sizeof(zoneID), 1, file);
			}
		}
	}


	//��������
	id = -1;
	for (int z = 0; z < sys->m_zZoneCount; ++z)
	{
		int zStep = z * sys->m_xZoneCount;
		for (int x = 0; x < sys->m_xZoneCount; ++x)
		{
			int zoneID = x + zStep;
			Zone* zone = &sys->m_gridZoneList[zoneID];
			int meshCount = GetDataCountFromQueue(zone->polygonList);
			for (int mesh = 0; mesh < meshCount; ++mesh)
			{
				MeshPolygon* meshPolygon = (MeshPolygon*)GetDataFromQueueByIndex(zone->polygonList, mesh);
				assert(++id == (int)GetPolygonUserData(meshPolygon));
				int pointCount = GetPolygonPointCount(meshPolygon);
				for (int p = 0; p < pointCount; ++p)
				{
					MeshPolygon* adjacentPolygon = GetAdjacentData(meshPolygon, p);
					int adjacentPolygonID = NULL != adjacentPolygon ? (int)GetPolygonUserData(adjacentPolygon) : -1;
					fwrite(&adjacentPolygonID, sizeof(adjacentPolygonID), 1, file);
				}
			}
		}
	}
	fclose(file);
	ENDPROFILE(sys);
	return true;
}
