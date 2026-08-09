#include "PCH/YakuCore_PCH.h"

#if YK_PLATFORM == YK_WINDOWS
#include "YK/Debugging/YK_Assert.h"
#include "YK/IO/Display/GLFW/YK_DisplaySurface_GLFW.hpp"
#include "YK/IO/Display/YK_DisplaySurfaceBase.h"
#include "YK/Types/Math/YK_Vector.h"

#include <GLAD/include/glad/glad.h>
#include <GLFW/include/glfw3.h>

#include "YK/IO/Display/GLFW/YK_DisplaySurface_GLFWHelpers.inl"

template <>
YK_DisplaySurfaceBase<YK_DisplaySurface_GLFWData>::~YK_DisplaySurfaceBase()
{
    YK_DisplaySurface_GLFWHelpers::DestroyWindow(m_platformSpecificData);
}

template <>
bool YK_DisplaySurfaceBase<YK_DisplaySurface_GLFWData>::InitImpl(YK_Vector2i p_dimensions)
{
    YK_DisplaySurface_GLFWHelpers::CreateWindow(m_platformSpecificData, this, p_dimensions);
    return true;
}

template <>
bool YK_DisplaySurfaceBase<YK_DisplaySurface_GLFWData>::IsValid() const
{
    return m_platformSpecificData.m_glfwWindow != nullptr;
}

template <>
bool YK_DisplaySurfaceBase<YK_DisplaySurface_GLFWData>::ShouldClose() const
{
    return glfwWindowShouldClose(m_platformSpecificData.m_glfwWindow);
}

template <>
void YK_DisplaySurfaceBase<YK_DisplaySurface_GLFWData>::SwapBuffers() const
{
    glfwSwapBuffers(m_platformSpecificData.m_glfwWindow);
}

#endif // YK_PLATFORM == YK_WINDOWS