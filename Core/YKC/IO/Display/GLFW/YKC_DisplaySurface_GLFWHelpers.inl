namespace YK_DisplaySurface_GLFWHelpers
{
    void FramebufferResizeCallback(GLFWwindow* p_window, int p_width, int p_height)
    {
        if (YK_DisplaySurface* displaySurface = static_cast<YK_DisplaySurface*>(glfwGetWindowUserPointer(p_window)))
        {
            displaySurface->OnSurfaceResized(YK_Vector2i(p_width, p_height));
        }
    }

    bool CreateWindow(YK_DisplaySurface_GLFWData& p_data, void* p_displaySurface, YK_Vector2i const& p_dimensions)
    {
        p_data.m_glfwWindow = glfwCreateWindow(p_dimensions.x, p_dimensions.y, "YakuEngine Game", NULL, NULL);
        YK_ASSERT(p_data.m_glfwWindow, "Failed to create a display surface!");

        glfwSetWindowUserPointer(p_data.m_glfwWindow, p_displaySurface);
        glfwSetFramebufferSizeCallback(p_data.m_glfwWindow, YK_DisplaySurface_GLFWHelpers::FramebufferResizeCallback);
        // TODO: Re-evaluate if this is okay to do if/when multiple windows becomes a thing
        // Immediately swapping context might not be for every situation
        glfwMakeContextCurrent(p_data.m_glfwWindow);
        return true;
    }

    void DestroyWindow(YK_DisplaySurface_GLFWData& p_data)
    {
        if (p_data.m_glfwWindow)
        {
            glfwDestroyWindow(p_data.m_glfwWindow);
            p_data.m_glfwWindow = nullptr;
        }
    }
} // namespace YK_DisplaySurface_GLFWHelpers