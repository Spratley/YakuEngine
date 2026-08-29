#pragma once

#include "CG/2D/Canvas/CG_Canvas.h"
#include "YK/Types/Math/YK_Integer.h"

// Temp
class CG_RenderTarget;
class CG_Shader;
class CG_Texture;

class CG_2DRenderer
{
public:
    CG_2DRenderer();
    ~CG_2DRenderer();

    void Render() const;

    CG_Canvas& GetCanvas(YK_Int8 p_priorityIndex) { return m_canvases[p_priorityIndex]; }
    CG_Canvas const& GetCanvas(YK_Int8 p_priorityIndex) const { return m_canvases[p_priorityIndex]; }

private:
    void RenderToFramebuffer() const;

private:
    // TODO: Replace with a formal YK_StaticArray? Move to template paramter? Do something?
    CG_Canvas m_canvases[4];

    CG_Shader const* m_2DShader = nullptr;
    CG_Shader const* m_fsqShader = nullptr;

    CG_RenderTarget* m_renderTarget = nullptr; // I don't want this to be a pointer, it should be on board right?
};