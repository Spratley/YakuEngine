#pragma once

#include "YK/IO/Asset/YK_AssetHandle.h"

#include "CG/Resource/Animation/CG_Animation.h"
#include "CG/Resource/Material/CG_Material.h"
#include "CG/Resource/Mesh/CG_Mesh.h"
#include "CG/Resource/Shader/CG_Shader.h"
#include "CG/Resource/Skeleton/CG_Skeleton.h"
#include "CG/Resource/Texture/CG_Texture.h"

using CG_AnimationHandle = YK_AssetHandle<CG_Animation>;
using CG_MaterialHandle = YK_AssetHandle<CG_Material>;
using CG_MeshHandle = YK_AssetHandle<CG_Mesh>;
using CG_ShaderHandle = YK_AssetHandle<CG_Shader>;
using CG_SkeletonHandle = YK_AssetHandle<CG_Skeleton>;
using CG_TextureHandle = YK_AssetHandle<CG_Texture>;