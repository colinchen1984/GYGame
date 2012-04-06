/*
**文件名称：Heap.h
**功能描述：网格寻路基础数据结构
**文件说明：
**作    者：陈琳
**创建时间：2012-02-21
**修    改：
*/

#ifndef __SH_HEAP_H__
#define __SH_HEAP_H__

struct UBI_MNHeap;
struct UBI_MNPolygon;

//创建一个新的堆
extern UBI_MNHeap* CreateHeap(int maxCapacity);

extern void ReleaseHeap(UBI_MNHeap* heap);

extern void MNHeapClear(UBI_MNHeap* heap);

extern void MNHeapPush(UBI_MNHeap* heap, UBI_MNPolygon* polygon);

extern void MNHeapModify(UBI_MNHeap* heap, UBI_MNPolygon* polygon, bool isHigher);

extern UBI_MNPolygon* MNHeapPop(UBI_MNHeap* heap);

extern bool MNHeapEmpty(UBI_MNHeap* heap);

#endif

