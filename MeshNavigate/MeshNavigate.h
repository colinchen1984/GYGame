/*
**文件名称：MeshNavigate.h
**功能描述：网格寻路基础数据结构
**文件说明：
**作    者：陈琳
**创建时间：2012-02-21
**修    改：
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

//item 不需要调用函数得到其凸包
//item在成功添加后不需要再释放其内存, 有sys管理其生命周期
extern bool AddItemToGridSystem(MeshNavigateGenerator* sys, ItemNavigateMesh* item);

extern bool MakeMeshNavigateData(MeshNavigateGenerator* sys);

extern int GetZoneCount(MeshNavigateGenerator* sys);

extern int GetMeshCount(MeshNavigateGenerator* sys);

extern MeshPolygon* GetMeshPolygon(MeshNavigateGenerator* sys, int meshIndex);

extern int GetVertexCount(MeshNavigateGenerator* sys);

extern const Point* GetVertexList(MeshNavigateGenerator* sys);

extern bool WriteDataToFile(MeshNavigateGenerator* sys, const char* fileName);

extern void BeginTest(MeshNavigateGenerator* sys);

extern void AddPointForTest(MeshNavigateGenerator* sys, float x, float z);

extern Queue* Endtest(MeshNavigateGenerator* sys, float maxx, float maxz);
