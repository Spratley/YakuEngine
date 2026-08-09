#include "PCH/YakuCore_PCH.h"
#if YK_PLATFORM == YK_WASM
#include "YK/Platforms/YK_PlatformCore.h"

// Emscripten specific GL headers
#include <emscripten.h>
#include <GLFW/glfw3.h>
#include <GLES3/gl3.h>

inline void loc_GLFWError(const char* errorMessage)
{
    const char* error;
    //glfwGetError(&error);

    // TODO: Add string builder
    YK_LOG_ERROR("GLFW Error:");
    YK_LOG_ERROR(errorMessage);
    //YK_LOG_ERROR(error);

    glfwTerminate();
}

bool YK_PlatformCore::Init()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if !YAKU_RETAIL
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif // !YAKU_RETAIL

#if !YAKU_RETAIL
    if (m_displaySurface.IsValid())
    {
        YK_LOG_ERROR("Attempting to re-initialize main window!");
        // TODO: Assert
        return false;
    }
#endif // !YAKU_RETAIL

    // TODO: Move hardcoded size outta here
    m_displaySurface.Init(YK_Vector2i(800, 500));

    if (!m_displaySurface.IsValid())
    {
        loc_GLFWError("Failed to create window!");
        return false;
    }

    // TODO: This probably shouldn't be here
    glViewport(0, 0, 800, 500);
    glClearColor(0.1133f, 0.1269f, 0.1122f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    return true;
}

void YK_PlatformCore::ShutDown() { glfwTerminate(); }

void YK_PlatformCore::LaunchCoreLoop(void (*p_coreLoop)(void*), void* p_contextData) const
{
    emscripten_set_main_loop_arg(p_coreLoop, p_contextData, 0, 1);
}

void YK_PlatformCore::OnFrameStart() const { glfwPollEvents(); }

#endif // YK_PLATFORM == YK_WASM