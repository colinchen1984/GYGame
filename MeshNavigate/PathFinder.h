/*
**文件名称：PathFinder.h
**功能描述：网格寻路基础数据结构
**文件说明：
**作    者：陈琳
**创建时间：2012-02-21
**修    改：
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
	int							m_parentToSelfAdjacentIndex;	//自己和父mesh相邻边在父mesh内的index
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

//寻路成功
const int PathFindSucessful = 0;
//起始位置不在寻路网格内
const int PathFindErrorBeginPositionNoInMesh = -1;
//目标位置不在寻路网格内
const int PathFindErrorTargetPositionNoInMesh = -2;
//传入的Buffer不够存放路点信息
const int PathFindErrorNotEnoughBufferForPathFind = -3;
//当前位置无法到达目标点
const int PathFindErrorTargetPositionUnarrivable = -4; 

extern UBI_MeshNavigateSystem* CreatePathFinder();


extern bool InitPathFinder(UBI_MeshNavigateSystem* sys, const char* configFile);

extern void ReleasePathFinder(UBI_MeshNavigateSystem* sys);

//返回值等于0时寻路成功
//返回值小于0时查找错误码
//返回值大于0时失败, 其值为路点使用的的float的个数
extern int findPath(UBI_MeshNavigateSystem* sys, float* pathBuffer, const int bufferSize, const float beginPosX, const float beginPosZ, const float targetX, const float targetZ);

#endif
