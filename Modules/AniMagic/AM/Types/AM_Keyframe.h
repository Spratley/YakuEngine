#pragma once

#include "YK/Types/Traits/YK_Concepts.h"

template <typename DataType, YK_FloatingPointType Precision = double>
struct AM_Keyframe
{
    auto operator<=>(AM_Keyframe const& p_other) const { return m_time <=> p_other.m_time; }
    auto operator<=>(Precision const& p_time) const { return m_time <=> p_time; }

    Precision m_time;
    DataType m_value;
};