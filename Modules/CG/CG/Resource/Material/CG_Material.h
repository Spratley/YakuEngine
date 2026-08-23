#pragma once

class CG_Shader;
class CG_Texture;

class CG_Material
{
public:
    CG_Shader const* m_shader;
    CG_Texture const* m_texture;
};