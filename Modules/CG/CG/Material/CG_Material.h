#pragma once

class CG_Shader;
class CG_Texture;

class CG_Material
{
public:
    CG_Shader* m_shader;
    CG_Texture* m_texture;
};