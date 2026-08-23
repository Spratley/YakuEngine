#include "PCH/YakuCore_PCH.h"
#if YK_PLATFORM == YK_WINDOWS
#include "YK/Core/YK_Core.h"

#include <GLAD/include/glad/glad.h>
#include <GLFW/include/glfw3.h>

#include "YK/Core/GLFW/YK_Core_GLFWHelpers.inl"

constexpr YK_Vector2i g_initialScreenSize = YK_Vector2i(800, 600);

bool YK_Core::Init()
{
    if (!YK_Core_GLFWHelpers::Initialize(m_displaySurface, g_initialScreenSize))
    {
        return false;
    }

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        YK_Core_GLFWHelpers::LogGLFWError("Failed to initialize GLAD!");
        return false;
    }

    // TODO: This probably shouldn't be here
    glViewport(0, 0, g_initialScreenSize.x, g_initialScreenSize.y);
    glClearColor(0.1133f, 0.1269f, 0.1122f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    return true;
}

void YK_Core::ShutDown() { glfwTerminate(); }

void YK_Core::LaunchCoreLoop(void (*p_coreLoop)(void*), void* p_contextData) const
{
    while (!m_displaySurface.ShouldClose())
    {
        p_coreLoop(p_contextData);
    }
}

void YK_Core::OnFrameStart() const { glfwPollEvents(); }

#endif // YK_PLATFORM == YK_WINDOWS