#pragma once

#include "YK/ECS/Components/YK_TransformComponent.h"
#include "YK/Types/Math/YK_Integer.h"

#include "AM/PoseSampler/AM_PoseSampler.h"

struct CG_Animation;

class AM_AnimationPoseSampler : public AM_PoseSampler
{
public:
    AM_AnimationPoseSampler() = default;
    AM_AnimationPoseSampler(CG_Animation const* p_animation)
        : m_animation(p_animation)
    {}

    YK_Transform Sample(YK_U8 p_boneIndex, double p_sampleTime) const;

private:
    CG_Animation const* m_animation = nullptr;
};