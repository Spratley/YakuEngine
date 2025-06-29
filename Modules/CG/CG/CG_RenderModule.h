#pragma once

// TODO: Don't couple CG and OpenGL/Windows
struct GLFWwindow;

// Temp
class CG_Mesh;
class CG_Shader;

class CG_RenderModule
{
public:
	CG_RenderModule(GLFWwindow* p_glfwWindow) : m_glfwWindow(p_glfwWindow) { TempInit(); }

	void TempInit();

	void Render() const;

private:
	// TODO: REPLACE THIS WITH A GENERIC RENDER TARGET
	GLFWwindow* m_glfwWindow;

	// TEMP
	CG_Mesh* temp_quad;
	CG_Shader* temp_shader;
};