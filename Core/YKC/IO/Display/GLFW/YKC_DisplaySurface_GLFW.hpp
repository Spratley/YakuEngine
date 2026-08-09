#pragma once
#include "YKC/Platforms/YKC_PlatformDefines.h"
#if YK_PLATFORM == YK_WINDOWS || YK_PLATFORM == YK_WASM

#include "YKC/IO/Display/YKC_DisplaySurfaceBase.h"

struct GLFWwindow;
struct YK_DisplaySurface_GLFWData
{
    GLFWwindow* m_glfwWindow = nullptr;
};

class YK_DisplaySurface : public YK_DisplaySurfaceBase<YK_DisplaySurface_GLFWData>
{};

#endif // YK_PLATFORM == YK_WINDOWS || YK_PLATFORM == YK_WASM