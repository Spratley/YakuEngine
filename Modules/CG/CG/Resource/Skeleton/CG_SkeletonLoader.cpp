#include "PCH/CG_PCH.h"
#include "CG_SkeletonLoader.h"

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

CG_Skeleton CG_SkeletonLoader::Load(YK_FilePath const& p_skeletonPath)
{
    YK_ASSERT(p_skeletonPath.Extension() == "gltf" || p_skeletonPath.Extension() == "glb",
              "YakuEn only supports GLTF skeletons!");

    CG_GLTF::File gltfSkeleton(p_skeletonPath);
    if (gltfSkeleton.CheckErrors() || !gltfSkeleton.HasSkeleton())
    {
        return CG_Skeleton();
    }

    tg3_model const& model = gltfSkeleton.GetModel();
    tg3_skin const& skin = model.skins[0];

    if (skin.joints_count > 64)
    {
        YK_LOG_ERROR_PARAM("Skeleton {} has {} bones! YakuEn's hard limit is 64!",
                           p_skeletonPath.CString(),
                           skin.joints_count);
        return CG_Skeleton();
    }

    CG_GLTF::DataView<YK_Matrix44> const inverseBindMatrixBuffer =
      gltfSkeleton.ViewData<YK_Matrix44>(skin.inverse_bind_matrices);

    CG_Skeleton skeleton;
    skeleton.m_inverseBindMatrices.resize(inverseBindMatrixBuffer.m_count);
    std::memcpy(static_cast<void*>(skeleton.m_inverseBindMatrices.data()),
                static_cast<void const*>(inverseBindMatrixBuffer.m_buffer),
                sizeof(YK_Matrix44) * inverseBindMatrixBuffer.m_count);

    skeleton.m_bones.resize(skin.joints_count);

    std::unordered_map<YK_U32, YK_SizeT> nodeIDToBoneIndex;
    for (auto i : YK_CountTo(skin.joints_count))
    {
        nodeIDToBoneIndex[skin.joints[i]] = i;
    }

    for (auto i : YK_CountTo(skin.joints_count))
    {
        CG_Skeleton::Bone& bone = skeleton.m_bones[i];
        tg3_node const& joint = model.nodes[skin.joints[i]];
        bone.m_index = static_cast<YK_U8>(i);
#if !YAKU_RETAIL
        bone.m_name = std::string(joint.name.data);
#endif // !YAKU_RETAIL

        bone.m_bindPoseTransform.m_position = YK_Vector3f{ static_cast<float>(joint.translation[0]),
                                                           static_cast<float>(joint.translation[1]),
                                                           static_cast<float>(joint.translation[2]) };

        bone.m_bindPoseTransform.m_orientation = YK_Quaternion{ static_cast<float>(joint.rotation[0]),
                                                                static_cast<float>(joint.rotation[1]),
                                                                static_cast<float>(joint.rotation[2]),
                                                                static_cast<float>(joint.rotation[3]) };

        // Honestly not sure if bind pose can HAVE a scale? Keeping this anway for posterity sake
        bone.m_bindPoseTransform.m_scale = YK_Vector3f{ static_cast<float>(joint.scale[0]),
                                                        static_cast<float>(joint.scale[1]),
                                                        static_cast<float>(joint.scale[2]) };

        for (auto j : YK_CountTo(joint.children_count))
        {
            YK_SizeT childBoneIndex = nodeIDToBoneIndex[joint.children[j]];
            skeleton.m_bones[childBoneIndex].m_parent = &bone;
        }
    }

    return skeleton;
}