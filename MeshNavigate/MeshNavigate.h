/*
**�ļ����ƣ�MeshNavigate.h
**��������������Ѱ·�������ݽṹ
**�ļ�˵����
**��    �ߣ�����
**����ʱ�䣺2012-02-21
**��    �ģ�
*/

#pragma once

struct MeshNavigateGenerator;
struct ItemNavigateMesh;
struct MeshPolygon;
struct Point;

extern MeshNavigateGenerator* CreateMeshNavigateGenarator();


extern bool InitMeshNavigateGenarator(MeshNavigateGenerator* sys, int maxXSize, int maxZSize, int terriaonVertexSpacing, const float* heightData, const int heightDataCount);

extern int AddPointToSystem(MeshNavigateGenerator* sys, float x, float z, bool checkRange);

extern void ReleaseMeshNavigateGenarator(MeshNavigateGenerator* sys);

//item ����Ҫ���ú����õ���͹��
//item�ڳɹ���Ӻ���Ҫ���ͷ����ڴ�, ��sys��������������
extern bool AddItemToGridSystem(MeshNavigateGenerator* sys, ItemNavigateMesh* item);

extern bool MakeMeshNavigateData(MeshNavigateGenerator* sys);

extern int GetZoneCount(MeshNavigateGenerator* sys);

extern int GetMeshCountInZone(MeshNavigateGenerator* sys, int index);

extern MeshPolygon* GetMeshPolygonInZone(MeshNavigateGenerator* sys, int zoneIndex, int meshIndex);

extern int GetVertexCount(MeshNavigateGenerator* sys);

extern const Point* GetVertexList(MeshNavigateGenerator* sys);

extern bool WriteDataToFile(MeshNavigateGenerator* sys, const char* fileName);
