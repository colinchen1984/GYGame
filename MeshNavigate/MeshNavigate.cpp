/*
**�ļ����ƣ�MeshNavigate.cpp
**��������������Ѱ·�������ݽṹ
**�ļ�˵����
**��    �ߣ�����
**����ʱ�䣺2012-02-21
**��    �ģ�
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
	Point*			m_pointList;				// �����б�
	int				m_terrianVertexsCount;		// ���ζ����б�����
	int				m_maxPointCount;			// �����б���������
	int				m_currentPointCount;		// ��ǰ��������
	//������Ϣ
	int				m_sceneMaxX;				// ����X���ֵ
	int				m_sceneMaxZ;				// ����Z���ֵ
	int				m_terriaonVertexSpacing;	// ���εĶ�����
	//Ѱ·�����������Ϣ
	int				m_xZoneCount;				// x��ָ�����������
	int				m_zZoneCount;				// z��ָ�����������
	Zone*			m_gridZoneList;				// ���������б�
	int				m_zoneSize;					// ����Ĵ�С
	int				m_zoneCount;				// �������������������
	int				m_gridZoneCount;	
	Polygon*		m_polygonList;				// �����ڶ�����б�
	int				m_triangleCount;			// �����ڶ��������

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
	sys->m_maxPointCount = sys->m_terrianVertexsCount << 1; //��ʼ�����б�Ĵ�С�ǳ������������double
	sys->m_pointList = malloc(sizeof(Point) * sys->m_maxPointCount);
	if(NULL == sys->m_pointList)
	{
		return false;
	}
	
	memset(sys->m_pointList, 0, sizeof(Point) * sys->m_maxPointCount);
	
	//��ʼ����������������
}

#define GetDistence(ax, az, bx, bz) ((ax - bx) * (ax - bx) + (az - bz) * (az - bz))
int AddPointToSystem(MeshNavigateSystem* sys, float x, float z, bool checkRange)
{
	if(NULL == sys)
	{
		return -1;
	}
	
	//����Ѿ�װ��,���·����ڴ�
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

