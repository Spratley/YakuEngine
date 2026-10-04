#include "PCH/AniMagic_PCH.h"
#include "AM_AnimationPoseSampler.h"

#include "YK/ECS/Components/YK_TransformComponent.h"
#include "YK/Math/YK_MathUtils.h"
#include "YK/Types/Math/YK_Integer.h"

#include "CG/Resource/Animation/CG_Animation.h"

YK_Transform AM_AnimationPoseSampler::Sample(YK_U8 p_boneIndex, double p_sampleTime) const
{
    YK_Transform result;
    if (!m_animation)
    {
        return result;
    }

    p_sampleTime = YK_FloatModulo(p_sampleTime, m_animation->m_duration);

    // TODO: Re-organize channel data so bone lookups are fast
    // Looping here is just awful
    for (CG_Animation::PositionChannel const& positionChannel : m_animation->m_positionChannels)
    {
        if (positionChannel.m_boneIndex != p_boneIndex)
        {
            continue;
        }
        result.m_position = positionChannel.Sample(p_sampleTime);
    }

    for (CG_Animation::OrientationChannel const& orientationChannel : m_animation->m_orientationChannels)
    {
        if (orientationChannel.m_boneIndex != p_boneIndex)
        {
            continue;
        }
        result.m_orientation = orientationChannel.Sample(p_sampleTime);
    }

    for (CG_Animation::ScaleChannel const& scaleChannel : m_animation->m_scaleChannels)
    {
        if (scaleChannel.m_boneIndex != p_boneIndex)
        {
            continue;
        }
        result.m_scale = scaleChannel.Sample(p_sampleTime);
    }

    return result;
}