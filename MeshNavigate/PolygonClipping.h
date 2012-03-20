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

//����β��ཻ ����Ҫ�ü�
const int NOT_NEED_CLIPPING = 3;

//�ü������λ�ڱ��ü�������ڲ����޷��ü�
//Ŀǰ��֧���οյ����
const int CLIPPING_WINDOW_ALL_IN_CLIPPED_POLYGON = 4;

//����ĵĲ�������
const int WRONG_PARAM = -1;

extern int ConvexPolygonClipping(const MeshPolygon* clippedPolygon,
								  const MeshPolygon* clipWindow,
								  Queue* queue);
