#include "PCH/YakuCore_PCH.h"
#if YK_PLATFORM == YK_WASM
#include "YK/Core/YK_Core.h"

// Emscripten specific GL headers
#include <GLES3/gl3.h>
#include <GLFW/glfw3.h>
#include <emscripten.h>

#include "YK/Core/GLFW/YK_Core_GLFWHelpers.inl"

bool YK_Core::Init() { return YK_Core_GLFWHelpers::Initialize(m_displaySurface, YK_Vector2i(800, 500)); }

void YK_Core::ShutDown() { glfwTerminate(); }

void YK_Core::LaunchCoreLoop(void (*p_coreLoop)(void*), void* p_contextData) const
{
    emscripten_set_main_loop_arg(p_coreLoop, p_contextData, 0, 1);
}

void YK_Core::OnFrameStart() const { glfwPollEvents(); }

#endif // YK_PLATFORM == YK_WASM