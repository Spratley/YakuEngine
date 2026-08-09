#pragma once

class CG_Shader;

class CG_ShaderLoader
{
public:
    static CG_Shader Load(const char* p_vertexPath, const char* p_fragmentPath)
    {
        return CG_Shader(p_vertexPath, p_fragmentPath);
    }
};

//template <>
//struct YK_ResourceTraits<CG_Shader>
//{
//    using Loader = CG_ShaderLoader;
//};
//
//using ShaderResources = YK_ResourceContainer<CG_Shader>;