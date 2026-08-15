#pragma once

#include "CG/Renderer/2D/CG_2DRenderer.h"
#include "CG/Renderer/3D/CG_3DRenderer.h"
#include "CG/Renderer/CG_RenderBinding.h"

class YK_DisplaySurface;
struct CG_CameraComponent;

// Temp
class CG_Mesh;
class CG_Shader;
class CG_Texture;

// Temp
namespace Zen
{
    class Garden;
}

class CG_RenderModule
{
public:
    CG_RenderModule(YK_DisplaySurface& p_mainDisplay)
        : m_display(&p_mainDisplay)
    {
        TempInit(p_mainDisplay);
    }

    void TempInit(YK_DisplaySurface& p_displaySurface);

    void Render(CG_CameraComponent const& p_camera, Zen::Garden const& p_entityGarden) const;

private:
    YK_DisplaySurface* m_display;

    CG_2DRenderer m_2dRenderer;
    CG_3DRenderer m_3dRenderer;

    mutable CG_RenderBinding m_renderBindingsCache;
};