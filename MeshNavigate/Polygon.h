/*
**文件名称：Polygon.h
**功能描述：多边形
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

extern MeshPolygon* CreatePolygonByPoint(const Point* pointList, const int pointCount);

extern void ReleasePolygon(MeshPolygon* polygon);

extern MeshPolygon* MakePolygonCopy(MeshPolygon* polygon);

extern void CleanPolygon(MeshPolygon* polygon);

//需要调用者自己保证Polygon是Convex Polygon
//大于0是在内部
//==0实在多边形的边上
//<0是在外部
extern int PointInPolygon(const MeshPolygon* polygon, float x, float z); 

extern bool AddPointToPolygon(MeshPolygon* polygon, float x, float z);

extern bool AddPointToPolygonByPoint(MeshPolygon* polygon, const Point* p);

extern bool IsConvexPolygon(const MeshPolygon* polygon);

extern const Point* GetPolygonPointList(const MeshPolygon* polygon);

extern int GetPolygonPointCount(const MeshPolygon* polygon);

extern int GetPolygonPointMaxCount(const MeshPolygon* polygon);

extern const Vector* GetPolygonNormal(const MeshPolygon* polygon);

extern bool IsSamePolygon(const MeshPolygon* a, const MeshPolygon* b);

extern bool AddAdjacentData(MeshPolygon* polygon, const int pointIndexA, const int pointIndexB, const MeshPolygon* adjacentPolygon);

extern MeshPolygon* GetAdjacentData(MeshPolygon* polygon, const int index);

extern bool SetPolygonUserData(MeshPolygon* polygon, void* userData);

extern void* GetPolygonUserData(MeshPolygon* polygon);
