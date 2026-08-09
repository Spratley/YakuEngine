namespace YK_Core_GLFWHelpers
{
    inline void LogGLFWError(const char* p_errorMessage)
    {
        const char* error;
        glfwGetError(&error);
        YK_LOG_ERROR_PARAM("GLFW Error: {} - {}", p_errorMessage, error);
        glfwTerminate();
    }

    bool Initialize(YK_DisplaySurface& p_displaySurface, YK_Vector2i const& p_dimensions)
    {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if !YAKU_RETAIL
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif // !YAKU_RETAIL

        p_displaySurface.Init(p_dimensions);

        if (!p_displaySurface.IsValid())
        {
            LogGLFWError("Failed to create window!");
            return false;
        }

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            LogGLFWError("Failed to initialize GLAD!");
            return false;
        }

        glViewport(0, 0, p_dimensions.x, p_dimensions.y);
        // TODO: This probably shouldn't be here
        glClearColor(0.1133f, 0.1269f, 0.1122f, 1.0f);
        glEnable(GL_DEPTH_TEST);
        return true;
    }
} // namespace YK_Core_GLFWHelpers