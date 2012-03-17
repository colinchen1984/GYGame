/*
**�ļ����ƣ�MeshNavigate.h
**��������������Ѱ·�������ݽṹ
**�ļ�˵����
**��    �ߣ�����
**����ʱ�䣺2012-02-21
**��    �ģ�
*/

#pragma once

struct MeshNavigateSystem;


extern MeshNavigateSystem* CreateGridSystem();


extern bool InitGridSystem(MeshNavigateSystem* sys, int maxXSize, int maxZSize, int terriaonVertexSpacing);

extern int AddPointToSystem(MeshNavigateSystem* sys, float x, float z, bool checkRange);

extern void ReleaseMeshNavigatedSystem(MeshNavigateSystem* sys);

