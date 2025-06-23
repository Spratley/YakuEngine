#pragma once

template <class Type>
inline constexpr Type const& YK_Min(Type const& p_lhs, Type const& p_rhs)
{
	return (p_lhs < p_rhs) ? p_lhs : p_rhs;
}

template <class Type>
inline constexpr Type const& YK_Max(Type const& p_lhs, Type const& p_rhs)
{
	return (p_lhs > p_rhs) ? p_lhs : p_rhs;
}