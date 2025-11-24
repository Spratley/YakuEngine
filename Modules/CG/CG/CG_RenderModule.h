#pragma once

#include "CG/2D/CG_2DRenderer.h"

// TODO: Don't couple CG and OpenGL/Windows
struct GLFWwindow;

// Temp
class CG_Mesh;
class CG_Shader;
class CG_Texture;

class CG_RenderModule
{
public:
	CG_RenderModule(GLFWwindow* p_glfwWindow) : m_glfwWindow(p_glfwWindow) { TempInit(); }

	void TempInit();

	void Render(YK_Matrix44 const& p_renderMatrix) const;

private:
	// TODO: REPLACE THIS WITH A GENERIC RENDER TARGET
	// Not platform agnostic, and not friendly to non-screen rendering
	GLFWwindow* m_glfwWindow;

	CG_2DRenderer m_2dRenderer;

	// TEMP
	CG_Mesh* temp_quad;
	CG_Shader* temp_shader;
	CG_Texture* temp_texture;
};