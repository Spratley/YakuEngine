#pragma once

#include "AM/PoseSampler/AM_PoseSampler.h"

#include "YK/ECS/Components/YK_TransformComponent.h"
#include "YK/Types/Math/YK_Integer.h"

class AM_LerpPoseSampler : public AM_PoseSampler
{
public:
    constexpr AM_LerpPoseSampler() = default;
    constexpr AM_LerpPoseSampler(AM_PoseSampler const* p_a, AM_PoseSampler const* p_b, float p_sampleFactor = 0.0f)
        : m_a(p_a)
        , m_b(p_b)
        , m_sampleFactor(p_sampleFactor)
    {}

    YK_Transform Sample(YK_U8 p_boneIndex, double p_sampleTime) const override
    {
        return YK_LerpClamped(m_a->Sample(p_boneIndex, p_sampleTime), m_b->Sample(p_boneIndex, p_sampleTime), m_sampleFactor);
    }

    void SetSampleFactor(float p_sampleFactor) { m_sampleFactor = p_sampleFactor; }

private:
    AM_PoseSampler const* m_a = nullptr;
    AM_PoseSampler const* m_b = nullptr;

    float m_sampleFactor = 0.0f;
};