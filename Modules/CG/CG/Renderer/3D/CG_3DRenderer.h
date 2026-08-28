#pragma once

#include "CG/Renderer/CG_RenderQueue.h"

class CG_RenderBinding;
struct CG_CameraComponent;

class YK_DisplaySurface;

class CG_3DRenderer
{
public:
    CG_3DRenderer(YK_DisplaySurface& p_displaySurface);
    ~CG_3DRenderer() = default;

    void Render(CG_RenderBinding& p_bindings, CG_CameraComponent const& p_camera) const;
};