#pragma once

#include "YK/Math/YK_MathUtils.h"
#include "YK/Math/YK_QuaternionMath.h"
#include "YK/Types/Containers/YK_FlatSet.h"
#include "YK/Types/Math/YK_Integer.h"
#include "YK/Types/Math/YK_Quaternion.h"
#include "YK/Types/Math/YK_Vector.h"
#include "YK/Utils/YK_AlgorithmUtils.h"

#include <utility>
#include <vector>

struct CG_Animation
{
    template <YK_Lerpable DataType>
    struct Channel
    {
        Channel() = default;
        Channel(Channel&& p_other) noexcept
            : m_boneIndex(YK_Exchange(p_other.m_boneIndex, 0))
            , m_keyframes(YK_Exchange(p_other.m_keyframes, std::vector<std::pair<float, DataType>>()))
        {}

        bool operator==(Channel const& p_rhs) const { return m_boneIndex == p_rhs.m_boneIndex; }

        DataType Sample(float p_time) const;

        YK_U8 m_boneIndex;
        std::vector<std::pair<float, DataType>> m_keyframes;
    };

    using PositionChannel = Channel<YK_Vector3f>;
    using OrientationChannel = Channel<YK_Quaternion>;
    using ScaleChannel = Channel<YK_Vector3f>;

    YK_FlatSet<PositionChannel> m_positionChannels;
    YK_FlatSet<OrientationChannel> m_orientationChannels;
    YK_FlatSet<ScaleChannel> m_scaleChannels;
};

template <YK_Lerpable DataType>
DataType CG_Animation::Channel<DataType>::Sample(float p_time) const
{
    // TODO: Replace with binary search to find keyframe range more efficently
    // I also feel like there's probably some kind of keyframe collapsing I could do so that we don't have to sample the
    // lerp factor for each keyframe if they're the same timings? Maybe that's overthinking it, gotta profile animation
    // first to see if it's expensive enough to consider
    for (YK_SizeT i = 1; i < m_keyframes.size(); ++i)
    {
        if (p_time > m_keyframes[i].first)
        {
            continue;
        }

        auto const& [startTime, startValue] = m_keyframes[i - 1];
        auto const& [endTime, endValue] = m_keyframes[i];
        float const lerpFactor = (p_time - startTime) / (endTime - startTime);
        return YK_Lerp(startValue, endValue, lerpFactor);
    }

    return m_keyframes[m_keyframes.size() - 1].second;
}