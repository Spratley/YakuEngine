namespace YK_Core_GLFWHelpers
{
    inline void LogGLFWError(char const* p_errorMessage)
    {
        char const* error;
#if YK_PLATFORM != YK_WASM
        glfwGetError(&error);
#endif // YK_PLATFORM != YK_WASM
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
        return true;
    }
} // namespace YK_Core_GLFWHelpers