#pragma once

#include "YK/Debugging/YK_Assert.h"
#include "YK/Math/YK_NumericLimits.h"
#include "YK/Types/Math/YK_Integer.h"
#include "YK/Types/Traits/YK_Concepts.h"

#include <cmath>
#include <concepts>

// -=-=-= Squares =-=-=-
template <YK_FloatingPointType Type>
constexpr Type YK_SquareRoot(Type p_value)
{
    YK_ASSERT(p_value >= static_cast<Type>(0),
              "There's no such thing as the square root of a negative! (Don't say complex numbers)");
    if consteval
    {
        if (p_value == static_cast<Type>(0) || p_value == static_cast<Type>(1))
        {
            return p_value;
        }

        Type current = p_value;
        Type previous = 0;

        // Newton-Raphson convergance method
        while (current != previous)
        {
            previous = current;
            current = static_cast<Type>(0.5) * (current + p_value / current);
        }
        return current;
    }
    else
    {
        return std::sqrt(p_value);
    }
}

template <typename Type>
constexpr Type YK_Square(Type const& p_value)
{
    return p_value * p_value;
}

// -=-=-= Logical Operations =-=-=-
template <typename Type>
constexpr Type const& YK_Min(Type const& p_lhs, Type const& p_rhs)
{
    return (p_lhs < p_rhs) ? p_lhs : p_rhs;
}

template <typename Type>
constexpr Type const& YK_Max(Type const& p_lhs, Type const& p_rhs)
{
    return (p_lhs > p_rhs) ? p_lhs : p_rhs;
}

template <typename Type>
constexpr Type YK_Clamp(Type const& p_value, Type const& p_min, Type const& p_max)
{
    return YK_Min(YK_Max(p_value, p_min), p_max);
}

template <typename Type>
constexpr Type YK_Abs(Type const& p_value)
{
    if consteval
    {
        if (p_value < static_cast<Type>(0))
        {
            return -p_value;
        }
        return p_value;
    }
    else
    {
        return std::abs(p_value);
    }
}

template <typename Type>
constexpr Type YK_Sign(Type const& p_value)
{
    return (p_value < static_cast<Type>(0)) ? static_cast<Type>(-1) : static_cast<Type>(1);
}

// -=-=-= Lerp =-=-=-
template <typename Type>
concept YK_NativelyLerpable = requires(Type t, float f) {
    { t + ((t - t) * f) } -> std::same_as<Type>;
};

template <typename Type, YK_FloatingPointType FactorType>
requires(YK_NativelyLerpable<Type>)
constexpr Type YK_Lerp(Type const& p_a, Type const& p_b, FactorType p_t)
{
    return p_a + static_cast<Type>((p_b - p_a) * p_t);
}

template <typename Type>
concept YK_Lerpable = requires(Type t, float f) {
    { YK_Lerp(t, t, f) } -> std::same_as<Type>;
};

template <YK_Lerpable Type, YK_FloatingPointType FactorType>
constexpr Type YK_LerpClamped(Type const& p_a, Type const& p_b, FactorType p_t)
{
    if (p_t <= 0.0f)
    {
        return p_a;
    }
    if (p_t >= 1.0f)
    {
        return p_b;
    }
    return YK_Lerp(p_a, p_b, p_t);
}