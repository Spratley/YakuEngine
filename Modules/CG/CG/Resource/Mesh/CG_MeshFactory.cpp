#include "PCH/CG_PCH.h"
#include "CG_MeshFactory.h"

#include "YK/Types/Math/YK_Integer.h"

#include "CG/GPU/CG_GPUDataPolicy.h"
#include "CG_Mesh.h"
#include "CG_MeshLayout.h"

CG_Mesh CG_MeshFactory::FromData(YK_Byte const* p_vertexData,
                                 YK_U32 p_vertexDataCount,
                                 YK_U32 const* p_indexData,
                                 YK_U32 p_indexCount,
                                 CG_MeshLayout p_meshLayout)
{
    // TODO: Setup factory policy, don't just assume what the user wants
    CG_Mesh mesh(CG_GPUDataPolicy::FREE_RAM_DO_GPU_UPLOAD);
    mesh.SetData(p_vertexData, p_vertexDataCount, p_indexData, p_indexCount, p_meshLayout);
    return mesh;
}

CG_Mesh CG_MeshFactory::Quad()
{
    // Packed position and UV data
    constexpr float vertices[] = { 0.5f,  0.5f,  0.0f, 1.0f, 1.0f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.0f,
                                   -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, -0.5f, 0.5f,  0.0f, 0.0f, 1.0f };
    constexpr unsigned int indices[] = { 0, 1, 3, 1, 2, 3 };
    constexpr CG_MeshLayout meshLayout{ CG_MeshAttribute::POSITION, CG_MeshAttribute::UV };
    return FromData(reinterpret_cast<YK_Byte const*>(vertices), 20, indices, 6, meshLayout);
}