#include "PCH/YakuCore_PCH.h"

#if YK_PLATFORM == YK_WASM
#include "YK/Debugging/YK_Assert.h"
#include "YK/IO/Display/GLFW/YK_DisplaySurface_GLFW.hpp"
#include "YK/IO/Display/YK_DisplaySurfaceBase.h"
#include "YK/Types/Math/YK_Vector.h"

// Emscripten specific GL headers
#include <GLES3/gl3.h>
#include <GLFW/glfw3.h>
#include <emscripten.h>
#include <emscripten/html5.h>

#include "YK/IO/Display/GLFW/YK_DisplaySurface_GLFWHelpers.inl"

#include <cmath>

namespace YK_DisplaySurface_Private
{
    bool OnEmscriptenWindowResized(int /*p_eventType*/, EmscriptenUiEvent const* /*p_uiEvent*/, void* p_marshalledData)
    {
        GLFWwindow* glfwWindow = static_cast<GLFWwindow*>(p_marshalledData);
        YK_Vector_N<double, 2> cssPixelDimensions;
        emscripten_get_element_css_size("#canvas", &cssPixelDimensions.x, &cssPixelDimensions.y);
        double monitorPixelRatio = emscripten_get_device_pixel_ratio();
        YK_Vector2i screenPixelDimensions(static_cast<int>(std::round(cssPixelDimensions.x * monitorPixelRatio)),
                                          static_cast<int>(std::round(cssPixelDimensions.y * monitorPixelRatio)));
        emscripten_set_canvas_element_size("#canvas", screenPixelDimensions.x, screenPixelDimensions.y);
        glfwSetWindowSize(glfwWindow, screenPixelDimensions.x, screenPixelDimensions.y);
        return EM_TRUE;
    }

    void OnEmscriptenWindowResized(void* p_marshalledData) { OnEmscriptenWindowResized(0, nullptr, p_marshalledData); }
} // namespace YK_DisplaySurface_Private

template <>
YK_DisplaySurfaceBase<YK_DisplaySurface_GLFWData>::~YK_DisplaySurfaceBase()
{
    YK_DisplaySurface_GLFWHelpers::DestroyWindow(m_platformSpecificData);
}

template <>
bool YK_DisplaySurfaceBase<YK_DisplaySurface_GLFWData>::InitImpl(YK_Vector2i p_dimensions)
{
    YK_DisplaySurface_GLFWHelpers::CreateWindow(m_platformSpecificData, this, p_dimensions);
    emscripten_set_resize_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW,
                                   static_cast<void*>(m_platformSpecificData.m_glfwWindow),
                                   EM_FALSE,
                                   YK_DisplaySurface_Private::OnEmscriptenWindowResized);
    emscripten_async_call(YK_DisplaySurface_Private::OnEmscriptenWindowResized, m_platformSpecificData.m_glfwWindow, 0);
    return true;
}

template <>
bool YK_DisplaySurfaceBase<YK_DisplaySurface_GLFWData>::IsValid() const
{
    return m_platformSpecificData.m_glfwWindow != nullptr;
}

template <>
bool YK_DisplaySurfaceBase<YK_DisplaySurface_GLFWData>::ShouldClose() const
{
    return glfwWindowShouldClose(m_platformSpecificData.m_glfwWindow);
}

template <>
void YK_DisplaySurfaceBase<YK_DisplaySurface_GLFWData>::SwapBuffers() const
{
    glfwSwapBuffers(m_platformSpecificData.m_glfwWindow);
}

#endif // YK_PLATFORM == YK_WASM