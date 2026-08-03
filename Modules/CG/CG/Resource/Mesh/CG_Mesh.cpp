#include "PCH/CG_PCH.h"
#include "CG_Mesh.h"

#include "CG/OpenGL/CG_GLMeshBuffer.h"

CG_Mesh::CG_Mesh()
    : CG_GPUResource<CG_Mesh>(CG_GPUDataPolicy::INVALID)
    , m_glData(nullptr)
    , m_vertexBuffer(nullptr)
    , m_indexBuffer(nullptr)
    , m_vertexBufferCount(0)
    , m_indexBufferCount(0)
    , m_triangleCount(0)
{}

CG_Mesh::CG_Mesh(CG_GPUDataPolicy::GPUDataPolicy p_dataPolicy)
    : CG_GPUResource<CG_Mesh>(p_dataPolicy)
    , m_glData(nullptr)
    , m_vertexBuffer(nullptr)
    , m_indexBuffer(nullptr)
    , m_vertexBufferCount(0)
    , m_indexBufferCount(0)
    , m_triangleCount(0)
{}

CG_Mesh::CG_Mesh(CG_Mesh&& p_otherMesh) noexcept
    : CG_GPUResource<CG_Mesh>(std::move(p_otherMesh))
    , m_glData(p_otherMesh.m_glData)
    , m_vertexBuffer(p_otherMesh.m_vertexBuffer)
    , m_indexBuffer(p_otherMesh.m_indexBuffer)
    , m_vertexBufferCount(p_otherMesh.m_vertexBufferCount)
    , m_indexBufferCount(p_otherMesh.m_indexBufferCount)
    , m_triangleCount(p_otherMesh.m_triangleCount)
{
    p_otherMesh.m_glData = nullptr;
    p_otherMesh.m_vertexBuffer = nullptr;
    p_otherMesh.m_indexBuffer = nullptr;

    p_otherMesh.m_vertexBufferCount = 0;
    p_otherMesh.m_indexBufferCount = 0;
    p_otherMesh.m_triangleCount = 0;
}

CG_Mesh& CG_Mesh::operator=(CG_Mesh&& p_otherMesh) noexcept
{
    if (this == &p_otherMesh)
    {
        return *this;
    }

    FlushResources();

    CG_GPUResource::operator=(std::move(p_otherMesh));

    YK_STEAL_MEMBER(m_glData, p_otherMesh, nullptr)
    YK_STEAL_MEMBER(m_vertexBuffer, p_otherMesh, nullptr)
    YK_STEAL_MEMBER(m_indexBuffer, p_otherMesh, nullptr)

    YK_STEAL_MEMBER(m_vertexBufferCount, p_otherMesh, 0)
    YK_STEAL_MEMBER(m_indexBufferCount, p_otherMesh, 0)
    YK_STEAL_MEMBER(m_triangleCount, p_otherMesh, 0)

    return *this;
}

void CG_Mesh::SetData(float* p_vertexBuffer,
                      YK_U32 p_vertexBufferCount,
                      YK_U32* p_indexBuffer,
                      YK_U32 p_indexBufferCount)
{
    // TODO: Assert when vertex data exists
    if (m_vertexBuffer)
    {
        // __debugbreak();
        return;
    }

    m_vertexBufferCount = p_vertexBufferCount;
    m_indexBufferCount = p_indexBufferCount;
    m_triangleCount = p_indexBufferCount / 3;

    // TODO: Steal the pointer instead of copying?
    m_vertexBuffer = new float[m_vertexBufferCount];
    m_indexBuffer = new YK_U32[m_indexBufferCount];

    memcpy(m_vertexBuffer, p_vertexBuffer, sizeof(float) * p_vertexBufferCount);
    memcpy(m_indexBuffer, p_indexBuffer, sizeof(YK_U32) * p_indexBufferCount);

    OnDataSet();
}

void CG_Mesh::UploadGPUDataImpl() { m_glData = new CG_GLMeshBuffer(*this); }

void CG_Mesh::FlushGPUDataImpl() { YK_SAFE_DELETE(m_glData); }

void CG_Mesh::FlushRAMDataImpl()
{
    YK_SAFE_DELETE_ARRAY(m_vertexBuffer);
    YK_SAFE_DELETE_ARRAY(m_indexBuffer);
}