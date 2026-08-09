#pragma once

#include "YKC/Types/Math/YKC_Integer.h"

template <YK_SizeT Number>
struct IsPowerOfTwo
{
    static constexpr bool value = (Number > 0) && ((Number & (Number - 1)) == 0);
};

template <YK_SizeT Number>
constexpr bool IsPowerOfTwo_v = IsPowerOfTwo<Number>::value;