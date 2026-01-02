#pragma once
#if YK_WINDOWS

struct GLFWwindow;

class YKC_WindowsWindow
{
public:
    YKC_WindowsWindow(YK_U32 p_width, YK_U32 p_height);
    ~YKC_WindowsWindow();

    bool IsValid() const { return m_glfwWindow != nullptr; }
    bool ShouldClose() const;

    GLFWwindow* GetGLFWWindow() const { return m_glfwWindow; }
    void* GetHWND() const;

    void SetWindowResizedCallback(void (*p_Callback)(YK_Vector2i)) { OnWindowResizedCallback = p_Callback; }
    void OnWindowResized(YK_Vector2i p_size) const
    {
        if (OnWindowResizedCallback)
        {
            OnWindowResizedCallback(p_size);
        }
    }

private:
    GLFWwindow* m_glfwWindow = nullptr;

    // TODO: Come back to this
    void (*OnWindowResizedCallback)(YK_Vector2i p_size) = nullptr;
};
#endif // YK_WINDOWS