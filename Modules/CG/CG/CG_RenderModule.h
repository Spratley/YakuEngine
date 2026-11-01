#pragma once

#include "YK/Libraries/Zen/Entity/Zen_Entity.h"

#include "CG/DebugDraw/CG_DebugDefines.h"
#include "CG/DebugDraw/CG_DebugDrawer.h"
#include "CG/RenderTarget/CG_RenderTarget.h"
#include "CG/Renderer/2D/CG_2DRenderer.h"
#include "CG/Renderer/3D/CG_3DRenderer.h"
#include "CG/Renderer/CG_RenderBinding.h"

class YK_DisplaySurface;

class CG_RenderModule
{
public:
    CG_RenderModule(YK_DisplaySurface& p_mainDisplay);

    void Render() const;

    CG_2DRenderer& Get2DRenderer() { return m_2DRenderer; }
    CG_2DRenderer const& Get2DRenderer() const { return m_2DRenderer; }

    void SetActiveCamera(Zen::Entity const& p_camera) { m_activeCamera = p_camera; }

private:
    YK_DisplaySurface* m_display;
    CG_RenderTarget m_displayRenderTarget;

    CG_3DRenderer m_3DRenderer;
    CG_2DRenderer m_2DRenderer;
    CG_IF_DEBUG_DRAW_ENABLED(CG_DebugDrawer m_debugDrawer);

    mutable CG_RenderBinding m_renderBindingsCache;

    Zen::Entity m_activeCamera;
};