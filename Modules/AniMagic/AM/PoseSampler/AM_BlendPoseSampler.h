#pragma once

#include "YK/ECS/Components/YK_TransformComponent.h"
#include "YK/Types/Math/YK_Integer.h"

#include "AM/PoseSampler/AM_PoseSampler.h"
#include "AM/Types/AM_Keyframe.h"

#include <vector>

class AM_BlendPoseSampler : public AM_PoseSampler
{
public:
    constexpr AM_BlendPoseSampler() = default;

    void AddSampler(AM_PoseSampler const* p_sampler, double p_blendValue);

    YK_Transform Sample(YK_U8 p_boneIndex, double p_sampleTime) const override;

    void SetBlendValue(double p_blendValue) { m_blendValue = p_blendValue; }

public:
    std::vector<AM_Keyframe<AM_PoseSampler const*>> m_poseSamplers;
    double m_blendValue = 0.0;
};