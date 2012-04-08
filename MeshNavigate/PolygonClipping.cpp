/*
**文件名称：PolygonClipping.cpp
**功能描述：
**文件说明：
**作    者：陈琳
**创建时间：2012-02-21
**修    改：
*/


#include "PolygonClipping.h"
#include "Polygon.h"
#include "BaseStruct.h"
#include "VectorMath.h"
#include <malloc.h>
#include <assert.h>
#include <memory.h>
#include <math.h>

enum INTER_POINT_TYPE
{
	INTER_POINT_TYPE_CLIPPED_PYLIGON_POINT = 0,
	INTER_POINT_TYPE_CLIPPING_WINDOW_POINT,
	INTER_POINT_TYPE_CLIPPING_WINDOW_POINT_AN_INSERCTION_POINT,
	INTER_POINT_TYPE_INTERSECTION_POINT_FOR_IN,
	INTER_POINT_TYPE_INTERSECTION_POINT_FOR_OUT,
};

enum PROCESS_STATE
{
	PROCESS_STATE_UNPROCESSED = 0,
	PROCESS_STATE_NO_NEED_PROCESS,
	PROCESS_STATE_BEGIN_ADD,
	PROCESS_STATE_ADDED_TO_POLYGON,
};

struct ClippingInterPoint
{
	Point point;
	int pointType;
	ClippingInterPoint* prev2ClippedPolygon;
	ClippingInterPoint* next2ClippedPolygon;
	float				tValueForClippedPolygon;
	ClippingInterPoint* prev2ClippingdWindow;
	ClippingInterPoint* next2ClippingdWindow;
	float				tValueForClippingWindow;
	PROCESS_STATE state;
};
static int GenerateConvexPolygon(ClippingInterPoint* pointList, const int pointCount, Queue* queue, int* bufferForPolygonConverCount, int bufferCount);
static ClippingInterPoint* CreateClippedPolygonData(const MeshPolygon* clippedPolygon, const MeshPolygon* clippingWindow);
static ClippingInterPoint* CreateClippingWindowData(const MeshPolygon* clippedPolygon, const MeshPolygon* clippingWindow);
float GetTValue( const Point* beginPoint, const Point* endPoint, const Point* middlePoint );

int ConvexPolygonClipping(const MeshPolygon* clippedPolygon,
								  const MeshPolygon* clippingWindow,
								  Queue* queue)
{
	if(NULL == clippedPolygon || NULL == clippingWindow || NULL == queue)
	{
		return WRONG_PARAM;
	}
	int allPointCountForClipping = 0;
	const Point* pointListForClipingWindow  = GetPolygonPointList(clippingWindow);
	const int pointCountForClippingWindow  = GetPolygonPointCount(clippingWindow);
	allPointCountForClipping += pointCountForClippingWindow;
	int clipingWindowPointInClippedWindowCount[3] = {0};
	for (int i = 0; i < pointCountForClippingWindow; ++i)
	{
		int checkResult = PointInPolygon(clippedPolygon, pointListForClipingWindow[i].x, pointListForClipingWindow[i].z);
		++clipingWindowPointInClippedWindowCount[checkResult + 1];
	}

	//裁剪多边形的顶点都位于被裁剪多边形的内部或者边上
	//目前无法裁剪这种情况
	if (0 == clipingWindowPointInClippedWindowCount[0])
	{
		return CLIPPING_WINDOW_ALL_IN_CLIPPED_POLYGON;
	}


	const Point* pointListForClippedPolygon = GetPolygonPointList(clippedPolygon);
	const int pointCountForClippedPolygon = GetPolygonPointCount(clippedPolygon);
	int clippedPolygonointInClippedWindowCount[3] = {0};
	for (int i = 0; i < pointCountForClippedPolygon; ++i)
	{
		int checkResult = PointInPolygon(clippingWindow, pointListForClippedPolygon[i].x, pointListForClippedPolygon[i].z);
		++clippedPolygonointInClippedWindowCount[checkResult + 1];
	}

	allPointCountForClipping += pointCountForClippedPolygon;
	//被裁剪的多边形的顶点全部为与裁剪多边形内或边上
	//不需要参见
	if (0 == clippedPolygonointInClippedWindowCount[0])
	{
		return ALL_POINT_IN_CLIPPING_WINDOW;
	}


	//两多边形不相交 不需要裁剪
	if (0 == clipingWindowPointInClippedWindowCount[2] && 0 == clippedPolygonointInClippedWindowCount[2])
	{
		return NOT_NEED_CLIPPING;
	}
	ClippingInterPoint* pointChainForClippingWindow = CreateClippingWindowData(clippedPolygon, clippingWindow);
	ClippingInterPoint* pointChainForClippedPolygon = CreateClippedPolygonData(clippedPolygon, clippingWindow);

	//查找交点并将交点插入正确的位置
	const Vector* const pNormalOfClippedPolygon = GetPolygonNormal(clippedPolygon);
	Rect checkRect;
	for (int indexOfClippingWindow = 0; indexOfClippingWindow < pointCountForClippingWindow; ++indexOfClippingWindow)
	{
		int nextIndexOfClippingWindow = indexOfClippingWindow + 1 == pointCountForClippingWindow ? 0 : indexOfClippingWindow + 1;

		for (int indexOfclippedPolygon = 0; indexOfclippedPolygon < pointCountForClippedPolygon; ++indexOfclippedPolygon)
		{
			int nextIndexOfClippedPolygon = indexOfclippedPolygon + 1 == pointCountForClippedPolygon ? 0 : indexOfclippedPolygon + 1;

			ClippingInterPoint* const pBeginPointOfClippinWindow = &pointChainForClippingWindow[indexOfClippingWindow];
			ClippingInterPoint* const pEndPointOfClippinWindow = &pointChainForClippingWindow[nextIndexOfClippingWindow];

			ClippingInterPoint* const pBeginPointOfClippedPolygon = &pointChainForClippedPolygon[indexOfclippedPolygon];
			ClippingInterPoint* const pEndPointOfClippedPolygon = &pointChainForClippedPolygon[nextIndexOfClippedPolygon];
			Vector toBeginPoint = {pBeginPointOfClippinWindow->point.x - pBeginPointOfClippedPolygon->point.x,
									0.0f,
									pBeginPointOfClippinWindow->point.z - pBeginPointOfClippedPolygon->point.z};
			Vector toEndPoint = {pEndPointOfClippinWindow->point.x - pBeginPointOfClippedPolygon->point.x,
								0.0f,
								pEndPointOfClippinWindow->point.z - pBeginPointOfClippedPolygon->point.z};

			float cosofToBeginPoint = VectorDotProduct(&toBeginPoint, &pNormalOfClippedPolygon[indexOfclippedPolygon]);					
			float cosofToEndPoint = VectorDotProduct(&toEndPoint, &pNormalOfClippedPolygon[indexOfclippedPolygon]);	
			bool beginPointIsSpecial = (FloatEqualZero(cosofToBeginPoint) && cosofToEndPoint > 0.0f);
			bool endPointIsSpecial = (FloatEqualZero(cosofToEndPoint) &&  cosofToBeginPoint> 0.0f);
			if (beginPointIsSpecial || endPointIsSpecial)
			{
				//特殊点，即交点在被裁减的多边形的边上，且同在内侧
				ClippingInterPoint* pTempPoint = beginPointIsSpecial ? pBeginPointOfClippinWindow : pEndPointOfClippinWindow;
				pTempPoint->pointType = INTER_POINT_TYPE_CLIPPING_WINDOW_POINT_AN_INSERCTION_POINT;
				ClippingInterPoint* pCurrentPositon = &pointChainForClippedPolygon[indexOfclippedPolygon];
				ClippingInterPoint* pNextPosition = pCurrentPositon->next2ClippedPolygon;
				ClippingInterPoint* pEndPosition = &pointChainForClippedPolygon[nextIndexOfClippedPolygon];
				pTempPoint->tValueForClippedPolygon = GetTValue(&pBeginPointOfClippedPolygon->point, &pEndPointOfClippedPolygon->point, &pTempPoint->point);

				while (true)
				{
					if( pTempPoint->tValueForClippedPolygon < pCurrentPositon->tValueForClippedPolygon)
					{
						ClippingInterPoint* pLastPosition = pCurrentPositon->prev2ClippedPolygon;
						pLastPosition->next2ClippedPolygon = pTempPoint;
						pCurrentPositon->prev2ClippedPolygon = pTempPoint;
						pTempPoint->prev2ClippedPolygon = pLastPosition;
						pTempPoint->next2ClippedPolygon = pCurrentPositon;
						break;
					}
					else if (pEndPosition == pNextPosition)
					{
						pCurrentPositon->next2ClippedPolygon = pTempPoint;
						pNextPosition->prev2ClippedPolygon = pTempPoint;
						pTempPoint->prev2ClippedPolygon = pCurrentPositon;
						pTempPoint->next2ClippedPolygon = pNextPosition;
						break;
					}
					pCurrentPositon = pNextPosition;
					pNextPosition = pCurrentPositon->next2ClippedPolygon;
				}
				continue;
			}

			if (FloatEqualZero(cosofToBeginPoint) && cosofToEndPoint < 0.0f)
			{
				//特殊点，即交点在被裁减的多边形的边上，且同在外侧
				//直接抛弃，不处理
				continue;
			}
			if (FloatEqualZero(cosofToEndPoint) && cosofToBeginPoint < 0.0f)
			{
				//特殊点，即交点在被裁减的多边形的边上，且同在外侧
				//直接抛弃，不处理
				continue;
			}

			//如果两点在边的同一侧，即没有交点，直接跳过
			if (cosofToBeginPoint > 0.0f && cosofToEndPoint > 0.0f)
			{
				continue;
			}
			if (cosofToBeginPoint < 0.0f && cosofToEndPoint < 0.0f)
			{
				continue;
			}	

			//如果有交点，则求交点并将交点插入正确的位置
			//两相交线段求交点
			//从而可以求出p的坐标 具体算法可以参考图形学算法
			float tValue =cosofToBeginPoint;
			tValue /= (cosofToBeginPoint - cosofToEndPoint);
			float insertionX = pBeginPointOfClippinWindow->point.x + tValue * (pEndPointOfClippinWindow->point.x - pBeginPointOfClippinWindow->point.x);
			float insertionZ = pBeginPointOfClippinWindow->point.z + tValue * (pEndPointOfClippinWindow->point.z - pBeginPointOfClippinWindow->point.z);
			
			//检查点是否在线段的延长线上
			Point point = {insertionX, insertionZ};
			Point a = pBeginPointOfClippedPolygon->point;
			Point b = pEndPointOfClippedPolygon->point;
			MakeRectByPoint(&checkRect, &a, &b);
			if(true != InRect(&checkRect, &point))
			{
				continue;
			}
			
			a = pBeginPointOfClippinWindow->point;
			b = pEndPointOfClippinWindow->point;
			MakeRectByPoint(&checkRect, &a, &b);
			if(true != InRect(&checkRect, &point))
			{
				continue;
			}
			
			ClippingInterPoint* pTempPoint = (ClippingInterPoint*)malloc(sizeof(ClippingInterPoint));
			++allPointCountForClipping;
			pTempPoint->point = point;
			pTempPoint->tValueForClippingWindow = tValue;
			pTempPoint->tValueForClippedPolygon = GetTValue(&pBeginPointOfClippedPolygon->point, &pEndPointOfClippedPolygon->point, &point);

			//通过遍历链表查找正确的位置插入生成点
			ClippingInterPoint* pCurrentPositon = &pointChainForClippingWindow[indexOfClippingWindow];
			ClippingInterPoint* pNextPosition = pCurrentPositon->next2ClippingdWindow;
			ClippingInterPoint* pEndPosition = &pointChainForClippingWindow[nextIndexOfClippingWindow];
			bool pointIsAdd = false;
			while(true)
			{
				if (IsSamePoint(&pTempPoint->point, &pCurrentPositon->point))
				{
					//检查该点以否已经添加,如果已经添加则退出循环
					//防止重复添加导致在后面在裁剪的时候死循环
					pointIsAdd = true;
					break;
				}
				if( pTempPoint->tValueForClippingWindow < pCurrentPositon->tValueForClippingWindow)
				{
					ClippingInterPoint* pLastPosition = pCurrentPositon->prev2ClippingdWindow;
					pLastPosition->next2ClippingdWindow = pTempPoint;
					pCurrentPositon->prev2ClippingdWindow = pTempPoint;
					pTempPoint->prev2ClippingdWindow = pLastPosition;
					pTempPoint->next2ClippingdWindow = pCurrentPositon;
					break;
				}
				else if (pEndPosition == pNextPosition)
				{
					pCurrentPositon->next2ClippingdWindow = pTempPoint;
					pNextPosition->prev2ClippingdWindow = pTempPoint;
					pTempPoint->prev2ClippingdWindow = pCurrentPositon;
					pTempPoint->next2ClippingdWindow = pNextPosition;
					break;
				}
				pCurrentPositon = pNextPosition;
				pNextPosition = pCurrentPositon->next2ClippingdWindow;
			}
			if (true == pointIsAdd)
			{
				free(pTempPoint);
				continue;
			}
			
			//通过遍历链表查找正确的位置插入生成点
			pCurrentPositon = &pointChainForClippedPolygon[indexOfclippedPolygon];
			pNextPosition = pCurrentPositon->next2ClippedPolygon;
			pEndPosition = &pointChainForClippedPolygon[nextIndexOfClippedPolygon];
			while (true)
			{
				if( pTempPoint->tValueForClippedPolygon < pCurrentPositon->tValueForClippedPolygon)
				{
					ClippingInterPoint* pLastPosition = pCurrentPositon->prev2ClippedPolygon;
					pLastPosition->next2ClippedPolygon = pTempPoint;
					pCurrentPositon->prev2ClippedPolygon = pTempPoint;
					pTempPoint->prev2ClippedPolygon = pLastPosition;
					pTempPoint->next2ClippedPolygon = pCurrentPositon;
					break;
				}
				else if (pEndPosition == pNextPosition)
				{
					pCurrentPositon->next2ClippedPolygon = pTempPoint;
					pNextPosition->prev2ClippedPolygon = pTempPoint;
					pTempPoint->prev2ClippedPolygon = pCurrentPositon;
					pTempPoint->next2ClippedPolygon = pNextPosition;
					break;
				}
				pCurrentPositon = pNextPosition;
				pNextPosition = pCurrentPositon->next2ClippedPolygon;
			}

			if (cosofToBeginPoint < 0.0f && cosofToEndPoint > 0.0f)
			{
				pTempPoint->pointType = INTER_POINT_TYPE_INTERSECTION_POINT_FOR_IN;
			}
			else if(cosofToBeginPoint > 0.0f && cosofToEndPoint < 0.0f)
			{
				pTempPoint->pointType = INTER_POINT_TYPE_INTERSECTION_POINT_FOR_OUT;
			}
			else
			{
				assert(0);
			}
		}
	}

	//完成链表构建
	//开始生成convex polygon

	//首先检查是否有足够的空间容纳结果
	int* bufferForPolygonConverCount = (int*)malloc(sizeof(int*) * allPointCountForClipping);
	GenerateConvexPolygon(pointChainForClippedPolygon, pointCountForClippedPolygon, NULL, bufferForPolygonConverCount, allPointCountForClipping);

	//完成链表构建
	//开始生成convex polygon

	for (int i = 0; i < pointCountForClippedPolygon; ++i)
	{
		pointChainForClippedPolygon[i].state = PROCESS_STATE_UNPROCESSED;
		int checkResult = PointInPolygon(clippingWindow, pointChainForClippedPolygon[i].point.x, pointChainForClippedPolygon[i].point.z);
		if (checkResult >= 0)
		{
			pointChainForClippedPolygon[i].state = PROCESS_STATE_NO_NEED_PROCESS;
		}

	}

	GenerateConvexPolygon(pointChainForClippedPolygon, pointCountForClippedPolygon, queue, bufferForPolygonConverCount, allPointCountForClipping);

	//释放分配的内存
	ClippingInterPoint* pCurrent = &pointChainForClippedPolygon[0];
	ClippingInterPoint* pNext = pCurrent->next2ClippedPolygon;
	while (pNext != &pointChainForClippedPolygon[0])
	{
		if (INTER_POINT_TYPE_INTERSECTION_POINT_FOR_IN == pNext->pointType || INTER_POINT_TYPE_INTERSECTION_POINT_FOR_OUT == pNext->pointType)
		{
			pCurrent = pNext;
			pNext = pNext->next2ClippedPolygon;
			free(pCurrent);
		}
		else
		{
			pNext = pNext->next2ClippedPolygon;
		}
	}
	Queue* checkQueue = CreateQueue(GetDataCountFromQueue(queue));
	while (MeshPolygon* checkPolygon = (MeshPolygon*)PopDataFromQueue(queue))
	{
		if (true != IsSamePolygon(checkPolygon, clippedPolygon))
		{
			PushDataToQueue(checkQueue, (void*)checkPolygon);
		}
	}
	ShiftQueueData(queue, checkQueue);
	ReleaseQueue(checkQueue);
	free(bufferForPolygonConverCount);
	free(pointChainForClippedPolygon);
	free(pointChainForClippingWindow);
	return 0;
}

//遍历链表，分割多边形
//教材上的的做法是用逆时针的多边形裁剪顺时针的多边形
//这里采用的做法是用顺时针的多边形裁剪顺时针的多边形(应为Convex不支持逆时针的多边形凸包)
//所以做法和教材上有区别
//教材上的做法是用单向链表即可
//本实现必须采用双向链表
static int GenerateConvexPolygon(ClippingInterPoint* pointList, const int pointCount, Queue* queue, int* bufferForPolygonConverCount, int bufferCount)
{
	bool isGenerate = NULL != queue ? true : false;
	int resultCount = 0;;
	if (true != isGenerate)
	{
		memset(bufferForPolygonConverCount, 0, sizeof(bufferForPolygonConverCount[0]) * bufferCount);
	}
	for (int i = 0; i < pointCount; ++i)
	{
		if (PROCESS_STATE_UNPROCESSED != pointList[i].state)
		{
			continue;
		}

		pointList[i].state = PROCESS_STATE_BEGIN_ADD;
		ClippingInterPoint* tempPoint = pointList[i].next2ClippedPolygon;
		ClippingInterPoint* lastPoint = &pointList[i];
		MeshPolygon* polygon = isGenerate ? CreatePolygon(bufferForPolygonConverCount[resultCount]) : NULL;
		if (isGenerate)
		{
			AddPointToPolygon(polygon, lastPoint->point.x, lastPoint->point.z);
		}
		else
		{
			++bufferForPolygonConverCount[resultCount];
		}
		while (true)
		{
			if(PROCESS_STATE_BEGIN_ADD == tempPoint->state)
			{
				break;
			}
			if(isGenerate)
			{
				AddPointToPolygon(polygon, tempPoint->point.x, tempPoint->point.z);
			}
			else
			{
				++bufferForPolygonConverCount[resultCount];
			}
			if (INTER_POINT_TYPE_CLIPPED_PYLIGON_POINT == tempPoint->pointType)
			{
				tempPoint->state = PROCESS_STATE_ADDED_TO_POLYGON;
				lastPoint = tempPoint;
				tempPoint = tempPoint->next2ClippedPolygon;
			}
			else if(INTER_POINT_TYPE_CLIPPING_WINDOW_POINT == tempPoint->pointType)
			{
				lastPoint = tempPoint;
				tempPoint = tempPoint->prev2ClippingdWindow;
			}
			else if(INTER_POINT_TYPE_INTERSECTION_POINT_FOR_IN == tempPoint->pointType)
			{
				lastPoint = tempPoint;
				tempPoint = tempPoint->next2ClippedPolygon;

			}
			else if(INTER_POINT_TYPE_INTERSECTION_POINT_FOR_OUT == tempPoint->pointType)
			{
				lastPoint = tempPoint;
				tempPoint = tempPoint->prev2ClippingdWindow;
			}
			else if(INTER_POINT_TYPE_CLIPPING_WINDOW_POINT_AN_INSERCTION_POINT == tempPoint->pointType)
			{
				//特殊点
				if(INTER_POINT_TYPE_CLIPPING_WINDOW_POINT == lastPoint->pointType
					|| INTER_POINT_TYPE_INTERSECTION_POINT_FOR_OUT == lastPoint->pointType
					|| INTER_POINT_TYPE_CLIPPING_WINDOW_POINT_AN_INSERCTION_POINT  == lastPoint->pointType)
				{
					lastPoint = tempPoint;
					tempPoint = tempPoint->next2ClippedPolygon;
				}
				else if(INTER_POINT_TYPE_CLIPPED_PYLIGON_POINT  == lastPoint->pointType)
				{
					lastPoint = tempPoint;
					tempPoint = tempPoint->prev2ClippingdWindow;
				}
				else
				{
					assert(0);
				}
			}
			else
			{
				assert(0);
			}
		}
		if (isGenerate)
		{
			PushDataToQueue(queue, polygon);
		}
		++resultCount;
	}
	return resultCount;
}
static ClippingInterPoint* CreateClippedPolygonData(const MeshPolygon* clippedPolygon, const MeshPolygon* clippingWindow)
{
	const Point* pointListForClippedPolygon = GetPolygonPointList(clippedPolygon);
	const int pointCountForClippedPolygon = GetPolygonPointCount(clippedPolygon);

	ClippingInterPoint* pointChainForClippedPolygon = (ClippingInterPoint*)malloc(sizeof(ClippingInterPoint) * pointCountForClippedPolygon);
	for (int i = 0; i < pointCountForClippedPolygon; ++i)
	{
		int prevIndex = 0 == i ? pointCountForClippedPolygon - 1 : i - 1;
		int nextIndex = i + 1 == pointCountForClippedPolygon ? 0 : i + 1;
		pointChainForClippedPolygon[i].point = pointListForClippedPolygon[i];
		pointChainForClippedPolygon[i].pointType = INTER_POINT_TYPE_CLIPPED_PYLIGON_POINT;
		pointChainForClippedPolygon[i].prev2ClippedPolygon = &pointChainForClippedPolygon[prevIndex];
		pointChainForClippedPolygon[i].next2ClippedPolygon = &pointChainForClippedPolygon[nextIndex];
		pointChainForClippedPolygon[i].prev2ClippingdWindow = NULL;
		pointChainForClippedPolygon[i].next2ClippingdWindow = NULL;
		pointChainForClippedPolygon[i].state = PROCESS_STATE_UNPROCESSED;
		pointChainForClippedPolygon[i].tValueForClippedPolygon = -999999.0f;
		pointChainForClippedPolygon[i].tValueForClippingWindow = -999999.0f;
		int checkReslt = PointInPolygon(clippingWindow, pointChainForClippedPolygon[i].point.x, pointChainForClippedPolygon[i].point.z);
		if (checkReslt >= 0)
		{
			pointChainForClippedPolygon[i].state = PROCESS_STATE_NO_NEED_PROCESS;
		}

	}
	return pointChainForClippedPolygon;
}

static ClippingInterPoint* CreateClippingWindowData(const MeshPolygon* clippedPolygon, const MeshPolygon* clippingWindow)
{

	const Point* pointListForClipingWindow  = GetPolygonPointList(clippingWindow);
	const int pointCountForClippingWindow  = GetPolygonPointCount(clippingWindow);

	ClippingInterPoint* pointChainForClippingWindow = (ClippingInterPoint*)malloc(sizeof(ClippingInterPoint) * pointCountForClippingWindow);
	for (int i = 0; i < pointCountForClippingWindow; ++i)
	{
		int prevIndex = 0 == i ? pointCountForClippingWindow - 1 : i - 1;
		int nextIndex = i + 1 == pointCountForClippingWindow ? 0 : i + 1;
		pointChainForClippingWindow[i].point = pointListForClipingWindow[i];
		pointChainForClippingWindow[i].pointType = INTER_POINT_TYPE_CLIPPING_WINDOW_POINT;
		pointChainForClippingWindow[i].prev2ClippingdWindow = &pointChainForClippingWindow[prevIndex];
		pointChainForClippingWindow[i].next2ClippingdWindow = &pointChainForClippingWindow[nextIndex];
		pointChainForClippingWindow[i].prev2ClippedPolygon = NULL;
		pointChainForClippingWindow[i].next2ClippedPolygon = NULL;
		pointChainForClippingWindow[i].tValueForClippedPolygon = -999999.0f;
		pointChainForClippingWindow[i].tValueForClippingWindow = -999999.0f;
	}
	return pointChainForClippingWindow;
}

static float GetTValue( const Point* beginPoint, const Point* endPoint, const Point* middlePoint )
{
	float t = 0.0f;
	float deltX = endPoint->x - beginPoint->x;
	if(abs(deltX) < EPSILON)
	{
		t = (middlePoint->z - beginPoint->z) / (endPoint->z - beginPoint->z);
	}
	else
	{
		t = (middlePoint->x - beginPoint->x) / (endPoint->x - beginPoint->x);
	}
	return t;
}
