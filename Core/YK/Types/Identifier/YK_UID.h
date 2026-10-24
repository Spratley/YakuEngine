#pragma once

#include "YK/Math/YK_Hash.h"
#include "YK/Types/Math/YK_Integer.h"

#include <type_traits>

struct YK_UID
{
    friend struct std::hash<YK_UID>;

public:
    constexpr YK_UID() = default;
    constexpr YK_UID(YK_UID const&) = default;

    constexpr YK_UID(YK_U64 p_value)
        : m_uid(p_value)
    {}

    constexpr YK_UID(char const* p_string)
        : m_uid(YK_Hash::FNV_1A_64(p_string))
    {}

    constexpr YK_UID& operator=(YK_UID const&) = default;
    constexpr YK_UID& operator=(YK_U64 const& p_value)
    {
        m_uid = p_value;
        return *this;
    }

    constexpr bool operator==(YK_UID const& p_other) const = default;
    constexpr bool operator!=(YK_UID const& p_other) const = default;

private:
    YK_U64 m_uid = 0;
};

template <>
struct std::hash<YK_UID>
{
    std::size_t operator()(YK_UID const& p_item) const noexcept { return std::hash<YK_U64>{}(p_item.m_uid); }
};