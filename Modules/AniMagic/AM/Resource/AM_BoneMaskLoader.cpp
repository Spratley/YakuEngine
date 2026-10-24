#include "PCH/AniMagic_PCH.h"
#include "AM_BoneMaskLoader.h"

#include "YK/IO/File/YK_FilePath.h"
#include "YK/IO/GLTF/YK_GLTF.h"
#include "YK/IO/Logging/YK_Logger.h"
#include "YK/Libraries/JParse/JParse.h"
#include "YK/Libraries/TinyGLTF/tiny_gltf_v3.h"
#include "YK/Types/Math/YK_Integer.h"
#include "YK/Utils/YK_AlgorithmUtils.h"

#include "AM/Resource/AM_BoneMask.h"

#include <string>

AM_BoneMask AM_BoneMaskLoader::Load(YK_FilePath const& p_boneMaskPath)
{
    if (p_boneMaskPath.Extension() != "AMBoneMask")
    {
        YK_LOG_ERROR("Attempting to load non-bone mask file as bone mask!\n{}", p_boneMaskPath.CString());
        return AM_BoneMask{};
    }

    JParse::Root testJSON;
    testJSON.Parse(p_boneMaskPath.CString());

    auto root = testJSON.GetRootItem<JParse::Object const>();
    auto skeletonPathStub = root->Get<JParse::String const>("skeleton");
    auto bonesInMask = root->Get<JParse::Array const>("bones");

    YK_GLTF::File skeletonFile(YK_FilePath(skeletonPathStub->m_value));
    tg3_model const& model = skeletonFile.GetModel();
    tg3_skin const& skin = model.skins[0];

    AM_BoneMask result;
    for (auto i : YK_CountTo(bonesInMask->m_contents.size()))
    {
        auto boneToMask = bonesInMask->Get<JParse::String>(i);

        for (auto boneIndex : YK_CountTo(skin.joints_count))
        {
            tg3_node const& bone = model.nodes[skin.joints[boneIndex]];
            std::string boneName{ bone.name.data, bone.name.len };
            if (boneName == boneToMask->m_value)
            {
                result.Enable(static_cast<YK_U8>(boneIndex));
            }
        }
    }

    return result;
}