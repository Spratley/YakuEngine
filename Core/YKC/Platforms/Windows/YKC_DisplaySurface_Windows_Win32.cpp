#include "PCH/YakuCore_PCH.h"

#if YK_WINDOWS
#include "YKC/Platforms/YKC_DisplaySurface.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/include/glfw3.h>
#include <GLFW/include/glfw3native.h>

void* YKC_DisplaySurface::GetNativeHandle() const { return glfwGetWin32Window(m_contents.m_glfwWindow); }

#endif // YK_WINDOWS