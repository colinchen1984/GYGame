/*
**文件名称：BaseStruct.h
**功能描述：网格寻路基础数据结构
**文件说明：
**作    者：陈琳
**创建时间：2012-02-21
**修    改：
*/

#include "BaseStruct.h"
#include <math.h>
#include <malloc.h>
#include <string.h>

void MakeRectByPoint(Rect* rect, const Point* a, const Point* b)
{
	rect->leftTop.x = a->x < b->x ? a->x : b->x;
	rect->leftTop.z = a->z < b->z ? a->z : b->z;
	
	rect->width = abs(a->x - b->x);
	rect->height = abs(a->z - b->z);
}

bool InRect(const Rect* rect, const Point* a)
{
	bool xreslut = rect->leftTop.x <= a->x && rect->leftTop.x + rect->width >= a->x;
	bool zreslut = rect->leftTop.z <= a->z && rect->leftTop.z + rect->height >= a->z;
	return xreslut && zreslut;
}

bool IsSamePoint( const Point* a, const Point* b )
{
	float xSquar = (a->x - b->x);
	float zSquar = (a->z - b->z);
	const static float SQUAR_EPSILON = EPSILON * EPSILON;
	xSquar *= xSquar;
	zSquar *= zSquar;
	bool result = xSquar + zSquar < SQUAR_EPSILON;
	return result;
}

Vector* NormalizationVector( Vector* v )
{
	float t = v->x * v->x + v->y * v->y + v->z * v->z;
	t = sqrt(t);
	v->x /= t;
	v->y /= t;
	v->z /= t;
	return v;
}

Matrix3x3* IdentityMatrix( Matrix3x3* matrix )
{
	for(int i = 0; i < 3; ++i)
	{
		for(int t = 0; t < 3; ++t)
		{
				matrix->point[i][t] = 0.0f;
		}
	}
	for (int i = 0; i < 3; ++i)
	{
		matrix->point[i][i] = 1.0f;
	}
	return matrix;
}

struct Queue
{
	void**	m_queue;
	int		m_maxCount;
	int		m_currentDataCount;
};

Queue* CreateQueue(int maxCount)
{
	Queue* queue = (Queue*)malloc(sizeof(Queue));
	queue->m_queue = (void**)(malloc(sizeof(void*) * maxCount));
	queue->m_maxCount = maxCount;
	queue->m_currentDataCount = 0;
	return queue;
}

void PushDataToQueue(Queue* queue, void* data)
{
	if (NULL == queue)
	{
		return;
	}

	if (queue->m_currentDataCount == queue->m_maxCount)
	{
		queue->m_maxCount = queue->m_maxCount << 1;
		queue->m_queue = (void**)realloc(queue->m_queue, sizeof(void*) * queue->m_maxCount);
	}
	queue->m_queue[queue->m_currentDataCount++] = data;
}

void* PopDataFromQueue(Queue* queue)
{
	if (NULL == queue)
	{
		return NULL;
	}

	if (0 == queue->m_currentDataCount)
	{
		return NULL;
	}

	return queue->m_queue[--queue->m_currentDataCount];
}

void ReleaseQueue(Queue* queue)
{
	free(queue->m_queue);
	free(queue);
}

void CleanQueue( Queue* queue )
{
	if (NULL == queue)
	{
		return;
	}

	queue->m_currentDataCount = 0;
}

int GetDataCountFromQueue( Queue* queue )
{
	if (NULL == queue)
	{
		return 0;
	}

	return queue->m_currentDataCount;
}

void* GetDataFromQueueByIndex(Queue* queue, int index)
{
	if (NULL == queue || index >= queue->m_currentDataCount)
	{
		return 0;
	}

	return queue->m_queue[index];
}


void* DeleteDataFromQueue( Queue* queue, int dataIndex )
{
	if (NULL == queue || dataIndex >= queue->m_currentDataCount)
	{
		return NULL;
	}
	if (dataIndex + 1== queue->m_currentDataCount)
	{
		return PopDataFromQueue(queue);
	}
	void* data = queue->m_queue[dataIndex];
	memmove(queue->m_queue[dataIndex], queue->m_queue[dataIndex + 1], sizeof(void*) * (queue->m_currentDataCount - dataIndex - 1));
	--queue->m_currentDataCount;
	return data;
}

void ShiftQueueData( Queue* dest, Queue* src )
{
	while(void* data = PopDataFromQueue(src))
	{
		PushDataToQueue(dest, data);
	}
}

bool InCircle( const Circle* c, const Point* p )
{
	float dX = c->center.x - p->x;
	float dZ = c->center.z - p->z;
	float d = dX * dX + dZ * dZ;
	return d < c->radiusSquare ;
}

bool IsSameEdga( const Edga* a, const Edga* b )
{
	if (IsSamePoint(&a->begin, &b->begin) && IsSamePoint(&a->end, &b->end))
	{
		return true;
	}
	else if (IsSamePoint(&a->begin, &b->end) && IsSamePoint(&a->end, &b->begin))
	{
		return true;
	}
	return false;
}