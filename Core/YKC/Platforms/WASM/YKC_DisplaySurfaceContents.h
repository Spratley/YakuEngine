#pragma once

#if YK_PLATFORM == YK_WASM

class GLFWwindow;

struct YKC_DisplaySurfaceContents
{
    GLFWwindow* m_glfwWindow;
};

#endif // YK_PLATFORM == YK_WASM