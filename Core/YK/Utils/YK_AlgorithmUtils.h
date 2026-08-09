#pragma once

#include "YK/Types/Math/YK_Integer.h"

template <typename T>
inline constexpr void YK_Swap(T& p_a, T& p_b)
{
    T temp = std::move(p_a);
    p_a = std::move(p_b);
    p_b = std::move(temp);
}

struct YK_CountTo
{
    constexpr YK_CountTo(YK_SizeT p_limit)
        : m_limit(p_limit)
    {}

    struct Iterator
    {
        constexpr Iterator(YK_SizeT p_i)
            : m_i(p_i)
        {}

        constexpr Iterator& operator++()
        {
            ++m_i;
            return *this;
        }

        constexpr YK_SizeT operator*() { return m_i; }
        constexpr friend bool operator!=(Iterator const& p_lhs, Iterator const& p_rhs)
        {
            return p_lhs.m_i != p_rhs.m_i;
        }

        YK_SizeT m_i;
    };

    constexpr Iterator begin() const { return Iterator(0); }
    constexpr Iterator end() const { return Iterator(m_limit); }

    YK_SizeT const m_limit;
};