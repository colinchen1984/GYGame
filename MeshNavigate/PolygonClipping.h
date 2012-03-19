/*
**文件名称：PolygonClipping.h
**功能描述：
**文件说明：
**作    者：陈琳
**创建时间：2012-02-21
**修    改：
*/

#pragma once

struct MeshPolygon;
struct Queue;

//基于Weiler-Atherton算法
//所有输入的多边形必须是顺时针顺序

//被裁剪多边形位于裁剪多边形内部，所以裁剪后完全消失
const int ALL_POINT_IN_CLIPPING_WINDOW = 2;

//传入的的参数有误
const int WRONG_PARAM = -1;
//传入的缓存区不够存储所有的结果，resultCount会返回需要的缓存区个数
const int NEED_MORE_SPACE_FOR_RESULT = -2;
//裁剪多边形位于被裁减多边形内部，无法裁剪
//目前不支持镂空的情况
const int CLIPPING_WINDOW_ALL_IN_CLIPPED_POLYGON = -3;
extern int ConvexPolygonClipping(const MeshPolygon* clippedPolygon,
								  const MeshPolygon* clipWindow,
								  Queue* queue);
