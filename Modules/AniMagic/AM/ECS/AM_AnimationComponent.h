#pragma once

struct CG_Animation;

struct AM_AnimationComponent
{
	CG_Animation const* m_animation = nullptr;
	float m_sampleTime = 0.0f;
};