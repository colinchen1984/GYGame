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

//��������Ķ���α�����˳ʱ��˳��

const int NO_NEED_DECOMPOSE = 1;

const int WRONG_DECOMPOSE_PARAM = -1;
extern int ConcavePolygonDecompose(const MeshPolygon* polygon,
								  MeshPolygon** result,
								  const int maxResultCount,
								  int* resultCount);
