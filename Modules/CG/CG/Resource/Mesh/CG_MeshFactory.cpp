#include "PCH/CG_PCH.h"
#include "CG_MeshFactory.h"

#include "CG/GPU/CG_GPUDataPolicy.h"
#include "CG_Mesh.h"

#include "YK/Types/Math/YK_Integer.h"

CG_Mesh CG_MeshFactory::FromData(float* p_vertexData,
                                 YK_U32 p_vertexDataCount,
                                 YK_U32* p_indexData,
                                 YK_U32 p_indexCount)
{
    // TODO: Setup factory policy, don't just assume what the user wants
    CG_Mesh mesh(CG_GPUDataPolicy::FREE_RAM_DO_GPU_UPLOAD);
    mesh.SetData(p_vertexData, p_vertexDataCount, p_indexData, p_indexCount);
    return mesh;
}

CG_Mesh CG_MeshFactory::Quad()
{
    // Packed positions and UVs (X, Y, Z, U, V), right now there's no way to NOT have UVs
    // TODO: Make FromData() and CG_Mesh configurable so vertex attributes can change
    float vertices[] = { 0.5f,  0.5f,  0.0f, 1.0f, 1.0f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.0f,
                         -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, -0.5f, 0.5f,  0.0f, 0.0f, 1.0f };

    unsigned int indices[] = { 0, 1, 3, 1, 2, 3 };

    return FromData(vertices, 20, indices, 6);
}