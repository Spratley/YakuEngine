#pragma once

#include "AM/PoseSampler/AM_MaskedPoseSampler.h"

struct CG_Animation;

struct AM_AnimationComponent
{
    AM_MaskedPoseSampler m_poseSampler;
    float m_sampleTime = 0.0f;
};