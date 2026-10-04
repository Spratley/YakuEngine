#pragma once

#include "YK/ECS/Components/YK_TransformComponent.h"
#include "YK/Types/Math/YK_Integer.h"

class AM_PoseSampler
{
public:
    constexpr AM_PoseSampler() = default;
    virtual ~AM_PoseSampler() = default;

    virtual YK_Transform Sample(YK_U8 p_boneIndex, double p_sampleTime) const = 0;
};