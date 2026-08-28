#pragma once

#include <type_traits>

template <typename Type>
	requires(std::is_floating_point_v<Type>)
struct YK_MathConstants
{
    static constexpr Type PI = static_cast<Type>(3.141592653589793);

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