#pragma once

#include "AM/PoseSampler/AM_BlendPoseSampler.h"

struct CG_Animation;

struct AM_AnimationComponent
{
    AM_BlendPoseSampler m_poseSampler;
	float m_sampleTime = 0.0f;
};