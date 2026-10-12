#pragma once

#include "YK/Math/YK_FloatMath.h"
#include "YK/Math/YK_VectorMath.h"
#include "YK/Types/Math/YK_Integer.h"
#include "YK/Types/Math/YK_Vector.h"
#include "YK/Types/Traits/YK_Concepts.h"
#include "YK/Utils/YK_AlgorithmUtils.h"

template <YK_FloatingPointType Type>
struct YK_MathConstants
{
    static constexpr Type PI = static_cast<Type>(3.141592653589793);
    static constexpr Type PI_2 = static_cast<Type>(3.141592653589793) * static_cast<Type>(2.0);

    static constexpr Type RadToDeg = static_cast<Type>(180.0) / PI;
    static constexpr Type DegToRad = PI / static_cast<Type>(180.0);
};

template <YK_FloatingPointType Type>
constexpr Type YK_ToRadians(Type p_valueDegrees)
{
    return p_valueDegrees * YK_MathConstants<Type>::DegToRad;
}

template <YK_FloatingPointType Type>
constexpr Type YK_ToDegrees(Type p_valueRadians)
{
    return p_valueRadians * YK_MathConstants<Type>::RadToDeg;
}

// -=-=-= Trigonometry =-=-=-
template <YK_FloatingPointType Type>
constexpr Type YK_Cosine(Type p_value)
{
    if consteval
    {
        // Wrap p_value to range [-PI : PI] for precision
        p_value = YK_FloatingPointMath<Type>::Modulo(p_value, YK_MathConstants<Type>::PI_2);
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

        for (auto i : YK_CountTo(precision))
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

template <YK_FloatingPointType Type>
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

template <YK_FloatingPointType Type>
/*constexpr*/ Type YK_ArcTangent2D(YK_Vector_N<Type, 2> const& p_value)
{
    return std::atan2(p_value.y, p_value.x);
}

// -=-=-= Angle Between =-=-=-
template <YK_FloatingPointType Type>
/*constexpr*/ Type YK_SignedAngleBetween(YK_Vector_N<Type, 2> const& p_from, YK_Vector_N<Type, 2> const& p_to)
{
    return YK_ArcTangent2D(YK_Vector_N<Type, 2>{ YK_Vector::Dot(p_from, p_to), p_from.x * p_to.y - p_from.y * p_to.x });
}

template <YK_FloatingPointType Type>
/*constexpr*/ Type YK_SignedAngleBetween2D(YK_Vector_N<Type, 3> const& p_from, YK_Vector_N<Type, 3> const& p_to)
{
    return YK_SignedAngleBetween(YK_Vector_N<Type, 2>{ p_from }, YK_Vector_N<Type, 2>{ p_to });
}