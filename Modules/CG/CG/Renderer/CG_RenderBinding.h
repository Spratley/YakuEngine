#pragma once

class CG_Shader;
class CG_Material;
class CG_Texture;
class CG_Mesh;

// TODO: Find a way around boilerplate
class CG_RenderBinding
{
public:
    void Bind(CG_Shader const& p_shader);
    void Bind(CG_Material const& p_material);
    void Bind(CG_Texture const& p_texture);
    void Bind(CG_Mesh const& p_mesh);

    CG_Mesh const* GetBoundMesh() const { return m_boundMesh; }

    void TempInvalidate()
    {
        m_boundShader = nullptr;
        m_boundMaterial = nullptr;
        m_boundTexture = nullptr;
        m_boundMesh = nullptr;
    }

private:
    CG_Shader const* m_boundShader = nullptr;
    CG_Material const* m_boundMaterial = nullptr;
    CG_Texture const* m_boundTexture = nullptr;
    CG_Mesh const* m_boundMesh = nullptr;
};