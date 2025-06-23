#pragma once
template <bool Condition, typename _Type = void>
struct YK_EnableIf {};

template <typename _Type>
struct YK_EnableIf<true, _Type>
{
	using Type = _Type;
};