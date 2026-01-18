#pragma once

#include "CG/2D/Canvas/CG_Canvas.h"
#include "CG/Resource/Shader/CG_Shader.h"

// Temp
class CG_Mesh;
class CG_Texture;
class CG_GLRenderTarget;

class CG_2DRenderer
{
public:
    CG_2DRenderer();
    ~CG_2DRenderer();

    void Temp_Init();

    void Render() const;

    CG_Canvas& GetCanvas(YK_Int8 p_priorityIndex) { return m_canvases[p_priorityIndex]; }
    CG_Canvas const& GetCanvas(YK_Int8 p_priorityIndex) const { return m_canvases[p_priorityIndex]; }

private:
    void RenderToFramebuffer() const;

private:
    // TODO: Replace with a formal YK_StaticArray? Move to template paramter? Do something?
    CG_Canvas m_canvases[4];

    CG_Shader m_2DShader;
    CG_Shader m_fsqShader;

    CG_GLRenderTarget* m_renderTarget; // I don't want this to be a pointer, it should be on board right?

    // Temp until I can find a better way to do this
    CG_Mesh* m_tempQuad;
};