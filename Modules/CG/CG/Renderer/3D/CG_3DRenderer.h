#pragma once

#include "CG/Renderer/CG_RenderQueue.h"
#include "CG/Resource/Shader/CG_Shader.h"

class CG_RenderBinding;

// Temp
class CG_Mesh;
class CG_Texture;
class CG_GLRenderTarget;

class YK_DisplaySurface;

// Temp
namespace Zen
{
    class Garden;
}

class CG_3DRenderer
{
public:
    CG_3DRenderer();
    ~CG_3DRenderer();

    void Temp_Init(YK_DisplaySurface& p_displaySurface);

    void Render(CG_RenderBinding& p_bindings, YK_Matrix44 const& p_view, Zen::Garden const& p_garden) const;

private:
    CG_Shader m_shader;
    mutable CG_RenderQueue m_renderQueue;
};