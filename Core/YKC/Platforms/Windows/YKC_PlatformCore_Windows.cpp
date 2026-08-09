#include "PCH/YakuCore_PCH.h"
#if YK_PLATFORM == YK_WINDOWS
#include "YKC/Platforms/YKC_PlatformCore.h"

#include "YKC/IO/Logging/YKC_Logger.h"

#include <GLAD/include/glad/glad.h>
#include <GLFW/include/glfw3.h>

inline void loc_GLFWError(const char* errorMessage)
{
    const char* error;
    glfwGetError(&error);
    YK_LOG_ERROR_PARAM("GLFW Error: {} - {}", errorMessage, error);
    glfwTerminate();
}

bool YKC_PlatformCore::Init()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if !YAKU_RETAIL
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif // !YAKU_RETAIL

    // TODO: Move hardcoded size outta here
    m_displaySurface.Init(YK_Vector2i(800, 500));

    if (!m_displaySurface.IsValid())
    {
        loc_GLFWError("Failed to create window!");
        return false;
    }

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        loc_GLFWError("Failed to initialize GLAD!");
        return false;
    }

    // TODO: This probably shouldn't be here
    glViewport(0, 0, 800, 500);
    glClearColor(0.1133f, 0.1269f, 0.1122f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    return true;
}

void YKC_PlatformCore::ShutDown() { glfwTerminate(); }

void YKC_PlatformCore::LaunchCoreLoop(void (*p_coreLoop)(void*), void* p_contextData) const
{
    while (!m_displaySurface.ShouldClose())
    {
        p_coreLoop(p_contextData);
    }
}

void YKC_PlatformCore::OnFrameStart() const { glfwPollEvents(); }

#endif // YK_PLATFORM == YK_WINDOWS