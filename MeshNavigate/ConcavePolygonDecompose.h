/*
**�ļ����ƣ�ConcavePolygonDecompose.h
**����������
**�ļ�˵����
**��    �ߣ�����
**����ʱ�䣺2012-02-21
**��    �ģ�
*/

#pragma once

struct MeshPolygon;
struct Queue;

//��������Ķ���α�����˳ʱ��˳��

const int NO_NEED_DECOMPOSE = 1;

const int WRONG_DECOMPOSE_PARAM = -1;
const int WRONG_DATA = -2;
extern int ConcavePolygonDecompose(const MeshPolygon* polygon,
								  Queue* result);
