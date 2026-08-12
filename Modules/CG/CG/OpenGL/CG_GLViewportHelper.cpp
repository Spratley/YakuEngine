#include "PCH/CG_PCH.h"
#include "CG_GLViewportHelper.h"

#if YK_PLATFORM == YK_WASM
// Emscripten specific GL headers
#include <GLES3/gl3.h>
#include <GLFW/glfw3.h>
#include <emscripten.h>
#else
#include <YK/Libraries/OpenGL/GLAD/include/glad/glad.h>
#include <YK/Libraries/OpenGL/GLFW/include/glfw3.h>
#endif

void CG_GLViewportHelper::SetViewportSize(YK_Vector2i p_viewportSize)
{
    if (s_viewportSize != p_viewportSize)
    {
        s_viewportSize = p_viewportSize;
        glViewport(0, 0, p_viewportSize.x, p_viewportSize.y);
    }
}