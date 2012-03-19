/*
**文件名称：ConcavePolygonDecompose.h
**功能描述：
**文件说明：
**作    者：陈琳
**创建时间：2012-02-21
**修    改：
*/

#pragma once

struct MeshPolygon;
struct Queue;

//所有输入的多边形必须是顺时针顺序

const int NO_NEED_DECOMPOSE = 1;

const int WRONG_DECOMPOSE_PARAM = -1;
const int WRONG_DATA = -2;
extern int ConcavePolygonDecompose(const MeshPolygon* polygon,
								  Queue* result);
