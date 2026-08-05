#pragma once

#include "YKC/Types/Traits/YKC_Concepts.h"

#include <limits>

template <YK_NumericType T>
struct YK_NumericLimits
{
    static constexpr T Max = std::numeric_limits<T>::max();
    static constexpr T Min = std::numeric_limits<T>::min();
};