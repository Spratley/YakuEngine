#pragma once

#include "CG/GPU/CG_GPUResource.h"

// TODO: Decouple mesh and OpenGL
struct CG_GLMeshBuffer;

class CG_Mesh : public CG_GPUResource<CG_Mesh>
{
	friend class CG_MeshFactory;
	friend class CG_GPUResource<CG_Mesh>;
public:
	CG_Mesh();
	// TODO: Destructor

	CG_Mesh& operator=(CG_Mesh& p_otherMesh) = delete;
	CG_Mesh& operator=(CG_Mesh&& p_otherMesh) noexcept;

	inline bool IsValid() const { return m_vertexBuffer || m_glData; } // Check if there is at least SOME data

	float* GetVertexBuffer() const { return m_vertexBuffer; }
	YK_U32 GetVertexBufferSize() const { return m_vertexBufferCount; }

	YK_U32* GetIndexBuffer() const { return m_indexBuffer; }
	YK_U32 GetIndexBufferSize() const { return m_indexBufferCount; }

	CG_GLMeshBuffer const& GetGLData() const { return *m_glData; }

private:
	CG_Mesh(CG_GPUDataPolicy::GPUDataPolicy p_dataPolicy);
	CG_Mesh(CG_Mesh&& p_otherMesh) noexcept;

	void SetData(float* p_vertexBuffer, YK_U32 p_vertexBufferCount, YK_U32* p_indexBuffer, YK_U32 p_indexBufferCount);

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
};