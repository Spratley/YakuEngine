#pragma once

#include "YK/Math/YK_MathUtils.h"
#include "YK/Math/YK_QuaternionMath.h"
#include "YK/Types/Containers/YK_FlatSet.h"
#include "YK/Types/Math/YK_Integer.h"
#include "YK/Types/Math/YK_Quaternion.h"
#include "YK/Types/Math/YK_Vector.h"

#include <utility>
#include <vector>

struct CG_Animation
{
    template <YK_Lerpable DataType>
    struct Channel
    {
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