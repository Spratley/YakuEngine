#include "PCH/CG_PCH.h"
#include "CG_ShaderLoader.h"

#include "YK/IO/File/YK_FilePath.h"
#include "YK/IO/Logging/YK_Logger.h"
#include "YK/Libraries/JParse/JParse.h"

#include "CG/Resource/Shader/CG_Shader.h"

CG_Shader CG_ShaderLoader::Load(YK_FilePath const& p_shaderPath)
{
    if (p_shaderPath.Extension() != "YKS")
    {
        YK_LOG_ERROR_PARAM("Non-YakuShader file passed into CG_ShaderLoader!\n{}", p_shaderPath.CString());
        // TODO: Hard-coded error shader
        return CG_Shader{};
    }
    JParse::Root jsonData;
    jsonData.Parse(p_shaderPath.CString());
    auto root = jsonData.GetRootItem<JParse::Object const>();

    auto const& vertex = root->Get<JParse::String const>("vertex")->m_value;
    auto const& fragment = root->Get<JParse::String const>("fragment")->m_value;
    auto isSkeletal = root->TryGetValue<bool>("skeletal");

    YK_FilePath vertexPath(vertex);
    YK_FilePath fragmentPath(fragment);

    return CG_Shader(vertexPath.CString(), fragmentPath.CString(), isSkeletal);
}