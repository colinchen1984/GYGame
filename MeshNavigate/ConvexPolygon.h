/*
**�ļ����ƣ�ConvexPolygon.h
**����������͹�����
**�ļ�˵����
**��    �ߣ�����
**����ʱ�䣺2012-02-21
**��    �ģ�
*/

#pragma once

struct ConvexPolygon;
struct Point;

extern ConvexPolygon* CreateConvexPolygon(int maxVertexCount);

extern void ReleaseConvexPolygon(ConvexPolygon* polygon);

extern ConvexPolygon* MakeConvexPolygonCopy(ConvexPolygon* polygon);

extern void CleanConvexPolygon(ConvexPolygon* polygon);

extern bool PointInPolygon(const ConvexPolygon* polygon, float x, float z); 

extern bool AddPointToPolygon(ConvexPolygon* polygon, float x, float z, bool check);

extern const Point* GetPolygonPointList(const ConvexPolygon* polygon);

extern int GetPolygonPointCount(const ConvexPolygon* polygon);
