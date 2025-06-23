#pragma once

// TODO: Don't couple CG and OpenGL/Windows

struct GLFWwindow;

class CG_RenderModule
{
public:
	CG_RenderModule(GLFWwindow* p_glfwWindow) : m_glfwWindow(p_glfwWindow) {}

	void Render() const;

private:
	// TODO: REPLACE THIS WITH A GENERIC RENDER TARGET
	GLFWwindow* m_glfwWindow;
};