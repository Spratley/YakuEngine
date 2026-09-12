#include "PCH/CG_PCH.h"
#include "CG_AnimationLoader.h"

#include "YK/Debugging/YK_Assert.h"
#include "YK/IO/File/YK_FilePath.h"
#include "YK/IO/Logging/YK_Logger.h"
#include "YK/Types/Math/YK_Integer.h"
#include "YK/Types/Math/YK_Matrix.h"
#include "YK/Types/Math/YK_Quaternion.h"
#include "YK/Types/Math/YK_Vector.h"
#include "YK/Utils/YK_AlgorithmUtils.h"

#include "CG/IO/CG_GLTF.h"
#include "CG/Libraries/TinyGLTF/tiny_gltf_v3.h"
#include "CG/Resource/Skeleton/CG_Skeleton.h"

#include <cstring>
#include <string>
#include <unordered_map>

// TODO: Skeletons are usually saved as part of the same mesh file
// There should be a compound-load function to get both, so we don't have to open the fiie twice when loading one asset

CG_Animation CG_AnimationLoader::Load(YK_FilePath const& p_animationPath)
{
    YK_ASSERT(p_animationPath.Extension() == "gltf" || p_animationPath.Extension() == "glb",
              "YakuEn only supports GLTF animations!");

    CG_GLTF gltfAnimation(p_animationPath);
    if (gltfAnimation.CheckErrors() || !gltfAnimation.HasSkeleton() || !gltfAnimation.HasAnimation())
    {
        YK_ASSERT(gltfAnimation.HasSkeleton(), "YakuEn only supports animation via skeletons!");
        return CG_Animation();
    }

    // Right now this only loads the first animation
    tg3_model const& model = gltfAnimation.GetModel();
    tg3_animation const& animation = model.animations[0];
    YK_Unused(animation);

    return CG_Animation();
}