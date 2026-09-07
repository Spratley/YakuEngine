#pragma once

#include "YK/Types/Math/YK_Integer.h"

#include "CG/GPU/CG_GPUDataPolicy.h"
#include "CG/GPU/CG_GPUResource.h"
#include "CG/Resource/Mesh/CG_MeshLayout.h"

// TODO: Decouple mesh and OpenGL
struct CG_GLMeshBuffer;

class CG_Mesh : public CG_GPUResource<CG_Mesh>
{
    friend class CG_MeshFactory;
    friend class CG_GPUResource<CG_Mesh>;

public:
    CG_Mesh();
    CG_Mesh(CG_Mesh&& p_otherMesh) noexcept;
    // TODO: Destructor

    CG_Mesh& operator=(CG_Mesh& p_otherMesh) = delete;
    CG_Mesh& operator=(CG_Mesh&& p_otherMesh) noexcept;

    inline bool IsValid() const { return m_vertexBuffer || m_glData; } // Check if there is at least SOME data

    float* GetVertexBuffer() const { return m_vertexBuffer; }
    YK_U32 GetVertexBufferSize() const { return m_vertexBufferCount; }

    YK_U32* GetIndexBuffer() const { return m_indexBuffer; }
    YK_U32 GetIndexBufferSize() const { return m_indexBufferCount; }

    YK_U32 GetTriangleCount() const { return m_triangleCount; }

    CG_GLMeshBuffer const& GetGLData() const { return *m_glData; }

    CG_MeshLayout& GetMeshLayout() { return m_meshLayout; }
    CG_MeshLayout const& GetMeshLayout() const { return m_meshLayout; }

private:
    CG_Mesh(CG_GPUDataPolicy::GPUDataPolicy p_dataPolicy);

    void SetData(float const* p_vertexBuffer,
                 YK_U32 p_vertexBufferCount,
                 YK_U32 const* p_indexBuffer,
                 YK_U32 p_indexBufferCount,
                 CG_MeshLayout p_meshLayout);

    // CG_GPUResource implementations
    bool HasDataImpl() const { return m_vertexBuffer && m_indexBuffer; }
    bool HasGPUDataImpl() const { return m_glData; }

    void UploadGPUDataImpl();
    void FlushGPUDataImpl();
    void FlushRAMDataImpl();

private:
    CG_GLMeshBuffer* m_glData;

    float* m_vertexBuffer;
    YK_U32* m_indexBuffer;

    YK_U32 m_vertexBufferCount;
    YK_U32 m_indexBufferCount;
    YK_U32 m_triangleCount;

    CG_MeshLayout m_meshLayout;
};