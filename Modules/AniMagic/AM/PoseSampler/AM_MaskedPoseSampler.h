#pragma once

#include "YK/ECS/Components/YK_TransformComponent.h"
#include "YK/Types/Math/YK_Integer.h"

#include "AM/PoseSampler/AM_PoseSampler.h"
#include "AM/Types/AM_PoseMask.h"

class AM_MaskedPoseSampler : public AM_PoseSampler
{
public:
    constexpr AM_MaskedPoseSampler() = default;
    AM_MaskedPoseSampler(AM_PoseMask const& p_poseMask,
                         AM_PoseSampler /*const*/* p_inMaskPose,
                         AM_PoseSampler /*const*/* p_outOfMaskPose)
        : m_inMaskPose(p_inMaskPose)
        , m_outOfMaskPose(p_outOfMaskPose)
        , m_poseMask(p_poseMask)
    {}

    YK_Transform Sample(YK_U8 p_boneIndex, double p_sampleTime) const override;

    // Temporary while I'm manually setting blending values from top-down
    AM_PoseSampler& GetInMaskSampler() { return *m_inMaskPose; }
    AM_PoseSampler& GetOutOfMaskSampler() { return *m_outOfMaskPose; }

private:
    AM_PoseSampler /*const*/* m_inMaskPose = nullptr;
    AM_PoseSampler /*const*/* m_outOfMaskPose = nullptr;
    AM_PoseMask m_poseMask = {};
};