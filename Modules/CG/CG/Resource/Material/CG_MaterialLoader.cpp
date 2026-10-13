#include "PCH/CG_PCH.h"
#include "CG_MaterialLoader.h"

#include "YK/Core/YK_Core.h"
#include "YK/Libraries/JParse/JParse.h"

#include "CG/Resource/Shader/CG_Shader.h"
#include "CG/Resource/Texture/CG_Texture.h"

// This is a very temporary loader while material files have zero complexity
CG_Material CG_MaterialLoader::Load(YK_FilePath const& p_materialPath)
{
    JParse::Root jsonData;
    jsonData.Parse(p_materialPath.CString());

    auto root = jsonData.GetRootItem<JParse::Object const>();
    auto const& shader = root->Get<JParse::String const>("shader")->m_value;
    auto const& texture = root->Get<JParse::String const>("texture")->m_value;

    YK_Core& engine = YK_Core::GetEngine();
    CG_Shader const& shaderAsset = engine.GetAssetManager().GetAsset<CG_Shader>(YK_FilePath(shader));
    CG_Texture const& textureAsset = engine.GetAssetManager().GetAsset<CG_Texture>(YK_FilePath(texture));

    return CG_Material{ .m_shader = &shaderAsset, .m_texture = &textureAsset };
}