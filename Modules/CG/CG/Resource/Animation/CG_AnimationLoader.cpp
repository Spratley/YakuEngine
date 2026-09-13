#include "PCH/CG_PCH.h"
#include "CG_AnimationLoader.h"

#include "YK/Debugging/YK_Assert.h"
#include "YK/IO/File/YK_FilePath.h"
#include "YK/IO/Logging/YK_Logger.h"
#include "YK/Types/Math/YK_Integer.h"
#include "YK/Types/Math/YK_Quaternion.h"
#include "YK/Types/Math/YK_Vector.h"
#include "YK/Utils/YK_AlgorithmUtils.h"

#include "CG/IO/CG_GLTF.h"
#include "CG/Libraries/TinyGLTF/tiny_gltf_v3.h"
#include "CG/Resource/Animation/CG_Animation.h"

#include <cstring>
#include <unordered_map>
#include <utility>

// TODO: Skeletons are usually saved as part of the same mesh file
// There should be a compound-load function to get both, so we don't have to open the fiie twice when loading one asset

namespace CG_AnimationLoader_Private
{
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
        YK_LOG_ERROR_PARAM("Unknown animation channel requested! {}", p_channelName);
        return static_cast<AnimationChannel>(-1);
    }

    template <typename ChannelDataType, typename ExtractionType = ChannelDataType>
    CG_Animation::Channel<ChannelDataType> ExtractChannel(CG_GLTF::File const& p_gltfData,
                                                          tg3_animation const& p_gltfAnimation,
                                                          std::unordered_map<YK_U32, YK_U8> p_nodeIDToBoneIndex,
                                                          YK_SizeT p_channelIndex)
    {
        tg3_animation_channel const& channel = p_gltfAnimation.channels[p_channelIndex];
        tg3_animation_sampler const& sampler = p_gltfAnimation.samplers[channel.sampler];

        YK_U8 boneIndex = p_nodeIDToBoneIndex[channel.target.node];
        CG_GLTF::DataView<float> const keys = p_gltfData.ViewData<float>(sampler.input);

        CG_Animation::Channel<ChannelDataType> resultChannel;
        resultChannel.m_keyframes.resize(keys.m_count);
        resultChannel.m_boneIndex = boneIndex;

        CG_GLTF::DataView<ExtractionType> const values = p_gltfData.ViewData<ExtractionType>(sampler.output);
        for (auto keyIndex : YK_CountTo(keys.m_count))
        {
            resultChannel.m_keyframes[keyIndex] =
              std::pair<float, ChannelDataType>{ keys.m_buffer[keyIndex], values.m_buffer[keyIndex] };
        }
        return resultChannel;
    }
} // namespace CG_AnimationLoader_Private

CG_Animation CG_AnimationLoader::Load(YK_FilePath const& p_animationPath)
{
    YK_ASSERT(p_animationPath.Extension() == "gltf" || p_animationPath.Extension() == "glb",
              "YakuEn only supports GLTF animations!");

    CG_GLTF::File gltfAnimation(p_animationPath);
    if (gltfAnimation.CheckErrors() || !gltfAnimation.HasSkeleton() || !gltfAnimation.HasAnimation())
    {
        YK_ASSERT(gltfAnimation.HasSkeleton(), "YakuEn only supports animation via skeletons!");
        return CG_Animation();
    }

    // Right now this only loads the first animation
    tg3_model const& model = gltfAnimation.GetModel();
    tg3_animation const& animation = model.animations[0];
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
        CG_AnimationLoader_Private::AnimationChannel channelType =
          CG_AnimationLoader_Private::GetChannelFromString(channel.target.path.data);

        if (channelType == CG_AnimationLoader_Private::POSITION)
        {
            result.m_positionChannels.Insert(
              CG_AnimationLoader_Private::ExtractChannel<YK_Vector3f>(gltfAnimation, animation, nodeIDToBoneIndex, i));
        }
        else if (channelType == CG_AnimationLoader_Private::ORIENTATION)
        {
            result.m_orientationChannels.Insert(
              CG_AnimationLoader_Private::ExtractChannel<YK_Quaternion, YK_Vector4f>(gltfAnimation,
                                                                                     animation,
                                                                                     nodeIDToBoneIndex,
                                                                                     i));
        }
        else if (channelType == CG_AnimationLoader_Private::SCALE)
        {
            result.m_scaleChannels.Insert(
              CG_AnimationLoader_Private::ExtractChannel<YK_Vector3f>(gltfAnimation, animation, nodeIDToBoneIndex, i));
        }
    }
    return result;
}