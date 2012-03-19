/*
**文件名称：ConcavePolygonDecompose.h
**功能描述：
**文件说明：
**作    者：陈琳
**创建时间：2012-02-21
**修    改：
*/
#include "ConcavePolygondecompose.h"
#include "BaseStruct.h"
#include "Polygon.h"
#include "VectorMath.h"
#include <math.h>

//使用的是顺时针序,所以和教材上介绍的方法正好相反

int ConcavePolygonDecompose(const MeshPolygon* polygon, 
								   Queue* result)
{
	if (NULL == polygon || NULL == result)
	{
		return WRONG_DECOMPOSE_PARAM;
	}

	if (true == IsConvexPolygon(polygon))
	{
		return NO_NEED_DECOMPOSE;
	}

	const Point* const pointList = GetPolygonPointList(polygon);
	int pointCount = GetPolygonPointCount(polygon);
	MeshPolygon* fistQuest = CreatePolygon(pointCount);

	for (int i = 0; i < pointCount; ++i)
	{
		Point temp = pointList[i];
		temp.userData = (void*)i;
		AddPointToPolygonByPoint(fistQuest, &temp);
	}

	Queue* finishQueue = result;

	Queue* questQueue = CreateQueue(16);
	PushDataToQueue(questQueue, (void*)fistQuest);

	Queue* unusedPolygonQueue = CreateQueue(16);

	Matrix3x3 transferMatrix;
	const Point originPoint = {0.0f, 0.0f};
	bool errorAndReturn = false;
	while (MeshPolygon* quest = (MeshPolygon*)PopDataFromQueue(questQueue))
	{
		if(errorAndReturn)
		{
			break;
		}
		Point* dPointList = (Point*)GetPolygonPointList(quest);
		int questPointCount = GetPolygonPointCount(quest);
		int beginIndex = 0;
		for (; beginIndex < questPointCount; ++beginIndex)
		{
			int nextIndex = beginIndex + 1 == questPointCount ? 0 : beginIndex + 1;
			int next2Index = nextIndex + 1 == questPointCount ? 0 : nextIndex + 1;
			IdentityMatrix(&transferMatrix);
			MakeTransferMatrix(&transferMatrix, &dPointList[beginIndex], &originPoint);
			//平移多边形
			for (int i = 0; i < questPointCount; ++i)
			{
				MritrixPlusPoint(&transferMatrix, &dPointList[i]);
			}
			//旋转多边形
			//求旋转矩阵
			bool countClockWiseRotate = dPointList[nextIndex].z > 0 ? true : false;
			//游戏里面Z朝内,所以旋转方向要取反
			countClockWiseRotate = !countClockWiseRotate;
			const static Vector targetVector = {1.0f, 0.0f, 0.0f};
			Vector fromBegin2Next = {dPointList[nextIndex].x - dPointList[beginIndex].x, 
									0.0f,
									dPointList[nextIndex].z - dPointList[beginIndex].z};
			NormalizationVector(&fromBegin2Next);
			float cosValue = VectorDotProduct(&fromBegin2Next, &targetVector);
			float angle = acos(cosValue);
			IdentityMatrix(&transferMatrix);
			MakeRotateMatrix(&transferMatrix, angle, countClockWiseRotate);
			for (int i = 0; i < questPointCount; ++i)
			{
				MritrixPlusPoint(&transferMatrix, &dPointList[i]);
			}
			if (FloatEqualZero(dPointList[next2Index].z) || dPointList[next2Index].z > 0.0f)
			{
				//该点不需处理
				continue;
			}

			//找到V(i + 2)点
			//寻找V(i + j)点
			int vj = next2Index + 1;
			for (; vj != beginIndex; ++vj)
			{
				if (vj == questPointCount)
				{
					vj = 0;
				}
				if (FloatEqualZero(dPointList[vj].z) || dPointList[vj].z > 0.0f)
				{
					break;
				}
			}
			if (vj == beginIndex)
			{
				errorAndReturn = true;
				break;
			}

			//beginIndex->nextIndex->next2Index->...->vj组成一个多边形
			// 组织多边形 如果该多边形是凸多边形，放入完成队列
			//如果不是，放入任务队列

			int vertexCount = 0;
			for (int i = nextIndex; ; ++i)
			{
				if (i == questPointCount)
				{
					i = 0;
				}
				++vertexCount;
				if(i == vj)
				{
					break;
				}
			}
			MeshPolygon* newPolygon = (MeshPolygon*)PopDataFromQueue(unusedPolygonQueue);

			if (NULL == newPolygon)
			{
				newPolygon = CreatePolygon(vertexCount);
			}
			else if(GetPolygonPointMaxCount(newPolygon) < vertexCount)
			{
				PushDataToQueue(unusedPolygonQueue, (void*)newPolygon);
				newPolygon = CreatePolygon(vertexCount);
			}
			for (int i = nextIndex; ; ++i)
			{
				if (i == questPointCount)
				{
					i = 0;
				}
				AddPointToPolygonByPoint(newPolygon, &dPointList[i]);
				if(i == vj)
				{
					break;
				}
			}
			if (IsConvexPolygon(newPolygon))
			{
				//添加到完成队列
				PushDataToQueue(finishQueue, (void*)newPolygon);
			}
			else
			{
				PushDataToQueue(questQueue, (void*)newPolygon);
			}
			//vj->.....->beginIndex 组成一个多边形
			vertexCount = 0;
			for (int i = vj; ; ++i)
			{
				if (i == questPointCount)
				{
					i = 0;
				}
				++vertexCount;
				if(i == nextIndex)
				{
					break;
				}
			}

			newPolygon = (MeshPolygon*)PopDataFromQueue(unusedPolygonQueue);
			if (NULL == newPolygon)
			{
				newPolygon = CreatePolygon(vertexCount);
			}
			else if(GetPolygonPointMaxCount(newPolygon) < vertexCount)
			{
				PushDataToQueue(unusedPolygonQueue, (void*)newPolygon);
				newPolygon = CreatePolygon(vertexCount);
			}

			for (int i = vj; ; ++i)
			{
				if (i == questPointCount)
				{
					i = 0;
				}
				AddPointToPolygonByPoint(newPolygon, &dPointList[i]);
				if (i == nextIndex)
				{
					break;
				}
			}
			if (IsConvexPolygon(newPolygon))
			{
				//添加到完成队列
				PushDataToQueue(finishQueue, (void*)newPolygon);
			}
			else
			{
				PushDataToQueue(questQueue, (void*)newPolygon);
			}
			CleanPolygon(quest);
			PushDataToQueue(unusedPolygonQueue, (void*)quest);
			break;
		}
	}

	while(MeshPolygon* unusedPolygon = (MeshPolygon*)PopDataFromQueue(unusedPolygonQueue))
	{
		ReleasePolygon(unusedPolygon);
	}

	while(MeshPolygon* newPolgon = (MeshPolygon*)PopDataFromQueue(finishQueue))
	{
		//MeshPolygon* newPolgon = finishQueue[i];
		Point* newpointList = (Point*)GetPolygonPointList(newPolgon);
		int newPointCount = GetPolygonPointCount(newPolgon);
		for (int t = 0; t < newPointCount; ++t)
		{
			newpointList[t] = pointList[(int)newpointList[t].userData];
		}
		PushDataToQueue(questQueue, (void*)newPolgon);
	}

	while(MeshPolygon* newPolgon = (MeshPolygon*)PopDataFromQueue(questQueue))
	{
		PushDataToQueue(finishQueue, (void*)newPolgon);
	}
	ReleaseQueue(questQueue);
	ReleaseQueue(unusedPolygonQueue);
	return 0;
}
