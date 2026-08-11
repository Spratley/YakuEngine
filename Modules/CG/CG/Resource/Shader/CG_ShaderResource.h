#pragma once

class CG_Shader;

class CG_ShaderLoader
{
public:
    static CG_Shader Load(char const* p_vertexPath, char const* p_fragmentPath)
    {
        return CG_Shader(p_vertexPath, p_fragmentPath);
    }
};