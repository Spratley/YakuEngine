#include "PCH/CG_PCH.h"
#include "CG/Mesh/CG_MeshFactory.h"

CG_Mesh* CG_MeshFactory::FromData(float* p_vertexData, YK_U32 p_vertexDataCount, YK_U32* p_indexData, YK_U32 p_indexCount)
{
	// TODO: Setup factory policy
	CG_Mesh* mesh = new CG_Mesh(CG_GPUDataPolicy::FREE_RAM_DO_GPU_UPLOAD);
	mesh->SetData(p_vertexData, p_vertexDataCount, p_indexData, p_indexCount);
	return mesh;
}

CG_Mesh* CG_MeshFactory::Quad()
{
	float vertices[] = 
	{
		// Positions		// UVs
		 0.5f,  0.5f, 0.0f,	1.0f, 1.0f,
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
	};

	unsigned int indices[] =
	{
		0, 1, 3,
		1, 2, 3
	};

	return FromData(vertices, 20, indices, 6);
}
