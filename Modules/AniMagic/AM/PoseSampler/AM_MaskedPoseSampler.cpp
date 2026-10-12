#include "PCH/AniMagic_PCH.h"
#include "AM_MaskedPoseSampler.h"

#include "YK/ECS/Components/YK_TransformComponent.h"
#include "YK/Types/Math/YK_Integer.h"

YK_Transform AM_MaskedPoseSampler::Sample(YK_U8 p_boneIndex, double p_sampleTime) const
{
    if (m_poseMask.IsSet(p_boneIndex))
    {
        return m_inMaskPose->Sample(p_boneIndex, p_sampleTime);
    }
    return m_outOfMaskPose->Sample(p_boneIndex, p_sampleTime);
}