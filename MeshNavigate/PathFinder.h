/*
**�ļ����ƣ�PathFinder.h
**��������������Ѱ·�������ݽṹ
**�ļ�˵����
**��    �ߣ�����
**����ʱ�䣺2012-02-21
**��    �ģ�
*/

#ifndef __SH_MESHNAVIGATE_H__
#define __SH_MESHNAVIGATE_H__

struct UBI_MeshNavigateSystem;

struct UBI_MNPoint
{
	float m_x;
	float m_z;
};

typedef UBI_MNPoint UBI_MNVector2D;

struct UBI_MNPolygon;

struct UBI_PathFindData
{
	int							m_flag;
	UBI_MNPolygon*				m_parent;
	int							m_parentToSelfAdjacentIndex;	//�Լ��͸�mesh���ڱ��ڸ�mesh�ڵ�index
	float						m_totalCost;
	UBI_MNPoint					m_pos;
	UBI_MNPolygon*				m_pathFindList;
};

struct UBI_MNPolygon
{
	int* 						m_pointIndex;
	int							m_pointCount;
	UBI_MNVector2D*				m_normalVectors;
	int							m_triangleSpecalLogic;
	UBI_MNPolygon**				m_adjacentPolygon;
	UBI_PathFindData			m_pathFindData;
	int							m_id;
};

//Ѱ·�ɹ�
const int PathFindSucessful = 0;
//��ʼλ�ò���Ѱ·������
const int PathFindErrorBeginPositionNoInMesh = -1;
//Ŀ��λ�ò���Ѱ·������
const int PathFindErrorTargetPositionNoInMesh = -2;
//�����Buffer�������·����Ϣ
const int PathFindErrorNotEnoughBufferForPathFind = -3;
//��ǰλ���޷�����Ŀ���
const int PathFindErrorTargetPositionUnarrivable = -4; 

extern UBI_MeshNavigateSystem* CreatePathFinder();


extern bool InitPathFinder(UBI_MeshNavigateSystem* sys, const char* configFile);

extern void ReleasePathFinder(UBI_MeshNavigateSystem* sys);

//����ֵ����0ʱѰ·�ɹ�
//����ֵС��0ʱ���Ҵ�����
//����ֵ����0ʱʧ��, ��ֵΪ·��ʹ�õĵ�float�ĸ���
extern int findPath(UBI_MeshNavigateSystem* sys, float* pathBuffer, const int bufferSize, const float beginPosX, const float beginPosZ, const float targetX, const float targetZ);

#endif
