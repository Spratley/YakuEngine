#pragma once

#include "YK/Types/Math/YK_Integer.h"

#include <type_traits>

template <typename Type>
requires(std::is_floating_point_v<Type>)
struct YK_MathConstants
{
    static constexpr Type PI = static_cast<Type>(3.141592653589793);
    static constexpr Type PI_2 = static_cast<Type>(3.141592653589793) * static_cast<Type>(2.0);

    static constexpr Type RadToDeg = static_cast<Type>(180.0) / PI;
    static constexpr Type DegToRad = PI / static_cast<Type>(180.0);
};

template <typename Type>
requires(std::is_floating_point_v<Type>)
constexpr Type YK_ToRadians(Type p_valueDegrees)
{
    return p_valueDegrees * YK_MathConstants<Type>::DegToRad;
}

template <typename Type>
requires(std::is_floating_point_v<Type>)
constexpr Type YK_ToDegrees(Type p_valueRadians)
{
    return p_valueRadians * YK_MathConstants<Type>::RadToDeg;
}

// -=-=-= Trigonometry =-=-=-
template <typename Type>
requires(std::is_floating_point_v<Type>)
constexpr Type YK_Cosine(Type p_value)
{
    if consteval
    {
        // Wrap p_value to range [-PI : PI] for precision
        p_value = YK_FloatModulo(p_value, YK_MathConstants<Type>::PI_2);
        if (p_value < static_cast<Type>(0.0))
        {
            p_value += YK_MathConstants<Type>::PI_2;
        }
        if (p_value > YK_MathConstants<Type>::PI)
        {
            p_value -= YK_MathConstants<Type>::PI_2;
        }

        // Taylor Series Approximation
        YK_U32 const precision = 15;

        Type sum = static_cast<Type>(1.0);
        Type term = static_cast<Type>(1.0);

        Type const numerator = -p_value * p_value;

        for (YK_U32 i = 1; i < precision; ++i)
        {
            Type const i2 = static_cast<Type>(2.0) * static_cast<Type>(i);
            Type const denominator = i2 * (i2 - 1);
            term = term * ((numerator) / (denominator));
            sum += term;
        }
        return sum;
    }
    else
    {
        return std::cos(p_value);
    }
}

template <typename Type>
requires(std::is_floating_point_v<Type>)
constexpr Type YK_Sine(Type p_value)
{
    if consteval
    {
        return YK_Cosine(p_value - YK_ToRadians(static_cast<Type>(90.0)));
    }
    else
    {
        return std::sin(p_value);
    }
}