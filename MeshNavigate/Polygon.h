/*
**�ļ����ƣ�Polygon.h
**���������������
**�ļ�˵����
**��    �ߣ�����
**����ʱ�䣺2012-02-21
**��    �ģ�
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

//��Ҫ�������Լ���֤Polygon��Convex Polygon
//����0�����ڲ�
//==0ʵ�ڶ���εı���
//<0�����ⲿ
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
