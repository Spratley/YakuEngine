#pragma once

#include "YKC/Types/Math/YKC_Integer.h"

#include <type_traits>

template <typename Type>
struct YK_FlatIterator
{
public:
    YK_FlatIterator(Type* p_buffer, YK_SizeT p_index)
        : m_buffer(p_buffer)
        , m_index(p_index)
    {}

    YK_FlatIterator& operator++();
    Type& operator*();

    template <typename OtherType>
    requires(std::is_same_v<std::remove_cv_t<Type>, std::remove_cv_t<OtherType>>)
    friend bool operator!=(YK_FlatIterator const& p_lhs, YK_FlatIterator<OtherType> const& p_rhs)
    {
        return p_lhs.m_index != p_rhs.m_index;
    }

private:
    Type* m_buffer = nullptr;
    YK_SizeT m_index = 0;
};

template <typename Type>
YK_FlatIterator<Type>& YK_FlatIterator<Type>::operator++()
{
    m_index++;
    return *this;
}

template <typename Type>
Type& YK_FlatIterator<Type>::operator*()
{
    return m_buffer[m_index];
}