#include "PCH/YakuEngine_PCH.h"
#if YK_WINDOWS
#include "YK_WindowsWindow.h"

#include <CG/Libraries/OpenGL/GLAD/include/glad/glad.h>
#include <CG/Libraries/OpenGL/GLFW/include/glfw3.h>

void loc_FramebufferResizeCallback(GLFWwindow* /*p_window*/, int p_width, int p_height)
{
	glViewport(0, 0, p_width, p_height);
}

YK_WindowsWindow::YK_WindowsWindow(YK_U32 p_width, YK_U32 p_height)
{
	m_glfwWindow = glfwCreateWindow(p_width, p_height, "YakuEngine Game", NULL, NULL);
	if (!m_glfwWindow)
	{
		return;
	}

	glfwSetFramebufferSizeCallback(m_glfwWindow, loc_FramebufferResizeCallback);
	glfwMakeContextCurrent(m_glfwWindow);
}

YK_WindowsWindow::~YK_WindowsWindow()
{
	if (m_glfwWindow)
	{
		glfwDestroyWindow(m_glfwWindow);
		m_glfwWindow = nullptr;
	}
}

bool YK_WindowsWindow::ShouldClose() const
{
	return glfwWindowShouldClose(m_glfwWindow);
}
#endif // YK_WINDOWS