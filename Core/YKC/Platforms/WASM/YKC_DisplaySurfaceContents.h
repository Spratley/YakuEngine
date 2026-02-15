#pragma once

#if YK_WEB_ASSEMBLY

class GLFWwindow;

struct YKC_DisplaySurfaceContents
{
    GLFWwindow* m_glfwWindow;
};

#endif // YK_WEB_ASSEMBLY