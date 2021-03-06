
#include "ObjectMesh.h"
#include "BaseStruct.h"
#include "VectorMath.h"
#include "Polygon.h"
#include <malloc.h>
#include <memory.h>
#include <math.h>
#include <stdlib.h>

//整个凸包的计算过程采用的右手坐标系,x朝右为正,y朝上为正
//符合当前游戏的设定

struct InterPoint
{
	Point					m_point;
	float					m_slope;
	int						m_id;
};
struct ItemNavigateMesh
{
	InterPoint*				m_pointList;
	int						m_pointCount;
	int						m_maxPointCount;
	Point*					m_ProjectionPolygon;
	int						m_projectionPointCount;
};

ItemNavigateMesh* CreateItemNavigateMesh(int maxVertexCount)
{
	if(maxVertexCount <= 2)
	{
		return NULL;
	}
	
	ItemNavigateMesh* p = (ItemNavigateMesh*)malloc(sizeof(ItemNavigateMesh));
	if(NULL != p)
	{
		p->m_ProjectionPolygon = NULL;
		p->m_projectionPointCount = 0;
		p->m_pointCount = 0;
		p->m_maxPointCount = maxVertexCount;
		p->m_pointList = (InterPoint*)(malloc(sizeof(InterPoint) * maxVertexCount));
		memset(p->m_pointList, 0, sizeof(p->m_pointList[0]) * maxVertexCount);
	}
	return p;
}


void ReleaseItemNavigateMesh( ItemNavigateMesh* item )
{
	if(NULL == item)
	{
		return;
	}
	free(item->m_ProjectionPolygon);
	free(item->m_pointList);
	free(item);
}

bool AddVertexToItemNavigateMesh(ItemNavigateMesh* item, float x, float z)
{
	if(NULL == item)
	{
		return false;
	}
	
	if(item->m_pointCount >= item->m_maxPointCount)
	{
		return false;
	}
	Point temp = {x, z};
	for (int i = 0; i < item->m_pointCount; ++i)
	{
		if (IsSamePoint(&item->m_pointList[i].m_point, &temp))
		{
			return false;
		}
	}
	InterPoint* p = &item->m_pointList[item->m_pointCount];
	p->m_point = temp;
	p->m_id = item->m_pointCount;
	++item->m_pointCount;
	return true;
}
static int compareInterPoint(const void *left, const void *right)
{
	const InterPoint* a = (InterPoint*)left;	
	const InterPoint* b = (InterPoint*)right;
	
	//排序使cos较小的值排在前面,也就是同x轴角度大的排在前面 所以有 a->m_slope < b->m_slope ? -1 : 1;
	int result = FloatEqual(a->m_slope, b->m_slope) ? 0 : a->m_slope < b->m_slope ? -1 : 1;
	return result;
}

bool MakeConvexHullFromItem(ItemNavigateMesh* item)
{
	if (NULL == item)
	{
		return false;
	}
	if (item->m_pointCount < 3)
	{
		return false;
	}
	
	if (0 != item->m_projectionPointCount)
	{
		return false;
	}

	//求得是左下角及z轴最大的的点,z相同时去x较小的点
	int basePointIndex = 0;
	for (int i = 0; i < item->m_pointCount; ++i)
	{
		if (item->m_pointList[i].m_point.z > item->m_pointList[basePointIndex].m_point.z)
		{
			basePointIndex = i;
		}
		else if(FloatEqual(item->m_pointList[i].m_point.z, item->m_pointList[basePointIndex].m_point.z))
		{
			if(item->m_pointList[i].m_point.x < item->m_pointList[basePointIndex].m_point.x)
			{
				basePointIndex = i;
			}
		}
	}

	//根据基点对每一点求相对于向量(1, 0, 0)的夹角
	const Vector baseVector = {1.0f, 0.0f, 0.0f};
	for(int i = 0; i < item->m_pointCount; ++i)
	{
		if(i == basePointIndex)
		{//保证基点排在第一位
			item->m_pointList[i].m_slope = -1.0f;
			continue;;
		}
		Vector v = {item->m_pointList[i].m_point.x - item->m_pointList[basePointIndex].m_point.x, 0.0f, item->m_pointList[i].m_point.z - item->m_pointList[basePointIndex].m_point.z};
		NormalizationVector(&v);
		item->m_pointList[i].m_slope = VectorDotProduct(&v, &baseVector);
	}

	qsort(item->m_pointList, item->m_pointCount, sizeof(item->m_pointList[0]), compareInterPoint);

	//整个遍历过程由左下沿顺时针转到右下
	//所以得到的凸包是顺时针序

	//这里的32是经验值
	int queueMaxCount = 32;
	Point* queue = (Point*)malloc(sizeof(Point) * queueMaxCount);
	memset(queue, 0, sizeof(queue[0]) * queueMaxCount);
	for (int i = 0; i < 3; ++i)
	{
		queue[i].x = item->m_pointList[i].m_point.x;
		queue[i].z = item->m_pointList[i].m_point.z;
	}
	int pointCountInQueue = 3;
	for (int i = 3; i < item->m_pointCount; ++i)
	{
		const InterPoint* point = &item->m_pointList[i];
		for (int q = pointCountInQueue - 1; q > 0; --q)
		{
			Vector pToPplus= {point->m_point.x - queue[q].x, 0.0f, point->m_point.z - queue[q].z};
			Vector pMinsToP = {queue[q].x - queue[q - 1].x, 0.0f, queue[q].z - queue[q - 1].z};
			Vector result;
			VectorCrossProduct(&result, &pMinsToP, &pToPplus);
			if(result.y < 0.0f)
			{
				//新的点在凸包上,直接添加
				queue[pointCountInQueue].x = point->m_point.x;
				queue[pointCountInQueue].z = point->m_point.z;
				++pointCountInQueue;
				if (pointCountInQueue == queueMaxCount)
				{
					queueMaxCount = queueMaxCount << 1;
					queue = (Point*)realloc(queue, sizeof(queue[0]) * queueMaxCount);
				}
				break;
			}
			else
			{
				--pointCountInQueue;
			}
		}
	}

	//凸包是顺时针序
	item->m_ProjectionPolygon = queue;
	item->m_projectionPointCount = pointCountInQueue;
	return true;

}

const int GetConvexHullPointCount(const ItemNavigateMesh* item)
{
	if(NULL == item || NULL == item->m_ProjectionPolygon)
	{
		return NULL;
	}
	
	return (item->m_projectionPointCount);
}

const Point* GetConvexHullPointList( const ItemNavigateMesh* item )
{
	if(NULL == item || NULL == item->m_ProjectionPolygon)
	{
		return NULL;
	}

	return (item->m_ProjectionPolygon);
}

MeshPolygon* GetConvexHullToPolygon( const ItemNavigateMesh* item )
{
	if (NULL == item)
	{
		return NULL;
	}

	return CreatePolygonByPoint(item->m_ProjectionPolygon, item->m_pointCount);
}
