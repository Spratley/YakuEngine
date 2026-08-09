#pragma once

#include <concepts>

template <typename ChildType, typename BaseType>
requires(std::derived_from<ChildType, BaseType>)
constexpr ChildType* YK_CRTPCast(BaseType* const& p_base)
{
    return static_cast<ChildType*>(p_base);
}