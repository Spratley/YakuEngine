#pragma once

#include "AM/PoseSampler/AM_LerpPoseSampler.h"

struct CG_Animation;

struct AM_AnimationComponent
{
    AM_LerpPoseSampler m_poseSampler;
	float m_sampleTime = 0.0f;
};