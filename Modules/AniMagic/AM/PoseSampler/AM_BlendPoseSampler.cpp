#include "PCH/AniMagic_PCH.h"
#include "AM_BlendPoseSampler.h"

#include "YK/ECS/Components/YK_TransformComponent.h"
#include "YK/Types/Math/YK_Integer.h"
#include "YK/Utils/YK_AlgorithmUtils.h"

#include "AM/Types/AM_Keyframe.h"
#include "AM_PoseSampler.h"

#include <algorithm>

void AM_BlendPoseSampler::AddSampler(AM_PoseSampler const* p_sampler, double p_blendValue)
{
    // TODO: Make some sort of sorted array class, or sorted insert helpers for YK_DynamicArray
    auto insertPosition = std::lower_bound(m_poseSamplers.begin(), m_poseSamplers.end(), p_blendValue);
    m_poseSamplers.insert(insertPosition, AM_Keyframe{ .m_time = p_blendValue, .m_value = p_sampler });
}

YK_Transform AM_BlendPoseSampler::Sample(YK_U8 p_boneIndex, double p_sampleTime) const
{
    YK_SizeT const poseSamplerCount = m_poseSamplers.size();
    if (poseSamplerCount == 0)
    {
        return YK_Transform();
    }

    if (poseSamplerCount == 1)
    {
        return m_poseSamplers[0].m_value->Sample(p_boneIndex, p_sampleTime);
    }

    for (auto i : YK_CountTo(1, poseSamplerCount))
    {
        if (m_blendValue > m_poseSamplers[i].m_time)
        {
            continue;
        }

        auto const& [startBlendValue, startSampler] = m_poseSamplers[i - 1];
        auto const& [endBlendValue, endSampler] = m_poseSamplers[i];
        double const lerpFactor = (m_blendValue - startBlendValue) / (endBlendValue - startBlendValue);
        return YK_Lerp(startSampler->Sample(p_boneIndex, p_sampleTime),
                       endSampler->Sample(p_boneIndex, p_sampleTime),
                       lerpFactor);
    }

    return m_poseSamplers.back().m_value->Sample(p_boneIndex, p_sampleTime);
}
