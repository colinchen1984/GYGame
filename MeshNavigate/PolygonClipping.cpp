/*
**文件名称：PolygonClipping.cpp
**功能描述：
**文件说明：
**作    者：陈琳
**创建时间：2012-02-21
**修    改：
*/

#include "PolygonClipping.h"
#include "ConvexPolygon.h"
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

int ConvexPolygonClipping(const ConvexPolygon* clippedPolygon,
								  const ConvexPolygon* clippingWindow,
								  ConvexPolygon** result,
								  const int maxResultCount,
								  int* resultCount)
{
	if(NULL == clippedPolygon || NULL == clippingWindow)
	{
		return WRONG_PARAM;
	}

	const Point* pointListForClipingWindow  = GetPolygonPointList(clippingWindow);
	const int pointCountForClippingWindow  = GetPolygonPointCount(clippingWindow);
	int clipingWindowPointInClippedWindowCount = 0;
	for (int i = 0; i < pointCountForClippingWindow; ++i)
	{
		if (PointInPolygon(clippedPolygon, pointListForClipingWindow[i].x, pointListForClipingWindow[i].z))
		{
			++clipingWindowPointInClippedWindowCount;
		}
	}


	if (pointCountForClippingWindow == clipingWindowPointInClippedWindowCount)
	{
		return CLIPPING_WINDOW_ALL_IN_CLIPPED_POLYGON;
	}


	const Point* pointListForClippedPolygon = GetPolygonPointList(clippedPolygon);
	const int pointCountForClippedPolygon = GetPolygonPointCount(clippedPolygon);
	int clippedPolygonointInClippedWindowCount = 0;
	for (int i = 0; i < pointCountForClippedPolygon; ++i)
	{
		if (PointInPolygon(clippingWindow, pointListForClippedPolygon[i].x, pointListForClippedPolygon[i].z))
		{
			++clippedPolygonointInClippedWindowCount;
		}
	}

	if (pointCountForClippedPolygon == clippedPolygonointInClippedWindowCount)
	{
		return ALL_POINT_IN_CLIPPING_WINDOW;
	}

	ClippingInterPoint* pointChainForClippedPolygon = (ClippingInterPoint*)malloc(sizeof(ClippingInterPoint) * pointCountForClippedPolygon);
	const ClippingInterPoint* const pHeadOfClippedPolygonChain = pointChainForClippedPolygon;
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
		
		if (PointInPolygon(clippingWindow, pointChainForClippedPolygon[i].point.x, pointChainForClippedPolygon[i].point.z))
		{
			pointChainForClippedPolygon[i].state = PROCESS_STATE_NO_NEED_PROCESS;
		}
		
	}

	ClippingInterPoint* pointChainForClippingWindow = (ClippingInterPoint*)malloc(sizeof(ClippingInterPoint) * pointCountForClippingWindow);
	const ClippingInterPoint* const pHeadOfClippingWindow = pointChainForClippingWindow;
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

	//查找交点并将交点插入正确的位置
	const Vector* const pNormalOfClippedPolygon = GetPolygonNormal(clippedPolygon);
	Rect checkRect;
	for (int indexOfClippingWindow = 0; indexOfClippingWindow < pointCountForClippingWindow; ++indexOfClippingWindow)
	{
		int nextIndexOfClippingWindow = indexOfClippingWindow + 1 == pointCountForClippingWindow ? 0 : indexOfClippingWindow + 1;

		for (int indexOfclippedPolygon = 0; indexOfclippedPolygon < pointCountForClippedPolygon; ++indexOfclippedPolygon)
		{
			int nextIndexOfClippedPolygon = indexOfclippedPolygon + 1 == pointCountForClippedPolygon ? 0 : indexOfclippedPolygon + 1;

			Vector toBeginPoint = {pointChainForClippingWindow[indexOfClippingWindow].point.x - pointChainForClippedPolygon[indexOfclippedPolygon].point.x,
									0.0f,
									pointChainForClippingWindow[indexOfClippingWindow].point.z - pointChainForClippedPolygon[indexOfclippedPolygon].point.z};
			Vector toEndPoint = {pointChainForClippingWindow[nextIndexOfClippingWindow].point.x - pointChainForClippedPolygon[indexOfclippedPolygon].point.x,
								0.0f,
								pointChainForClippingWindow[nextIndexOfClippingWindow].point.z - pointChainForClippedPolygon[indexOfclippedPolygon].point.z};

			float cosofToBeginPoint = VectorDotProduct(&toBeginPoint, &pNormalOfClippedPolygon[indexOfclippedPolygon]);					
			float cosofToEndPoint = VectorDotProduct(&toEndPoint, &pNormalOfClippedPolygon[indexOfclippedPolygon]);	

			if (abs(cosofToBeginPoint) < EPSILON && cosofToEndPoint > 0.0f)
			{
				//特殊点，即交点在被裁减的多边形的边上，且同在内侧
				ClippingInterPoint* pTempPoint = &pointChainForClippingWindow[indexOfClippingWindow];
				pTempPoint->pointType = INTER_POINT_TYPE_CLIPPING_WINDOW_POINT_AN_INSERCTION_POINT;
				ClippingInterPoint* pCurrentPositon = &pointChainForClippedPolygon[indexOfclippedPolygon];
				ClippingInterPoint* pNextPosition = pCurrentPositon->next2ClippedPolygon;
				ClippingInterPoint* pEndPosition = &pointChainForClippedPolygon[nextIndexOfClippedPolygon];
				float deltX = pointChainForClippedPolygon[nextIndexOfClippedPolygon].point.x - pointChainForClippedPolygon[indexOfclippedPolygon].point.x;
				if(abs(deltX) < EPSILON)
				{
					pTempPoint->tValueForClippedPolygon = (pTempPoint->point.z - pointChainForClippedPolygon[indexOfclippedPolygon].point.z) / (pointChainForClippedPolygon[nextIndexOfClippedPolygon].point.z - pointChainForClippedPolygon[indexOfclippedPolygon].point.z);
				}
				else
				{
					pTempPoint->tValueForClippedPolygon = (pTempPoint->point.x - pointChainForClippedPolygon[indexOfclippedPolygon].point.x) / (pointChainForClippedPolygon[nextIndexOfClippedPolygon].point.x - pointChainForClippedPolygon[indexOfclippedPolygon].point.x);
				}
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

			if (abs(cosofToBeginPoint) < EPSILON && cosofToEndPoint < 0.0f)
			{
				//特殊点，即交点在被裁减的多边形的边上，且同在外侧
				//直接抛弃，不处理

				continue;
			}

			if (abs(cosofToEndPoint) < EPSILON &&  cosofToBeginPoint> 0.0f)
			{
				//特殊点，即交点在被裁减的多边形的边上，且同在内侧
				ClippingInterPoint* pTempPoint = &pointChainForClippingWindow[nextIndexOfClippingWindow];
				pTempPoint->pointType = INTER_POINT_TYPE_CLIPPING_WINDOW_POINT_AN_INSERCTION_POINT;
				ClippingInterPoint* pCurrentPositon = &pointChainForClippedPolygon[indexOfclippedPolygon];
				ClippingInterPoint* pNextPosition = pCurrentPositon->next2ClippedPolygon;
				ClippingInterPoint* pEndPosition = &pointChainForClippedPolygon[nextIndexOfClippedPolygon];
				float deltX = pointChainForClippedPolygon[nextIndexOfClippedPolygon].point.x - pointChainForClippedPolygon[indexOfclippedPolygon].point.x;
				if(abs(deltX) < EPSILON)
				{
					pTempPoint->tValueForClippedPolygon = (pTempPoint->point.z - pointChainForClippedPolygon[indexOfclippedPolygon].point.z) / (pointChainForClippedPolygon[nextIndexOfClippedPolygon].point.z - pointChainForClippedPolygon[indexOfclippedPolygon].point.z);
				}
				else
				{
					pTempPoint->tValueForClippedPolygon = (pTempPoint->point.x - pointChainForClippedPolygon[indexOfclippedPolygon].point.x) / (pointChainForClippedPolygon[nextIndexOfClippedPolygon].point.x - pointChainForClippedPolygon[indexOfclippedPolygon].point.x);
				}
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

			if (abs(cosofToEndPoint) < EPSILON && cosofToBeginPoint < 0.0f)
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
			//从而可以求出p的坐标
			float tValue =cosofToBeginPoint;
			tValue /= (cosofToBeginPoint - cosofToEndPoint);
			float insertionX = pointChainForClippingWindow[indexOfClippingWindow].point.x + tValue * (pointChainForClippingWindow[nextIndexOfClippingWindow].point.x - pointChainForClippingWindow[indexOfClippingWindow].point.x);
			float insertionZ = pointChainForClippingWindow[indexOfClippingWindow].point.z + tValue * (pointChainForClippingWindow[nextIndexOfClippingWindow].point.z - pointChainForClippingWindow[indexOfClippingWindow].point.z);
			Point point = {insertionX, insertionZ};
			//检查点是否在线段的延长线上
			Point a = {pointChainForClippedPolygon[indexOfclippedPolygon].point.x, pointChainForClippedPolygon[indexOfclippedPolygon].point.z};
			Point b = {pointChainForClippedPolygon[nextIndexOfClippedPolygon].point.x, pointChainForClippedPolygon[nextIndexOfClippedPolygon].point.z};
			MakeRectByPoint(&checkRect, &a, &b);
			if(true != InRect(&checkRect, &point))
			{
				continue;
			}

			ClippingInterPoint* pTempPoint = (ClippingInterPoint*)malloc(sizeof(ClippingInterPoint));
			pTempPoint->point = point;
			pTempPoint->tValueForClippingWindow = tValue;

			float deltX = pointChainForClippedPolygon[nextIndexOfClippedPolygon].point.x - pointChainForClippedPolygon[indexOfclippedPolygon].point.x;
			if(abs(deltX) < EPSILON)
			{
				pTempPoint->tValueForClippedPolygon = (insertionZ - pointChainForClippedPolygon[indexOfclippedPolygon].point.z) / (pointChainForClippedPolygon[nextIndexOfClippedPolygon].point.z - pointChainForClippedPolygon[indexOfclippedPolygon].point.z);
			}
			else
			{
				pTempPoint->tValueForClippedPolygon = (insertionX - pointChainForClippedPolygon[indexOfclippedPolygon].point.x) / (pointChainForClippedPolygon[nextIndexOfClippedPolygon].point.x - pointChainForClippedPolygon[indexOfclippedPolygon].point.x);
			}
			ClippingInterPoint* pCurrentPositon = &pointChainForClippingWindow[indexOfClippingWindow];
			ClippingInterPoint* pNextPosition = pCurrentPositon->next2ClippingdWindow;
			ClippingInterPoint* pEndPosition = &pointChainForClippingWindow[nextIndexOfClippingWindow];
			while(true)
			{
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
	*resultCount = 0;

	//首先检查是否有足够的空间容纳结果
	for (int i = 0; i < pointCountForClippedPolygon; ++i)
	{
		if (PROCESS_STATE_UNPROCESSED != pointChainForClippedPolygon[i].state)
		{
			continue;
		}
		*resultCount += 1;
		pointChainForClippedPolygon[i].state = PROCESS_STATE_BEGIN_ADD;
		ClippingInterPoint* tempPoint = pointChainForClippedPolygon[i].next2ClippedPolygon;
		ClippingInterPoint* lastPoint = &pointChainForClippedPolygon[i];
		while (true)
		{
			if(PROCESS_STATE_BEGIN_ADD == tempPoint->state)
			{
				break;
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
					|| INTER_POINT_TYPE_INTERSECTION_POINT_FOR_OUT == lastPoint->pointType)
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
	}

	if (maxResultCount < *resultCount)
	{
		return NEED_MORE_SPACE_FOR_RESULT;
	}

	//完成链表构建
	//开始生成convex polygon

	for (int i = 0; i < pointCountForClippedPolygon; ++i)
	{
		pointChainForClippedPolygon[i].state = PROCESS_STATE_UNPROCESSED;
		if (PointInPolygon(clippingWindow, pointChainForClippedPolygon[i].point.x, pointChainForClippedPolygon[i].point.z))
		{
			pointChainForClippedPolygon[i].state = PROCESS_STATE_NO_NEED_PROCESS;
		}

	}

	memset(result, 0, sizeof(result[0]) * maxResultCount);
	*resultCount = 0;

	for (int i = 0; i < pointCountForClippedPolygon; ++i)
	{
		if (PROCESS_STATE_UNPROCESSED != pointChainForClippedPolygon[i].state)
		{
			continue;
		}
		ConvexPolygon* polygon = CreateConvexPolygon(100);
		pointChainForClippedPolygon[i].state = PROCESS_STATE_BEGIN_ADD;
		ClippingInterPoint* tempPoint = pointChainForClippedPolygon[i].next2ClippedPolygon;
		ClippingInterPoint* lastPoint = &pointChainForClippedPolygon[i];
		AddPointToPolygon(polygon, lastPoint->point.x, lastPoint->point.z, false);
		while (true)
		{
			if(PROCESS_STATE_BEGIN_ADD == tempPoint->state)
			{
				break;
			}

			AddPointToPolygon(polygon, tempPoint->point.x, tempPoint->point.z, false);
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
					|| INTER_POINT_TYPE_INTERSECTION_POINT_FOR_OUT == lastPoint->pointType)
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
		result[*resultCount] = polygon;
		*resultCount += 1;
	}

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
	free(pointChainForClippedPolygon);
	free(pointChainForClippingWindow);
	return 0;
}
