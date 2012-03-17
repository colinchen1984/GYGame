/*
**文件名称：MeshNavigate.cpp
**功能描述：网格寻路基础数据结构
**文件说明：
**作    者：陈琳
**创建时间：2012-02-21
**修    改：
*/


#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include <math.h>

struct Zone
{
	Polygon**	  	m_gridList;
	int maxGridListCount;
};


static const int MN_INVALID_VALUE = -1;

struct MeshNavigateSystem
{
	Point*			m_pointList;				// 顶点列表
	int				m_terrianVertexsCount;		// 地形顶点列表数量
	int				m_maxPointCount;			// 顶点列表的最大容量
	int				m_currentPointCount;		// 当前顶点数量
	//场景信息
	int				m_sceneMaxX;				// 场景X最大值
	int				m_sceneMaxZ;				// 场景Z最大值
	int				m_terriaonVertexSpacing;	// 地形的顶点间隔
	//寻路的区域管理信息
	int				m_xZoneCount;				// x轴分割的区域的数量
	int				m_zZoneCount;				// z轴分割的区域的数量
	Zone*			m_gridZoneList;				// 场景区域列表
	int				m_zoneSize;					// 区域的大小
	int				m_zoneCount;				// 整个场景的区域的数量
	int				m_gridZoneCount;	
	Polygon*		m_polygonList;				// 场景内多边形列表
	int				m_triangleCount;			// 场景内多边形数量

};

MeshNavigateSystem* CreateGridSystem();
{
	return (MeshNavigateSystem*)malloc(sizeof(MeshNavigateSystem));
}

bool InitGridSystem(MeshNavigateSystem* sys, int maxXSize, int maxZSize, int terriaonVertexSpacing)
{
	if(NULL == sys)
	{
		return false;
	}
	memset(sys, 0, sizeof(*sys));
	if(0 != maxXSize % 2)
	{
		return false;
	}
	
	if(0 != maxZSize % 2)
	{
		return false;
	}
	
	if(0 != maxXSize % terriaonVertexSpacing)
	{
		return false;
	}
	
	if(0 != maxZSize % terriaonVertexSpacing)
	{
		return false;
	}
	
	sys->m_sceneMaxX = maxXSize;
	sys->m_sceneMaxZ = maxZSize;
	sys->m_terriaonVertexSpacing  = terriaonVertexSpacing;
	sys->m_terrianVertexsCount = (maxXSize / terriaonVertexSpacing) * (maxZSize / terriaonVertexSpacing);
	sys->m_maxPointCount = sys->m_terrianVertexsCount << 1; //初始化是列表的大小是场景顶点个数的double
	sys->m_pointList = malloc(sizeof(Point) * sys->m_maxPointCount);
	if(NULL == sys->m_pointList)
	{
		return false;
	}
	
	memset(sys->m_pointList, 0, sizeof(Point) * sys->m_maxPointCount);
	
	//初始化区域管理相关数据
}

#define GetDistence(ax, az, bx, bz) ((ax - bx) * (ax - bx) + (az - bz) * (az - bz))
int AddPointToSystem(MeshNavigateSystem* sys, float x, float z, bool checkRange)
{
	if(NULL == sys)
	{
		return -1;
	}
	
	//如果已经装满,重新分配内存
	if(sys->m_currentPointCount + 1 == sys->m_maxPointCount)
	{
		sys->m_maxPointCount = sys->m_maxPointCount << 1;
		sys->m_pointList = (Point*)realloc(sys->m_pointList, sizeof(Point) * sys->m_maxPointCount);
		if(NULL == sys->m_pointList)
		{
			return -1;
		}
		memset(&sys->m_pointList[sys->m_currentPointCount + 1], 0, sizeof(Point) * (sys->m_maxPointCount - sys->m_currentPointCount - 1)); 
	}
	int result = -1;
	if(true == checkRange)
	{
		for(int i = 0; i < sys->m_currentPointCount; ++i)
		{
			const static float minRange = 0.01 * 0.01;
			if(GetDistence(x, z, sys->m_pointList[i].x, sys->m_pointList[i].z) < minRange)
			{
				result = i;
				break;
			}
		}

	}
	
	if(-1 == result)
	{
		result = sys->m_currentPointCount++;
		Point* p = &sys->m_pointList[result];
		p->x = x;
		p->z = z;
	}
		
	return result;
}

