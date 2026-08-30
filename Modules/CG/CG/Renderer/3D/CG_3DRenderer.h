#pragma once

class CG_RenderBinding;
class CG_RenderTarget;
struct CG_CameraComponent;

class YK_DisplaySurface;

class CG_3DRenderer
{
public:
    CG_3DRenderer(YK_DisplaySurface& p_displaySurface);
    ~CG_3DRenderer() = default;

    void Render(CG_RenderTarget const& p_target,
                CG_RenderBinding& p_bindings,
                CG_CameraComponent const& p_camera) const;
};