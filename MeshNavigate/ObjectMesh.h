/*
**文件名称：ObjectMesh.h
**功能描述：场景内的物体投影到水平面的mesh
**文件说明：
**作    者：陈琳
**创建时间：2012-02-21
**修    改：
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

