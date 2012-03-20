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
struct ItemNavigateMesh;

extern MeshNavigateSystem* CreateGridSystem();


extern bool InitGridSystem(MeshNavigateSystem* sys, int maxXSize, int maxZSize, int terriaonVertexSpacing, const float* heightData, const int heightDataCount);

extern int AddPointToSystem(MeshNavigateSystem* sys, float x, float z, bool checkRange);

extern void ReleaseMeshNavigatedSystem(MeshNavigateSystem* sys);

//item ����Ҫ���ú����õ���͹��
//item�ڳɹ���Ӻ���Ҫ���ͷ����ڴ�, ��sys��������������
extern bool AddItemToGridSystem(MeshNavigateSystem* sys, ItemNavigateMesh* item);

extern bool MakeMeshNavigateData(MeshNavigateSystem* sys);

extern Queue* GetItemQueue(MeshNavigateSystem* sys);
