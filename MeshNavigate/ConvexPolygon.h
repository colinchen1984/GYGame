/*
**�ļ����ƣ�ConvexPolygon.h
**����������͹�����
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

extern void ReleasePolygon(MeshPolygon* polygon);

extern MeshPolygon* MakePolygonCopy(MeshPolygon* polygon);

extern void CleanPolygon(MeshPolygon* polygon);

//��Ҫ�������Լ���֤Polygon��Convex Polygon
extern bool PointInPolygon(const MeshPolygon* polygon, float x, float z); 

extern bool AddPointToPolygon(MeshPolygon* polygon, float x, float z);

extern bool IsConvexPolygon(MeshPolygon* polygon);

extern const Point* GetPolygonPointList(const MeshPolygon* polygon);

extern int GetPolygonPointCount(const MeshPolygon* polygon);

extern const Vector* GetPolygonNormal(const MeshPolygon* polygon);
