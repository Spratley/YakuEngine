#include "PCH/YakuCore_PCH.h"

#if YK_WINDOWS
#include "YKC_WindowsWindow.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/include/glfw3.h>
#include <GLFW/include/glfw3native.h>

void* YKC_WindowsWindow::GetHWND() const
{
	return glfwGetWin32Window(m_glfwWindow);
}
#endif // YK_WINDOWS