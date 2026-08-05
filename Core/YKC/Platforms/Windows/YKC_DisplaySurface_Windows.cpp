#include "PCH/YakuCore_PCH.h"

#if YK_PLATFORM == YK_WINDOWS
#include "YKC/Platforms/YKC_DisplaySurface.h"

#include <GLAD/include/glad/glad.h>
#include <GLFW/include/glfw3.h>

void loc_FramebufferResizeCallback(GLFWwindow* p_window, int p_width, int p_height)
{
    if (YKC_DisplaySurface* displaySurface = static_cast<YKC_DisplaySurface*>(glfwGetWindowUserPointer(p_window)))
    {
        displaySurface->OnWindowResized(YK_Vector2i(p_width, p_height));
    }
}

YKC_DisplaySurface::~YKC_DisplaySurface()
{
    if (m_contents.m_glfwWindow)
    {
        glfwDestroyWindow(m_contents.m_glfwWindow);
        m_contents.m_glfwWindow = nullptr;
    }
}

bool YKC_DisplaySurface::InitImpl(YK_Vector2i p_dimensions)
{
    m_contents.m_glfwWindow = glfwCreateWindow(p_dimensions.x, p_dimensions.y, "YakuEngine Game", NULL, NULL);
    if (!m_contents.m_glfwWindow)
    {
        YK_LOG_ERROR("Failed to create display surface!");
        // TODO: Assert
        return false;
    }

    glfwSetWindowUserPointer(m_contents.m_glfwWindow, this);
    glfwSetFramebufferSizeCallback(m_contents.m_glfwWindow, loc_FramebufferResizeCallback);
    // TODO: Re-evaluate if this is okay to do when multiple windows becomes a thing
    // Immediately swapping context might not be for every situation
    glfwMakeContextCurrent(m_contents.m_glfwWindow);
    return true;
}

bool YKC_DisplaySurface::IsValid() const { return m_contents.m_glfwWindow != nullptr; }
bool YKC_DisplaySurface::ShouldClose() const { return glfwWindowShouldClose(m_contents.m_glfwWindow); }
void YKC_DisplaySurface::SwapBuffers() const { glfwSwapBuffers(m_contents.m_glfwWindow); }

#endif // YK_PLATFORM == YK_WINDOWS