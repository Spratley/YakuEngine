#pragma once

#include "CG/2D/CG_2DRenderer.h"
#include "YKC/Resource/YKC_ResourceManager.h"

#include "CG/Resource/Shader/CG_ShaderResource.h"

// TODO: Don't couple CG and OpenGL/Windows
struct GLFWwindow;

// Temp
class CG_Mesh;
class CG_Texture;

class CG_RenderModule
{
public:
    CG_RenderModule(GLFWwindow* p_glfwWindow) : m_glfwWindow(p_glfwWindow) { TempInit(); }

    void TempInit();

    void Render(YK_Matrix44 const& p_renderMatrix) const;

private:
    // TODO: REPLACE THIS WITH A GENERIC RENDER TARGET
    // Not platform agnostic, and not friendly to non-screen rendering
    GLFWwindow* m_glfwWindow;

    CG_2DRenderer m_2dRenderer;

private:
    using MeshResources = YKC_ResourceContainer<CG_Mesh>;
    YKC_ResourceManager</*MeshResources,*/ ShaderResources> m_cgResources;

    // TEMP
    CG_Mesh* temp_quad;
    CG_ShaderHandle shader;
    CG_Texture* temp_texture;
};