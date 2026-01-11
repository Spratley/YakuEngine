#pragma once

#include "CG/2D/CG_2DRenderer.h"
#include "YKC/Resource/YKC_ResourceManager.h"

// TODO: Don't couple CG and OpenGL/Windows
struct GLFWwindow;

// Temp
class CG_Mesh;
class CG_Shader;
class CG_Texture;

// More Temp
class CG_ShaderLoader
{
public:
    static CG_Shader Load(const char* p_vertexPath, const char* p_fragmentPath)
    {
        YK_UNUSED(p_vertexPath);
        YK_UNUSED(p_fragmentPath);
        YK_LOG("Calling VS and FS loader!");
        return CG_Shader(p_vertexPath, p_fragmentPath);
    }
    static CG_Shader Load(const char* p_path)
    {
        YK_UNUSED(p_path);
        YK_LOG("Calling single path loader!");
        return CG_Shader();
    }
};

template <>
struct YKC_ResourceTraits<CG_Shader>
{
    using Loader = CG_ShaderLoader;
};

YK_DECLARE_HANDLE(CG_Shader);

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
    using ShaderResources = YKC_ResourceContainer<CG_Shader>;
    YKC_ResourceManager</*MeshResources,*/ ShaderResources> m_cgResources;

    // TEMP
    CG_Mesh* temp_quad;
    // CG_Shader* temp_shader;
    CG_ShaderHandle shader;
    CG_Texture* temp_texture;
};