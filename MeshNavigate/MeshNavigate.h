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


extern MeshNavigateSystem* CreateGridSystem();


extern bool InitGridSystem(MeshNavigateSystem* sys, int maxXSize, int maxZSize, int terriaonVertexSpacing);

extern int AddPointToSystem(MeshNavigateSystem* sys, float x, float z, bool checkRange);

extern void ReleaseMeshNavigatedSystem(MeshNavigateSystem* sys);

