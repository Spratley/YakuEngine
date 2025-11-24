#include "PCH/CG_PCH.h"
#include "CG_2DRenderer.h"

// TODO: Encapsulate into rendering wrapper so that we can swap renderers
#include <GLAD/include/glad/glad.h>
#include <GLFW/include/glfw3.h>

#include "CG/2D/Canvas/CG_Canvas.h"

CG_2DRenderer::CG_2DRenderer()
    : m_canvases()
    , m_2DRShader("J:/Harbourfront/Data/Shaders/ShaderCode/2DR_Vertex.vs",
                  "J:/Harbourfront/Data/Shaders/ShaderCode/2DR_Fragment.fs")
{}

void CG_2DRenderer::Render() const
{
    // TODO: Refactor into batched rendering
    for (CG_Canvas const& canvas : m_canvases)
    {
        for (CG_CanvasTextureItem const& textureItem : canvas.GetItems())
        {
            // TODO: Make this not like just straight up in the renderer
        }
    }
}