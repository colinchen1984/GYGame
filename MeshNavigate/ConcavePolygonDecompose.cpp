/*
**�ļ����ƣ�ConcavePolygonDecompose.h
**����������
**�ļ�˵����
**��    �ߣ�����
**����ʱ�䣺2012-02-21
**��    �ģ�
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

//ʹ�õ���˳ʱ����,���Ժͽ̲��Ͻ��ܵķ��������෴

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
			//ƽ�ƶ����
			for (int i = 0; i < pointCount; ++i)
			{
				MritrixPlusPoint(&transferMatrix, &dPointList[i].point);
			}
			//��ת�����
			//����ת����
			bool countClockWiseRotate = dPointList[nextIndex].point.z > 0 ? true : false;
			//��Ϸ����Z����,������ת����Ҫȡ��
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
				//�õ㲻�账��
				continue;
			}
		}

		break;
	}
	return 0;
}
