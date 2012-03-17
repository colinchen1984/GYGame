/*
**文件名称：ConvexPolygon.h
**功能描述：凸多边形
**文件说明：
**作    者：陈琳
**创建时间：2012-02-21
**修    改：
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
