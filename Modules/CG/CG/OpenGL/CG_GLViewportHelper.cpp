#include "PCH/CG_PCH.h"
#include "CG_GLViewportHelper.h"

#include <YKC/Libraries/OpenGL/GLAD/include/glad/glad.h>
#include <YKC/Libraries/OpenGL/GLFW/include/glfw3.h>

YK_Vector2i CG_GLViewportHelper::s_viewportSize = YK_Vector2i(800, 500); // TODO: Remove hardcoded

void CG_GLViewportHelper::SetViewportSize(YK_Vector2i p_viewportSize)
{
    if (s_viewportSize != p_viewportSize)
    {
        s_viewportSize = p_viewportSize;
        glViewport(0, 0, p_viewportSize.x, p_viewportSize.y);
    }
}