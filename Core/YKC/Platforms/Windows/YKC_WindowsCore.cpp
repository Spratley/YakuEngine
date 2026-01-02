#include "PCH/YakuCore_PCH.h"
#if YK_WINDOWS
#include "YKC_WindowsCore.h"

#include <GLAD/include/glad/glad.h>
#include <GLFW/include/glfw3.h>

#include "YKC_WindowsWindow.h"

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

bool YKC_WindowsCore::Init()
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

	// TODO: Move the hardcoded size outta here
	m_mainWindow = new YKC_WindowsWindow(800, 500);

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

	// TODO: This probably shouldn't be here
	glViewport(0, 0, 800, 500);
	glClearColor(0.1133f, 0.1269f, 0.1122f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	return true;
}

void YKC_WindowsCore::ShutDown()
{
	glfwTerminate();
}

bool YKC_WindowsCore::ShouldClose() const
{
	return m_mainWindow->ShouldClose();
}

void YKC_WindowsCore::OnFrameStart() const
{
	glfwPollEvents();
}

#endif // YK_WINDOWS