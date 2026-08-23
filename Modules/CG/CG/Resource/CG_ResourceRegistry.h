#pragma once

#include "CG/Resource/Material/CG_Material.h"
#include "CG/Resource/Material/CG_MaterialLoader.h"
#include "CG/Resource/Mesh/CG_Mesh.h"
#include "CG/Resource/Mesh/CG_MeshFactory.h"
//#include "CG/Resource/Mesh/CG_MeshLoader.h"
#include "CG/Resource/Shader/CG_Shader.h"
#include "CG/Resource/Shader/CG_ShaderLoader.h"
#include "CG/Resource/Texture/CG_Texture.h"
#include "CG/Resource/Texture/CG_TextureFactory.h"
//#include "CG/Resource/Texture/CG_TextureLoader.h"

#include "YK/IO/Asset/YK_AssetManager.h"

namespace CG_ResourceRegistry
{
    void RegisterAssetTypes(YK_AssetManager& p_assetManager)
    {
        p_assetManager.RegisterType<CG_Mesh, CG_MeshLoader>();
        p_assetManager.RegisterType<CG_Shader, CG_ShaderLoader>();
        p_assetManager.RegisterType<CG_Texture, CG_TextureLoader>();
        p_assetManager.RegisterType<CG_Material, CG_MaterialLoader>();
    }
} // namespace CG_ResourceRegistry