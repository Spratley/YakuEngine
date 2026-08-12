#include "PCH/CG_PCH.h"
#include "CG/Renderer/CG_RenderBinding.h"

#include "CG/Material/CG_Material.h"
#include "CG/OpenGL/CG_GLMeshBuffer.h"
#include "CG/OpenGL/CG_GLTextureBuffer.h"
#include "CG/Resource/Mesh/CG_Mesh.h"
#include "CG/Resource/Shader/CG_Shader.h"
#include "CG/Resource/Texture/CG_Texture.h"

// TODO: Migrate all binding logic into here

void CG_RenderBinding::Bind(CG_Shader const& p_shader)
{
    if (m_boundShader != &p_shader)
    {
        p_shader.Use();
        m_boundShader = &p_shader;
    }
}

void CG_RenderBinding::Bind(CG_Material const& p_material) 
{
    if (m_boundMaterial != &p_material && p_material.m_texture && p_material.m_shader) 
    {
        Bind(*p_material.m_shader);
        Bind(*p_material.m_texture);
        m_boundMaterial = &p_material;
    }
}

// TODO: Slot indexing
void CG_RenderBinding::Bind(CG_Texture const& p_texture)
{
    if (m_boundTexture != &p_texture)
    {
        p_texture.GetGLData().Bind(0);
        m_boundTexture = &p_texture;
    }
}

void CG_RenderBinding::Bind(CG_Mesh const& p_mesh)
{
    if (m_boundMesh != &p_mesh)
    {
        p_mesh.GetGLData().Bind();
        m_boundMesh = &p_mesh;
    }
}