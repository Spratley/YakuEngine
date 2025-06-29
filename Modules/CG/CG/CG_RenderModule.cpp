#include "PCH/CG_PCH.h"
#include "CG_RenderModule.h"

#include <GLAD/include/glad/glad.h>
#include <GLFW/include/glfw3.h>

// TEMP
#include "CG/Mesh/CG_MeshFactory.h"
#include "CG/OpenGL/CG_GLMeshBuffer.h"
#include "CG/OpenGL/CG_GLTextureBuffer.h"
#include "CG/Shader/CG_Shader.h"
#include "CG/Texture/CG_TextureFactory.h"

void CG_RenderModule::TempInit()
{
	CG_TextureFactory::Init();

	temp_quad = CG_MeshFactory::Quad();
	temp_shader = new CG_Shader("J:/Harbourfront/Data/Shaders/ShaderCode/vertex.vs", "J:/Harbourfront/Data/Shaders/ShaderCode/fragment.fs");
	temp_texture = CG_TextureFactory::LoadPNG("J:/Harbourfront/Data/Textures/Splash/YakuEn_Logo_Dark.png");
}

void CG_RenderModule::Render() const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	temp_shader->Use();
	temp_texture->GetGLData().Bind(1);
	temp_quad->GetGLData().Bind();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glfwSwapBuffers(m_glfwWindow);
}