#pragma once

#include "YK/Debugging/YK_Assert.h"

#include <cmath>
#include <concepts>
#include <type_traits>
#include "YK/Types/Math/YK_Integer.h"

// -=-=-= Squares =-=-=-
template <typename Type>
requires(std::is_floating_point_v<Type>)
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
requires(std::is_floating_point_v<Type>)
constexpr Type YK_Truncate(Type p_value)
{
#if _MSC_VER
    if consteval
    {
        return static_cast<Type>(static_cast<YK_Int64>(p_value));
    }
    else
#endif // _MSC_VER
    {
        return std::trunc(p_value);
    }
}

template <typename Type>
requires(std::is_floating_point_v<Type>)
constexpr Type YK_Floor(Type p_value)
{
#if _MSC_VER
    if consteval
    {
        YK_Int64 const wholePart = static_cast<YK_Int64>(p_value);
        return (p_value < wholePart) ? static_cast<Type>(wholePart - 1) : static_cast<Type>(wholePart);
    }
    else
#endif // _MSC_VER
    {
        // Praise to Herb Sutter (and also whoever submtted the proposal) that std::floor is constexpr in C++ 23
        // Now if only MSVC actually supported it...
        return std::floor(p_value);
    }
}

template <typename Type>
requires(std::is_floating_point_v<Type>)
constexpr Type YK_Ceiling(Type p_value)
{
#if _MSC_VER
    if consteval
    {
        YK_Int64 const wholePart = static_cast<YK_Int64>(p_value);
        return (wholePart < p_value) ? static_cast<Type>(wholePart + 1) : static_cast<Type>(wholePart);
    }
    else
#endif // _MSC_VER
    {
        return std::ceil(p_value);
    }
}

template <typename Type>
requires(std::is_floating_point_v<Type>)
constexpr Type YK_FloatModulo(Type p_numerator, Type p_denominator)
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

// -=-=-= Lerp =-=-=-
template <typename Type>
concept YK_NativelyLerpable = requires(Type t, float f) {
    { t + ((t - t) * f) } -> std::same_as<Type>;
};

template <typename Type>
requires(YK_NativelyLerpable<Type>)
constexpr Type YK_Lerp(Type const& p_a, Type const& p_b, float p_t)
{
    return p_a + ((p_b - p_a) * p_t);
}

template <typename Type>
concept YK_Lerpable = requires(Type t, float f) {
    { YK_Lerp(t, t, f) } -> std::same_as<Type>;
};