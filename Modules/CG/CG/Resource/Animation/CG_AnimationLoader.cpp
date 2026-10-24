#include "PCH/CG_PCH.h"
#include "CG_AnimationLoader.h"

#include "YK/Debugging/YK_Assert.h"
#include "YK/IO/File/YK_FilePath.h"
#include "YK/IO/GLTF/YK_GLTF.h"
#include "YK/IO/Logging/YK_Logger.h"
#include "YK/Libraries/JParse/JParse.h"
#include "YK/Libraries/TinyGLTF/tiny_gltf_v3.h"
#include "YK/Math/YK_FloatMath.h"
#include "YK/Math/YK_MathUtils.h"
#include "YK/Types/Math/YK_Integer.h"
#include "YK/Types/Math/YK_Quaternion.h"
#include "YK/Types/Math/YK_Vector.h"
#include "YK/Utils/YK_AlgorithmUtils.h"

#include "CG/Resource/Animation/CG_Animation.h"

#include <cstring>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>

// TODO: Skeletons are usually saved as part of the same mesh file
// There should be a compound-load function to get both, so we don't have to open the fiie twice when loading one asset

namespace CG_AnimationLoader_Private
{
    YK_SizeT FindAnimation(YK_GLTF::File const& p_file, std::string const& p_animationName)
    {
        if (p_animationName == "")
        {
            return 0;
        }

        tg3_model const& model = p_file.GetModel();
        for (auto i : YK_CountTo(model.animations_count))
        {
            tg3_animation const& animation = model.animations[i];

            if (p_animationName == std::string(animation.name.data, animation.name.len))
            {
                return i;
            }
        }
        YK_LOG_ERROR("Failed to find animation [{}] in file", p_animationName);
        return 0;
    }

    enum AnimationChannel
    {
        POSITION,
        ORIENTATION,
        SCALE,
    };

    AnimationChannel GetChannelFromString(const char* p_channelName)
    {
        if (std::strcmp(p_channelName, "translation") == 0)
        {
            return AnimationChannel::POSITION;
        }
        if (std::strcmp(p_channelName, "rotation") == 0)
        {
            return AnimationChannel::ORIENTATION;
        }
        if (std::strcmp(p_channelName, "scale") == 0)
        {
            return AnimationChannel::SCALE;
        }
        YK_LOG_ERROR("Unknown animation channel requested! {}", p_channelName);
        return static_cast<AnimationChannel>(-1);
    }

    template <typename ChannelDataType, typename ExtractionType = ChannelDataType>
    CG_Animation::Channel<ChannelDataType> ExtractChannel(YK_GLTF::File const& p_gltfData,
                                                          tg3_animation const& p_gltfAnimation,
                                                          std::unordered_map<YK_U32, YK_U8> p_nodeIDToBoneIndex,
                                                          YK_SizeT p_channelIndex,
                                                          double& p_outDuration)
    {
        tg3_animation_channel const& channel = p_gltfAnimation.channels[p_channelIndex];
        tg3_animation_sampler const& sampler = p_gltfAnimation.samplers[channel.sampler];

        YK_U8 boneIndex = p_nodeIDToBoneIndex[channel.target.node];
        YK_GLTF::DataView<float> const keys = p_gltfData.ViewData<float>(sampler.input);

        CG_Animation::Channel<ChannelDataType> resultChannel;
        resultChannel.m_keyframes.resize(keys.m_count);
        resultChannel.m_boneIndex = boneIndex;

        YK_GLTF::DataView<ExtractionType> const values = p_gltfData.ViewData<ExtractionType>(sampler.output);
        for (auto keyIndex : YK_CountTo(keys.m_count))
        {
            resultChannel.m_keyframes[keyIndex] =
              std::pair<float, ChannelDataType>{ keys.m_buffer[keyIndex], values.m_buffer[keyIndex] };
        }
        p_outDuration = YK_Max(p_outDuration, static_cast<double>(resultChannel.m_keyframes.back().first));
        return resultChannel;
    }

    bool IsRoot(tg3_animation_channel const& p_channel, YK_GLTF::File const& p_file)
    {
        tg3_node const& bone = p_file.GetModel().nodes[p_channel.target.node];
        return std::strcmp("Root", bone.name.data) == 0;
    }
} // namespace CG_AnimationLoader_Private

CG_Animation CG_AnimationLoader::Load(YK_FilePath const& p_animationPath)
{
    if (p_animationPath.Extension() == "gltf" || p_animationPath.Extension() == "glb")
    {
        return LoadFromGLTF(p_animationPath, "");
    }

    if (p_animationPath.Extension() != "YKA")
    {
        YK_LOG_ERROR("Non-YakuAnimation file passed into CG_AnimationLoader!\n{}", p_animationPath.CString());
        return CG_Animation{};
    }

    JParse::Root jsonData;
    jsonData.Parse(p_animationPath.CString());
    auto root = jsonData.GetRootItem<JParse::Object const>();
    auto const& gltfFilePath = root->Get<JParse::String const>("file")->m_value;
    auto const& animationName = root->Get<JParse::String const>("animation")->m_value;
    return LoadFromGLTF(YK_FilePath(gltfFilePath), animationName);
}

CG_Animation CG_AnimationLoader::LoadFromGLTF(YK_FilePath const& p_animationPath,
                                              std::string const& p_animationName,
                                              bool m_ignoreRootMotion)
{
    YK_ASSERT(p_animationPath.Extension() == "gltf" || p_animationPath.Extension() == "glb",
              "YakuEn only supports GLTF animations!");

    YK_GLTF::File gltfAnimation(p_animationPath);
    if (gltfAnimation.CheckErrors() || !gltfAnimation.HasSkeleton() || !gltfAnimation.HasAnimation())
    {
        YK_ASSERT(gltfAnimation.HasSkeleton(), "YakuEn only supports animation via skeletons!");
        return CG_Animation();
    }

    tg3_model const& model = gltfAnimation.GetModel();
    tg3_animation const& animation =
      model.animations[CG_AnimationLoader_Private::FindAnimation(gltfAnimation, p_animationName)];
    tg3_skin const& skin = model.skins[0];

    std::unordered_map<YK_U32, YK_U8> nodeIDToBoneIndex;
    for (auto i : YK_CountTo(skin.joints_count))
    {
        nodeIDToBoneIndex[skin.joints[i]] = static_cast<YK_U8>(i);
    }

    CG_Animation result;
    for (auto i : YK_CountTo(animation.channels_count))
    {
        tg3_animation_channel const& channel = animation.channels[i];

        if (m_ignoreRootMotion && CG_AnimationLoader_Private::IsRoot(channel, gltfAnimation))
        {
            continue;
        }

        CG_AnimationLoader_Private::AnimationChannel channelType =
          CG_AnimationLoader_Private::GetChannelFromString(channel.target.path.data);

        if (channelType == CG_AnimationLoader_Private::POSITION)
        {
            result.m_positionChannels.Insert(
              CG_AnimationLoader_Private::ExtractChannel<YK_Vector3f>(gltfAnimation,
                                                                      animation,
                                                                      nodeIDToBoneIndex,
                                                                      i,
                                                                      result.m_duration));
        }
        else if (channelType == CG_AnimationLoader_Private::ORIENTATION)
        {
            result.m_orientationChannels.Insert(
              CG_AnimationLoader_Private::ExtractChannel<YK_Quaternion, YK_Vector4f>(gltfAnimation,
                                                                                     animation,
                                                                                     nodeIDToBoneIndex,
                                                                                     i,
                                                                                     result.m_duration));
        }
        else if (channelType == CG_AnimationLoader_Private::SCALE)
        {
            result.m_scaleChannels.Insert(CG_AnimationLoader_Private::ExtractChannel<YK_Vector3f>(gltfAnimation,
                                                                                                  animation,
                                                                                                  nodeIDToBoneIndex,
                                                                                                  i,
                                                                                                  result.m_duration));
        }
    }

#if !YAKU_RETAIL
    // GLTF breaks HARD when there's non-unform scale
    for (CG_Animation::ScaleChannel& scaleChannel : result.m_scaleChannels)
    {
        for (std::pair<float, YK_Vector3f>& keyframe : scaleChannel.m_keyframes)
        {
            YK_Vector3f& scale = keyframe.second;
            if (YK_FloatMath::ApproximatelyEqual(scale.x, scale.y)
                && YK_FloatMath::ApproximatelyEqual(scale.x, scale.z))
            {
                continue;
            }
            YK_LOG_ERROR(
              "GLTF Error: Non-uniform scale detected when loading animation {} in file {}! (Scale was {})",
              p_animationName,
              p_animationPath.CString(),
              scale);
        }
    }
#endif // !YAKU_RETAIL

    return result;
}