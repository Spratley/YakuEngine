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

#include "CG/Matrix/CG_MatrixExtras.h"

YK_Matrix44 g_perspective;

void CG_RenderModule::TempInit()
{
	CG_TextureFactory::Init();

	temp_quad = CG_MeshFactory::Quad();
	temp_shader = new CG_Shader("J:/Harbourfront/Data/Shaders/ShaderCode/vertex.vs", "J:/Harbourfront/Data/Shaders/ShaderCode/fragment.fs");
	temp_texture = CG_TextureFactory::LoadPNG("J:/Harbourfront/Data/Textures/Test.png");

	g_perspective = YK_Matrix::Perspective<float>(60.0f, 1.0f, 0.0001f, 100.0f);
}

void CG_RenderModule::Render(YK_Matrix44 const& p_renderMatrix) const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	temp_shader->Use();


	YK_Matrix44 transform = p_renderMatrix * g_perspective;
	temp_shader->SetMatrix44("transform", transform.GetData());
	
	
	temp_texture->GetGLData().Bind(1);
	temp_quad->GetGLData().Bind();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glfwSwapBuffers(m_glfwWindow);
}