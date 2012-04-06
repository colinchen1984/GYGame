/*
**�ļ����ƣ�Heap.h
**��������������Ѱ·�������ݽṹ
**�ļ�˵����
**��    �ߣ�����
**����ʱ�䣺2012-02-21
**��    �ģ�
*/

#ifndef __SH_HEAP_H__
#define __SH_HEAP_H__

struct UBI_MNHeap;
struct UBI_MNPolygon;

//����һ���µĶ�
extern UBI_MNHeap* CreateHeap(int maxCapacity);

extern void ReleaseHeap(UBI_MNHeap* heap);

extern void MNHeapClear(UBI_MNHeap* heap);

extern void MNHeapPush(UBI_MNHeap* heap, UBI_MNPolygon* polygon);

extern void MNHeapModify(UBI_MNHeap* heap, UBI_MNPolygon* polygon, bool isHigher);

extern UBI_MNPolygon* MNHeapPop(UBI_MNHeap* heap);

extern bool MNHeapEmpty(UBI_MNHeap* heap);

#endif

