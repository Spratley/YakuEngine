#pragma once

#include "YK/ECS/Components/YK_TransformComponent.h"
#include "YK/Types/Math/YK_Integer.h"

#include "CG/Resource/CG_ResourceHandles.h"

#include "AM/PoseSampler/AM_PoseSampler.h"

class AM_AnimationPoseSampler : public AM_PoseSampler
{
public:
    constexpr AM_AnimationPoseSampler() = default;
    AM_AnimationPoseSampler(CG_AnimationHandle p_animation)
        : m_animation(p_animation)
    {}

    YK_Transform Sample(YK_U8 p_boneIndex, double p_sampleTime) const override;

private:
    CG_AnimationHandle m_animation = CG_AnimationHandle::EmptyHandle;
};