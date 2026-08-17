#pragma once

#include "YK/Types/Math/YK_Integer.h"

#include <type_traits>

template <typename T>
concept YK_NumericType = std::is_arithmetic_v<T>;

template <YK_SizeT Number>
concept YK_IsPowerOfTwo = (Number > 0) && ((Number & (Number - 1)) == 0);