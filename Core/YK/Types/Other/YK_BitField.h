#pragma once

#include "YK/Types/Math/YK_Integer.h"
#include "YK/Debugging/YK_Assert.h"

template <typename /*TODO: Constrain to unsigned integers*/ StorageType>
struct YK_BitField
{
public:
    static constexpr YK_U8 MaximumValue = sizeof(StorageType) * 8;

public:
    constexpr YK_BitField() = default;
    constexpr YK_BitField(StorageType p_value)
        : m_data(p_value)
    {}
    constexpr YK_BitField(YK_BitField const& p_other)
        : m_data(p_other.m_data)
    {}

    constexpr void Enable(YK_U8 p_index)
    {
        YK_ASSERT(p_index < MaximumValue, "Attempting to set a BitField value greater than it can store!");
        m_data |= static_cast<StorageType>(1) << static_cast<StorageType>(p_index);
    }
    constexpr void Disable(YK_U8 p_index)
    {
        YK_ASSERT(p_index < MaximumValue, "Attempting to set a BitField value greater than it can store!");
        m_data &= ~(static_cast<StorageType>(1) << static_cast<StorageType>(p_index));
    }

    constexpr void Set(YK_U8 p_index, bool p_value)
    {
        YK_ASSERT(p_index < MaximumValue, "Attempting to set a BitField value greater than it can store!");
        m_data = (m_data & ~(static_cast<StorageType>(1) << static_cast<StorageType>(p_index)))
                 | (p_value << static_cast<StorageType>(p_index));
    }

    constexpr bool IsSet(YK_U8 p_index) const
    {
        YK_ASSERT(p_index < MaximumValue, "Attempting to set a BitField value greater than it can store!");
        return m_data & (static_cast<StorageType>(1) << static_cast<StorageType>(p_index));
    }

private:
    StorageType m_data = 0;
};