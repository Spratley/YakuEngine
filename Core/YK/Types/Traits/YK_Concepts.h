#pragma once

#include "YK/Types/Math/YK_Integer.h"
#include "YK/Types/Traits/YK_TypeTraits.h"

#include <type_traits>

template <typename T>
concept YK_NumericType = std::is_arithmetic_v<T>;

template <typename T>
concept YK_FloatingPointType = std::is_floating_point_v<T>;

template <typename T>
concept YK_SizeTConvertible = std::is_integral_v<T> || std::is_enum_v<T>;

template <YK_SizeT Number>
concept YK_IsPowerOfTwo = (Number > 0) && ((Number & (Number - 1)) == 0);

template <typename T>
concept YK_NonCVRType = std::is_same_v<T, YK_StripCVR_T<T>>;
