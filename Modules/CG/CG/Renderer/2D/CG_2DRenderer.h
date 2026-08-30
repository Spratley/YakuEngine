#pragma once

#include "CG/2D/Canvas/CG_Canvas.h"
#include "CG/RenderTarget/CG_RenderTarget.h"

#include "YK/Types/Math/YK_Integer.h"

class CG_Shader;

class CG_2DRenderer
{
public:
    CG_2DRenderer();
    ~CG_2DRenderer() = default;

    void Render(CG_RenderTarget const& p_target) const;

    CG_Canvas& GetCanvas(YK_Int8 p_priorityIndex) { return m_canvases[p_priorityIndex]; }
    CG_Canvas const& GetCanvas(YK_Int8 p_priorityIndex) const { return m_canvases[p_priorityIndex]; }

private:
    void RenderToFramebuffer() const;

private:
    // This shouldn't be hard-coded
    CG_Canvas m_canvases[4];

    CG_Shader const* m_2DShader = nullptr;
    CG_Shader const* m_fsqShader = nullptr;

    CG_RenderTarget m_framebuffer;

    // Empty VAO since the 2D renderer has a built in quad
    YK_U32 m_nullVAO;
};