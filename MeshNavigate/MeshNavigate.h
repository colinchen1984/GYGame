/*
**文件名称：MeshNavigate.h
**功能描述：网格寻路基础数据结构
**文件说明：
**作    者：陈琳
**创建时间：2012-02-21
**修    改：
*/

#pragma once

struct MeshNavigateSystem;
struct ItemNavigateMesh;

extern MeshNavigateSystem* CreateGridSystem();


extern bool InitGridSystem(MeshNavigateSystem* sys, int maxXSize, int maxZSize, int terriaonVertexSpacing, const float* heightData, const int heightDataCount);

extern int AddPointToSystem(MeshNavigateSystem* sys, float x, float z, bool checkRange);

extern void ReleaseMeshNavigatedSystem(MeshNavigateSystem* sys);

//item 不需要调用函数得到其凸包
//item在成功添加后不需要再释放其内存, 有sys管理其生命周期
extern bool AddItemToGridSystem(MeshNavigateSystem* sys, ItemNavigateMesh* item);

extern bool MakeMeshNavigateData(MeshNavigateSystem* sys);

extern Queue* GetItemQueue(MeshNavigateSystem* sys);
