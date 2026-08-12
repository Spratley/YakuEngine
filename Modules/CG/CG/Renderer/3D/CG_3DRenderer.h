#pragma once

#include "CG/Renderer/CG_RenderQueue.h"
#include "CG/Resource/Shader/CG_Shader.h"

class CG_RenderBinding;
struct CG_CameraComponent;

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
    CG_3DRenderer() = default;
    ~CG_3DRenderer() = default;

    void Temp_Init(YK_DisplaySurface& p_displaySurface);

    void Render(CG_RenderBinding& p_bindings, CG_CameraComponent const& p_camera, Zen::Garden const& p_garden) const;

private:
    mutable CG_RenderQueue m_renderQueue;
};