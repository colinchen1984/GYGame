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
#include <malloc.h>
#include <math.h>

struct DecomposePoint
{
	Point point;
	int id;
};

//使用的是顺时针序,所以和教材上介绍的方法正好相反

int ConcavePolygonDecompose(const MeshPolygon* polygon, 
								   MeshPolygon** result, 
								   const int maxResultCount, 
								   int* resultCount )
{
	//if (NULL == polygon || NULL == resultCount)
	//{
	//	return WRONG_DECOMPOSE_PARAM;
	//}

	if (true == IsConvexPolygon(polygon))
	{
		return NO_NEED_DECOMPOSE;
	}

	const Point* const pointList = GetPolygonPointList(polygon);
	int pointCount = GetPolygonPointCount(polygon);
	DecomposePoint* dPointList = (DecomposePoint*)malloc(sizeof(DecomposePoint) * pointCount);
	for (int i = 0; i < pointCount; ++i)
	{
		dPointList[i].point = pointList[i];
		dPointList[i].id = i;
	}
	Matrix3x3 transferMatrix;
	const Point originPoint = {0.0f, 0.0f};
	while (true)
	{
		int beginIndex = 0;
		for (; beginIndex < pointCount; ++beginIndex)
		{
			int nextIndex = beginIndex + 1 == pointCount ? 0 : beginIndex + 1;
			int next2Index = nextIndex + 1 == pointCount ? 0 : nextIndex + 1;
			IdentityMatrix(&transferMatrix);
			MakeTransferMatrix(&transferMatrix, &dPointList[beginIndex].point, &originPoint);
			//平移多边形
			for (int i = 0; i < pointCount; ++i)
			{
				MritrixPlusPoint(&transferMatrix, &dPointList[i].point);
			}
			//旋转多边形
			//求旋转矩阵
			bool countClockWiseRotate = dPointList[nextIndex].point.z > 0 ? true : false;
			//游戏里面Z朝内,所以旋转方向要取反
			countClockWiseRotate = !countClockWiseRotate;
			const static Vector targetVector = {1.0f, 0.0f, 0.0f};
			Vector fromBegin2Next = {dPointList[nextIndex].point.x - dPointList[beginIndex].point.x, 
									0.0f,
									dPointList[nextIndex].point.z - dPointList[beginIndex].point.z};
			NormalizationVector(&fromBegin2Next);
			float cosValue = VectorDotProduct(&fromBegin2Next, &targetVector);
			float angle = acos(cosValue);
			IdentityMatrix(&transferMatrix);
			MakeRotateMatrix(&transferMatrix, angle, countClockWiseRotate);
			for (int i = 0; i < pointCount; ++i)
			{
				MritrixPlusPoint(&transferMatrix, &dPointList[i].point);
			}
			if (FloatEqualZero(dPointList[next2Index].point.z) || dPointList[next2Index].point.z > 0.0f)
			{
				//该点不需处理
				continue;
			}
		}

		break;
	}
	return 0;
}
