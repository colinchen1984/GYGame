/*
**�ļ����ƣ�ObjectMesh.h
**���������������ڵ�����ͶӰ��ˮƽ���mesh
**�ļ�˵����
**��    �ߣ�����
**����ʱ�䣺2012-02-21
**��    �ģ�
*/

#pragma once

struct ItemNavigateMesh;
struct ConvexPolygon;
struct Point;

extern ItemNavigateMesh* CreateItemNavigateMesh(int maxVertexCount);

extern void ReleaseItemNavigateMesh(ItemNavigateMesh* item);

extern bool AddVertexToItemNavigateMesh(ItemNavigateMesh* item, float x, float z);

extern bool MakeConvexHullFromItem(ItemNavigateMesh* item);

extern const int GeConvexHullPointCount(const ItemNavigateMesh* item);

extern const Point* GetConvexHullPointList(const ItemNavigateMesh* item);

