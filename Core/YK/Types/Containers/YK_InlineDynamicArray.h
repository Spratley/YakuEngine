#pragma once

#include "YK/Debugging/YK_Assert.h"
#include "YK/Types/Containers/Iterator/YK_FlatIterator.h"
#include "YK/Types/Math/YK_Integer.h"
#include "YK/Utils/YK_MemoryUtils.h"

#include <type_traits>

template <typename DataType, YK_SizeT Capacity>
struct YK_InlineDynamicArray
{
public:
    constexpr YK_InlineDynamicArray() = default;
    constexpr ~YK_InlineDynamicArray();

    constexpr YK_InlineDynamicArray(YK_InlineDynamicArray const&) = delete;
    constexpr YK_InlineDynamicArray(YK_InlineDynamicArray&&) = delete;
    constexpr YK_InlineDynamicArray& operator=(YK_InlineDynamicArray const&) = delete;
    constexpr YK_InlineDynamicArray& operator=(YK_InlineDynamicArray&&) = delete;

    constexpr DataType& operator[](YK_SizeT p_index);
    constexpr DataType const& operator[](YK_SizeT p_index) const;

    constexpr DataType& Add(DataType const& p_item)
    requires(std::is_copy_constructible_v<DataType>);

    constexpr DataType& Add(DataType&& p_item)
    requires(std::is_move_constructible_v<DataType>);

    template <typename... Parameters>
    requires(std::is_constructible_v<DataType, Parameters...>)
    constexpr DataType& AddInPlace(Parameters&&... p_parameters);

    constexpr bool IsFull() const { return m_count >= Capacity; }
    constexpr bool IsEmpty() const { return m_count == 0; }

    constexpr YK_SizeT CountU() const { return m_count; }
    consteval YK_SizeT CapacityU() const { return Capacity; }

    constexpr YK_FlatIterator<DataType> begin() { return YK_FlatIterator(&m_buffer[0].m_item, 0); }
    constexpr YK_FlatIterator<DataType const> begin() const { return YK_FlatIterator(&m_buffer[0].m_item, 0); }
    constexpr YK_FlatIterator<DataType> end() { return YK_FlatIterator(&m_buffer[0].m_item, m_count); }
    constexpr YK_FlatIterator<DataType const> end() const { return YK_FlatIterator(&m_buffer[0].m_item, m_count); }

private:
    union DeferredConstructable
    {
        DeferredConstructable() {}
        ~DeferredConstructable() {}

        DataType m_item;
    };
    static_assert(sizeof(DeferredConstructable) == sizeof(DataType));
    DeferredConstructable m_buffer[Capacity];

    YK_SmallestStorage_T<Capacity> m_count = 0;
};

template <typename DataType, YK_SizeT Capacity>
constexpr YK_InlineDynamicArray<DataType, Capacity>::~YK_InlineDynamicArray()
{
    for (DataType& item : *this)
    {
        YK_PlacementNew::Destruct(item);
    }
    m_count = 0;
}

template <typename DataType, YK_SizeT Capacity>
constexpr DataType& YK_InlineDynamicArray<DataType, Capacity>::operator[](YK_SizeT p_index)
{
    YK_ASSERT(p_index < m_count, "Accessing YK_InlineDynamicArray out of bounds!");
    return m_buffer[p_index].m_item;
}

template <typename DataType, YK_SizeT Capacity>
constexpr DataType const& YK_InlineDynamicArray<DataType, Capacity>::operator[](YK_SizeT p_index) const
{
    YK_ASSERT(p_index < m_count, "Accessing YK_InlineDynamicArray out of bounds!");
    return m_buffer[p_index].m_item;
}

template <typename DataType, YK_SizeT Capacity>
constexpr DataType& YK_InlineDynamicArray<DataType, Capacity>::Add(DataType const& p_source)
requires(std::is_copy_constructible_v<DataType>)
{
    YK_ASSERT(!IsFull(), "Pushing an element into a full YK_InlineDynamicArray!");
    DataType* item = YK_PlacementNew::New<DataType>(&m_buffer[m_count++].m_item, p_source);
    return *item;
}

template <typename DataType, YK_SizeT Capacity>
constexpr DataType& YK_InlineDynamicArray<DataType, Capacity>::Add(DataType&& p_source)
requires(std::is_move_constructible_v<DataType>)
{
    YK_ASSERT(!IsFull(), "Pushing an element into a full YK_InlineDynamicArray!");
    DataType* item = YK_PlacementNew::New<DataType>(&m_buffer[m_count++].m_item, std::move(p_source));
    return *item;
}

template <typename DataType, YK_SizeT Capacity>
template <typename... Parameters>
requires(std::is_constructible_v<DataType, Parameters...>)
constexpr DataType& YK_InlineDynamicArray<DataType, Capacity>::AddInPlace(Parameters&&... p_parameters)
{
    YK_ASSERT(!IsFull(), "Pushing an element into a full YK_InlineDynamicArray!");
    DataType* item =
      YK_PlacementNew::New<DataType>(&m_buffer[m_count++].m_item, std::forward<Parameters>(p_parameters)...);
    return *item;
}