#include "PCH/CG_PCH.h"
#include "CG_GLMeshBuffer.h"

#include <GLAD/include/glad/glad.h>
#include <GLFW/include/glfw3.h>

#include "CG/Mesh/CG_Mesh.h"

CG_GLMeshBuffer::CG_GLMeshBuffer(CG_Mesh const& p_sourceMesh)
{
	// TODO: Come back and remember what GL_STATIC_DRAW is and if that needs to be parametrized
	glGenVertexArrays(1, &m_vaoID);
	glBindVertexArray(m_vaoID);

	glGenBuffers(1, &m_vboID);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * p_sourceMesh.GetVertexBufferSize(), p_sourceMesh.GetVertexBuffer(), GL_STATIC_DRAW);

	glGenBuffers(1, &m_eboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(YK_U32) * p_sourceMesh.GetIndexBufferSize(), p_sourceMesh.GetIndexBuffer(), GL_STATIC_DRAW);

	// TODO: Make vertex attributes mesh-driven
	constexpr size_t stride = 5 * sizeof(float); // 3 position attributes, 2 UV
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

CG_GLMeshBuffer::~CG_GLMeshBuffer()
{
	glDeleteBuffers(1, &m_eboID);
	glDeleteBuffers(1, &m_vboID);
	glDeleteVertexArrays(1, &m_vaoID);
}

void CG_GLMeshBuffer::Bind() const
{
	glBindVertexArray(m_vaoID);
}