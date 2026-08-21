#pragma once

#include "CG/Resource/Mesh/CG_Mesh.h"

class CG_MeshFactory
{
public:
	static CG_Mesh* LoadOBJ(char const* p_path);

	static CG_Mesh* FromData(float* p_vertexData, YK_U32 p_vertexDataCount, YK_U32* p_indexData, YK_U32 p_indexCount);

	static CG_Mesh* Quad();
};

struct CG_MeshLoader
{
    static CG_Mesh* Load(char const* p_path) { return CG_MeshFactory::LoadOBJ(p_path); }
};