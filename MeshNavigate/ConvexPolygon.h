/*
**文件名称：ConvexPolygon.h
**功能描述：凸多边形
**文件说明：
**作    者：陈琳
**创建时间：2012-02-21
**修    改：
*/

#pragma once

struct MeshPolygon;
struct Point;
struct Vector;

extern MeshPolygon* CreatePolygon(int maxVertexCount);

extern void ReleasePolygon(MeshPolygon* polygon);

extern MeshPolygon* MakePolygonCopy(MeshPolygon* polygon);

extern void CleanPolygon(MeshPolygon* polygon);

//需要调用者自己保证Polygon是Convex Polygon
extern bool PointInPolygon(const MeshPolygon* polygon, float x, float z); 

extern bool AddPointToPolygon(MeshPolygon* polygon, float x, float z);

extern bool IsConvexPolygon(MeshPolygon* polygon);

extern const Point* GetPolygonPointList(const MeshPolygon* polygon);

extern int GetPolygonPointCount(const MeshPolygon* polygon);

extern const Vector* GetPolygonNormal(const MeshPolygon* polygon);
