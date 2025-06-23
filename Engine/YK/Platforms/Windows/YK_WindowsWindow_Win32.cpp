#include "PCH/YakuEngine_PCH.h"
#if YK_WINDOWS
#include "YK/Platforms/Windows/YK_WindowsWindow.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#include <CG/Libraries/OpenGL/GLFW/include/glfw3.h>
#include <CG/Libraries/OpenGL/GLFW/include/glfw3native.h>

void* YK_WindowsWindow::GetHWND() const
{
	return glfwGetWin32Window(m_glfwWindow);
}
#endif // YK_WINDOWS