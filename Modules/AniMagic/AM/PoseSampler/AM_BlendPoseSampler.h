#pragma once

#include "AM/PoseSampler/AM_PoseSampler.h"

#include <vector>

class AM_BlendPoseSampler : public AM_PoseSampler
{
public:

public:
    std::vector<AM_PoseSampler> m_poseSamplers;
};