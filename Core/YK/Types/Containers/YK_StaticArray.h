#pragma once

#include "YK/Debugging/YK_Assert.h"
#include "YK/Types/Containers/Iterator/YK_FlatIterator.h"
#include "YK/Types/Math/YK_Integer.h"

template <typename DataType, YK_SizeT Count>
struct YK_StaticArray
{
public:
    YK_StaticArray() = default;
    ~YK_StaticArray() = default;

    YK_StaticArray(YK_StaticArray const&) = delete;
    YK_StaticArray(YK_StaticArray&&) = delete;

    YK_StaticArray& operator=(YK_StaticArray const&) = delete;
    YK_StaticArray& operator=(YK_StaticArray&&) = delete;

    DataType& operator[](YK_SizeT p_index);
    DataType const& operator[](YK_SizeT p_index) const;

    consteval YK_SizeT CountU() const { return Count; }

    YK_FlatIterator<DataType> begin() { return YK_FlatIterator<DataType>(m_buffer, 0); }
    YK_FlatIterator<DataType> end() { return YK_FlatIterator<DataType>(m_buffer, Count); }

private:
    DataType m_buffer[Count];
};

template <typename DataType, YK_SizeT Count>
DataType& YK_StaticArray<DataType, Count>::operator[](YK_SizeT p_index)
{
    YK_ASSERT(p_index < Count, "Accessing YK_StaticArray out of bounds!");
    return m_buffer[p_index];
}

template <typename DataType, YK_SizeT Count>
DataType const& YK_StaticArray<DataType, Count>::operator[](YK_SizeT p_index) const
{
    YK_ASSERT(p_index < Count, "Accessing YK_StaticArray out of bounds!");
    return m_buffer[p_index];
}