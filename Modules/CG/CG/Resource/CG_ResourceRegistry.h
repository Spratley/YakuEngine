#pragma once

#include "CG/Resource/Animation/CG_Animation.h"
#include "CG/Resource/Animation/CG_AnimationLoader.h"
#include "CG/Resource/Material/CG_Material.h"
#include "CG/Resource/Material/CG_MaterialLoader.h"
#include "CG/Resource/Mesh/CG_Mesh.h"
#include "CG/Resource/Mesh/CG_MeshLoader.h"
#include "CG/Resource/Shader/CG_Shader.h"
#include "CG/Resource/Shader/CG_ShaderLoader.h"
#include "CG/Resource/Skeleton/CG_Skeleton.h"
#include "CG/Resource/Skeleton/CG_SkeletonLoader.h"
#include "CG/Resource/Texture/CG_Texture.h"
#include "CG/Resource/Texture/CG_TextureLoader.h"

#include "YK/IO/Asset/YK_AssetManager.h"

namespace CG_ResourceRegistry
{
    void RegisterAssetTypes(YK_AssetManager& p_assetManager)
    {
        p_assetManager.RegisterType<CG_Animation, CG_AnimationLoader>();
        p_assetManager.RegisterType<CG_Material, CG_MaterialLoader>();
        p_assetManager.RegisterType<CG_Mesh, CG_MeshLoader>();
        p_assetManager.RegisterType<CG_Shader, CG_ShaderLoader>();
        p_assetManager.RegisterType<CG_Skeleton, CG_SkeletonLoader>();
        p_assetManager.RegisterType<CG_Texture, CG_TextureLoader>();

        CG_TextureLoader::Init();
    }
} // namespace CG_ResourceRegistry