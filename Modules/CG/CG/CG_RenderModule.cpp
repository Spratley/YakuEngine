#include "PCH/CG_PCH.h"
#include "CG/CG_RenderModule.h"

#include <GLAD/include/glad/glad.h>
#include <GLFW/include/glfw3.h>

// TEMP
#include "CG/Mesh/CG_MeshFactory.h"
#include "CG/OpenGL/CG_GLMeshBuffer.h"
#include "CG/Shader/CG_Shader.h"

void CG_RenderModule::TempInit()
{
	temp_quad = CG_MeshFactory::Quad();
	temp_shader = new CG_Shader("J:/TEMP/vertex.vs", "J:/TEMP/fragment.fs");
}

void CG_RenderModule::Render() const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	temp_shader->Use();
	temp_quad->GetGLData().Bind();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glfwSwapBuffers(m_glfwWindow);
}