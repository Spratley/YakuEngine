#include "PCH/YakuCore_PCH.h"
#if YK_PLATFORM == YK_WINDOWS
#include "YK/Core/YK_Core.h"

#include "YK/IO/Logging/YK_Logger.h"

#include <GLAD/include/glad/glad.h>
#include <GLFW/include/glfw3.h>

#include "YK/Core/GLFW/YK_Core_GLFWHelpers.inl"

bool YK_Core::Init() { return YK_Core_GLFWHelpers::Initialize(m_displaySurface, YK_Vector2i(800, 600)); }

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