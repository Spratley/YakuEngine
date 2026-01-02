#include "PCH/YakuCore_PCH.h"
#if YK_WINDOWS
    #include "YKC_WindowsWindow.h"

    #include <GLAD/include/glad/glad.h>
    #include <GLFW/include/glfw3.h>

void loc_FramebufferResizeCallback(GLFWwindow* p_window, int p_width, int p_height)
{
    if (YKC_WindowsWindow* windowsWindow = static_cast<YKC_WindowsWindow*>(glfwGetWindowUserPointer(p_window)))
    {
        windowsWindow->OnWindowResized(YK_Vector2i(p_width, p_height));
    }
}

YKC_WindowsWindow::YKC_WindowsWindow(YK_U32 p_width, YK_U32 p_height)
{
    m_glfwWindow = glfwCreateWindow(p_width, p_height, "YakuEngine Game", NULL, NULL);
    if (!m_glfwWindow)
    {
        return;
    }

    glfwSetWindowUserPointer(m_glfwWindow, this);
    glfwSetFramebufferSizeCallback(m_glfwWindow, loc_FramebufferResizeCallback);
    glfwMakeContextCurrent(m_glfwWindow);
}

YKC_WindowsWindow::~YKC_WindowsWindow()
{
    if (m_glfwWindow)
    {
        glfwDestroyWindow(m_glfwWindow);
        m_glfwWindow = nullptr;
    }
}

bool YKC_WindowsWindow::ShouldClose() const
{
    return glfwWindowShouldClose(m_glfwWindow);
}
#endif // YK_WINDOWS