#pragma once

class CG_Mesh;

// TODO: Buffer ID pooling
// ...whatever that means
struct CG_GLMeshBuffer
{
public:
	CG_GLMeshBuffer() = delete;
	CG_GLMeshBuffer(CG_Mesh const& p_sourceMesh);
	~CG_GLMeshBuffer();

	void Bind() const;

private:
	YK_U32 m_vaoID;
	YK_U32 m_vboID;
	YK_U32 m_eboID;
};