#include "PCH/YakuEngine_PCH.h"
#if YK_WINDOWS
#include "YK_WindowsCore.h"

#include <CG/Libraries/OpenGL/GLAD/include/glad/glad.h>
#include <CG/Libraries/OpenGL/GLFW/include/glfw3.h>

#include "YK_WindowsWindow.h"

inline void loc_GLFWError(const char* errorMessage)
{
	const char* error;
	glfwGetError(&error);

	// TODO: String builder
	YK_LOG_ERROR("GLFW Error:");
	YK_LOG_ERROR(errorMessage);
	YK_LOG_ERROR(error);
	
	glfwTerminate();
}

bool YK_WindowsCore::Init()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if !YAKU_RETAIL
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif // !YAKU_RETAIL

#if !YAKU_RETAIL
	if (m_mainWindow)
	{
		YK_LOG_ERROR("Attempting to re-initialize main window!");
		return false;
	}
#endif // !YAKU_RETAIL

	m_mainWindow = new YK_WindowsWindow(800, 500);

	if (!m_mainWindow->IsValid())
	{
		loc_GLFWError("Failed to create window!");
		return false;
	}

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		loc_GLFWError("Failed to initialize GLAD!");
		return false;
	}

	glViewport(0, 0, 800, 500);
	glClearColor(0.5f, 1.0f, 0.2f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	return true;
}

void YK_WindowsCore::ShutDown()
{
	glfwTerminate();
}

bool YK_WindowsCore::ShouldClose() const
{
	return m_mainWindow->ShouldClose();
}

void YK_WindowsCore::OnFrameStart() const
{
	glfwPollEvents();
}

#endif // YK_WINDOWS