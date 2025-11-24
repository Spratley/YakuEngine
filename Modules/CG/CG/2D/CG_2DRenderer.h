#pragma once

#include "CG/2D/Canvas/CG_Canvas.h"
#include "CG/Shader/CG_Shader.h"

class CG_2DRenderer
{
public:
    CG_2DRenderer();

    void Render() const;

    CG_Canvas& GetCanvas(YK_Int8 p_priorityIndex) { return m_canvases[p_priorityIndex]; }
    CG_Canvas const& GetCanvas(YK_Int8 p_priorityIndex) const { return m_canvases[p_priorityIndex]; }

private:
    // TODO: Replace with a formal YK_StaticArray? Move to template paramter? Do something?
    CG_Canvas m_canvases[4];

    CG_Shader m_2DRShader;

    // TODO: Replace with ARI API (Agnostic Renderer Interface)
    YK_

};