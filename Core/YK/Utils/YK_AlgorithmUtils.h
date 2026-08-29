#pragma once

#include "YK/Types/Math/YK_Integer.h"

#include "YK/Types/Traits/YK_TypeTraits.h"

#include <type_traits>
#include <utility>

// Overload for scalar types so that any constant that can be reasonably assigned to a type will work
template <typename ScalarType>
requires(std::is_scalar_v<ScalarType> && std::is_assignable_v<ScalarType&, ScalarType>)
constexpr ScalarType YK_Exchange(ScalarType& p_source, YK_TypeType_T<ScalarType> p_newValue)
{
    ScalarType oldValue = p_source;
    p_source = p_newValue;
    return oldValue;
}

template <typename Type, typename NewValueType = Type>
requires(std::is_move_constructible_v<Type> && std::is_assignable_v<Type&, NewValueType> && !std::is_scalar_v<Type>)
constexpr Type YK_Exchange(Type& p_source, NewValueType&& p_newValue)
{
    Type oldValue = std::move(p_source);
    p_source = std::forward<NewValueType>(p_newValue);
    return oldValue;
}

template <typename Type>
requires(std::is_move_assignable_v<Type> && std::is_move_constructible_v<Type>)
constexpr void YK_Swap(Type& p_a, Type& p_b)
{
    p_a = YK_Exchange(p_b, std::move(p_a));
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