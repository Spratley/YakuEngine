#pragma once

template <typename T>
inline constexpr void YK_Swap(T& p_a, T& p_b)
{
    T temp = std::move(p_a);
    p_a = std::move(p_b);
    p_b = std::move(temp);
}