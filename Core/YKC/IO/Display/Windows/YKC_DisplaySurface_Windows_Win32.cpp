#include "PCH/YakuCore_PCH.h"

#if YK_PLATFORM == YK_WINDOWS
#include "YKC/IO/Display/GLFW/YKC_DisplaySurface_GLFW.hpp"
#include "YKC/IO/Display/YKC_DisplaySurfaceBase.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/include/glfw3.h>
#include <GLFW/include/glfw3native.h>

template <>
void* YK_DisplaySurfaceBase<YK_DisplaySurface_GLFWData>::GetNativeHandle() const
{
    return glfwGetWin32Window(m_platformSpecificData.m_glfwWindow);
}

#endif // YK_PLATFORM == YK_WINDOWS