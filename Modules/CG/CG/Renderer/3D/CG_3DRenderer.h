#pragma once

class CG_RenderBinding;
class CG_RenderTarget;
struct CG_Camera;

class YK_DisplaySurface;

class CG_3DRenderer
{
public:
    void Render(CG_RenderTarget const& p_target, CG_RenderBinding& p_bindings, CG_Camera const& p_camera) const;
};