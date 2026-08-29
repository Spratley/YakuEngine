#pragma once

#include "CG/RenderTarget/CG_RenderTarget.h"
#include "CG/Renderer/2D/CG_2DRenderer.h"
#include "CG/Renderer/3D/CG_3DRenderer.h"
#include "CG/Renderer/CG_RenderBinding.h"

class YK_DisplaySurface;
struct CG_CameraComponent;

class CG_RenderModule
{
public:
    CG_RenderModule(YK_DisplaySurface& p_mainDisplay);

    void Render(CG_CameraComponent const& p_camera) const;

    CG_2DRenderer& Get2DRenderer() { return m_2DRenderer; }
    CG_2DRenderer const& Get2DRenderer() const { return m_2DRenderer; }

private:
    YK_DisplaySurface* m_display;
    CG_RenderTarget m_displayRenderTarget;

    CG_3DRenderer m_3DRenderer;
    CG_2DRenderer m_2DRenderer;

    mutable CG_RenderBinding m_renderBindingsCache;
};