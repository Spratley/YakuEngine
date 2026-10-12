#pragma once

#include "YK/Math/YK_MathUtils.h"
#include "YK/Types/Math/YK_Integer.h"
#include "YK/Types/Math/YK_SameWidthInteger.h"
#include "YK/Types/Traits/YK_Concepts.h"

#include <bit>
#include <cmath>
#include <limits>

template <YK_FloatingPointType FloatType>
struct YK_FloatingPointMath
{
    using BitStorageType = YK_SameWidthInt_T<FloatType>;

    struct Constants
    {
        static constexpr FloatType Epsilon = std::numeric_limits<FloatType>::epsilon();
    };

    static constexpr bool ApproximatelyEqual(FloatType p_lhs, FloatType p_rhs, YK_U8 p_precision = 32u)
    {
        if (p_lhs == p_rhs)
        {
            return true;
        }

        BitStorageType lhsBits = std::bit_cast<BitStorageType>(p_lhs);
        BitStorageType rhsBits = std::bit_cast<BitStorageType>(p_rhs);

        if (YK_Sign(lhsBits) != YK_Sign(rhsBits))
        {
            return false;
        }

        return YK_Abs(lhsBits - rhsBits) < p_precision;
    }

    static constexpr FloatType YK_Truncate(FloatType p_value)
    {
#if _MSC_VER
        if consteval
        {
            return static_cast<FloatType>(static_cast<YK_Int64>(p_value));
        }
        else
#endif // _MSC_VER
        {
            return std::trunc(p_value);
        }
    }

    static constexpr FloatType Floor(FloatType p_value)
    {
#if _MSC_VER
        if consteval
        {
            YK_Int64 const wholePart = static_cast<YK_Int64>(p_value);
            return (p_value < wholePart) ? static_cast<FloatType>(wholePart - 1) : static_cast<FloatType>(wholePart);
        }
        else
#endif // _MSC_VER
        {
            // Praise to Herb Sutter (and also whoever submtted the proposal) that std::floor is constexpr in C++ 23
            // Now if only MSVC actually supported it...
            return std::floor(p_value);
        }
    }

    static constexpr FloatType Ceiling(FloatType p_value)
    {
#if _MSC_VER
        if consteval
        {
            YK_Int64 const wholePart = static_cast<YK_Int64>(p_value);
            return (wholePart < p_value) ? static_cast<FloatType>(wholePart + 1) : static_cast<FloatType>(wholePart);
        }
        else
#endif // _MSC_VER
        {
            return std::ceil(p_value);
        }
    }

    static constexpr FloatType Modulo(FloatType p_numerator, FloatType p_denominator)
    {
        if consteval
        {
            return p_numerator - (YK_Truncate(p_numerator / p_denominator) * p_denominator);
        }
        else
        {
            return std::fmod(p_numerator, p_denominator);
        }
    }
};

using YK_FloatMath = YK_FloatingPointMath<float>;
using YK_DoubleMath = YK_FloatingPointMath<double>;