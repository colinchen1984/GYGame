/*
**�ļ����ƣ�PolygonClipping.h
**����������
**�ļ�˵����
**��    �ߣ�����
**����ʱ�䣺2012-02-21
**��    �ģ�
*/

#pragma once

struct MeshPolygon;
struct Queue;

//����Weiler-Atherton�㷨
//��������Ķ���α�����˳ʱ��˳��

//���ü������λ�ڲü�������ڲ������Բü�����ȫ��ʧ
const int ALL_POINT_IN_CLIPPING_WINDOW = 2;

//����ĵĲ�������
const int WRONG_PARAM = -1;
//����Ļ����������洢���еĽ����resultCount�᷵����Ҫ�Ļ���������
const int NEED_MORE_SPACE_FOR_RESULT = -2;
//�ü������λ�ڱ��ü�������ڲ����޷��ü�
//Ŀǰ��֧���οյ����
const int CLIPPING_WINDOW_ALL_IN_CLIPPED_POLYGON = -3;
extern int ConvexPolygonClipping(const MeshPolygon* clippedPolygon,
								  const MeshPolygon* clipWindow,
								  Queue* queue);
