/*
**文件名称：Heap.cpp
**功能描述：网格寻路基础数据结构
**文件说明：
**作    者：陈琳
**创建时间：2012-02-21
**修    改：
*/
#include "stdafx.h"
#include "Heap.h"
#include "PathFinder.h"
#include <malloc.h>
#include <memory.h>
#include <stdio.h>

struct UBI_MNHeap
{
	UBI_MNPolygon** m_polygons;
	int m_capacity;
	int m_size;
};

UBI_MNHeap* CreateHeap( int maxCapacity )
{
	UBI_MNHeap* heap = (UBI_MNHeap*)malloc(sizeof(UBI_MNHeap));
	heap->m_capacity = maxCapacity;
	heap->m_size = 0;
	heap->m_polygons = (UBI_MNPolygon**)malloc(sizeof(UBI_MNPolygon*) * maxCapacity);
	return heap;
}

void ReleaseHeap( UBI_MNHeap* heap )
{
	free(heap->m_polygons);
	free(heap);
}

bool MNHeapEmpty( UBI_MNHeap* heap )
{
	return 	heap->m_size == 0;
}

void MNHeapClear( UBI_MNHeap* heap )
{
	heap->m_size = 0;
	memset(heap->m_polygons, 0, sizeof(UBI_MNPolygon*) * heap->m_capacity);
}

#define PolygonCompare(a, b) (a->m_pathFindData.m_totalCost <= b->m_pathFindData.m_totalCost)

static void MNHeapShiftUp(UBI_MNHeap* heap, UBI_MNPolygon* polygon, int index)
{
	int parentIndex;
	while (index > 0)
	{
		parentIndex = index / 2;
		if(!PolygonCompare(polygon, heap->m_polygons[parentIndex]))
		{
			break;
		}
		heap->m_polygons[index] = heap->m_polygons[parentIndex];
		heap->m_polygons[parentIndex] = polygon;
		index = parentIndex;
	}
}

static void MNHeapShiftDown(UBI_MNHeap* heap, UBI_MNPolygon* polygon, int index)
{
	int leftChild = 0;
	int rightChild = 0;
	int swapIndex = 0;
	while (index < heap->m_size)
	{
		leftChild = (index << 1) + 1;
		rightChild = leftChild + 1;
		bool swapLeft = leftChild < heap->m_size;
		bool swapRight = rightChild < heap->m_size;

		if (swapLeft && swapRight)
		{
			swapIndex = PolygonCompare(heap->m_polygons[leftChild], heap->m_polygons[rightChild]) ? leftChild : rightChild;
		}
		else if(swapLeft)
		{
			swapIndex = leftChild;
		}
		else if(swapRight)
		{
			swapIndex = rightChild;
		}
		else
		{
			break;
		}

		if (PolygonCompare(polygon, heap->m_polygons[swapIndex]))
		{
			break;
		}
		heap->m_polygons[index] = heap->m_polygons[swapIndex];
		heap->m_polygons[swapIndex] = polygon;
		index = swapIndex;
	}
}

void MNHeapPush(UBI_MNHeap* heap, UBI_MNPolygon* polygon )
{
	int index = heap->m_size++;
	heap->m_polygons[index] = polygon;
	MNHeapShiftUp(heap, polygon, index);
}

void MNHeapModify( UBI_MNHeap* heap, UBI_MNPolygon* polygon, bool isHigher)
{
	for (int i = 0; i < heap->m_size; ++i)
	{
		if (heap->m_polygons[i] == polygon)
		{
			isHigher ? MNHeapShiftUp(heap, polygon, i) : MNHeapShiftDown(heap, polygon, i); 
			break;
		}
	}
}

UBI_MNPolygon* MNHeapPop( UBI_MNHeap* heap )
{
	if(0 == heap->m_size)
	{
		return NULL;
	}
	UBI_MNPolygon* result = heap->m_polygons[0];
	heap->m_polygons[0] = NULL;
	if (1 == heap->m_size)
	{
		heap->m_size = 0;
		return result;
	}
	heap->m_polygons[0] = heap->m_polygons[--heap->m_size];
	heap->m_polygons[heap->m_size] = NULL;
	MNHeapShiftDown(heap, heap->m_polygons[0], 0);
	return result;
}

