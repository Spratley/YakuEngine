#pragma once

#include "CG/2D/CG_2DRenderer.h"
#include "CG/Resource/Shader/CG_ShaderResource.h"

class YK_DisplaySurface;

// Temp
class CG_Mesh;
class CG_Shader;
class CG_Texture;

// Temp
namespace Zen
{
    class Garden;
}

// The API communication point to launch any renderer based activities
// Probably shouldn't actually be doing any of the rendering itself

class CG_RenderModule
{
public:
    CG_RenderModule(YK_DisplaySurface& p_mainDisplay)
        : m_display(&p_mainDisplay)
    {
        TempInit(p_mainDisplay);
    }

    void TempInit(YK_DisplaySurface& p_displaySurface);

    void Render(YK_Matrix44 const& p_viewMatrix, Zen::Garden const& p_entityGarden) const;

private:
    // TODO: REPLACE THIS WITH A GENERIC RENDER TARGET?
    // This is locked into purely drawing to the screen right now
    // In order to do advanced render stuff (Deferred lighting etc)
    // I need to be able to draw to an arbitrary target
    YK_DisplaySurface* m_display;

    CG_2DRenderer m_2dRenderer;

private:
    // TEMP
    CG_Mesh* temp_quad;
    CG_Shader* shader;
    CG_Texture* temp_texture;
};