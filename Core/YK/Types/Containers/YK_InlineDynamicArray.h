#pragma once

#include "YK/Debugging/YK_Assert.h"
#include "YK/Types/Containers/Iterator/YK_FlatIterator.h"
#include "YK/Types/Math/YK_Integer.h"
#include "YK/Utils/YK_MemoryUtils.h"

// TODO: Make this constexpr compatible
// Right now I'm relying on pointer manipulation which isn't allowed in constexpr contexts

template <typename DataType, YK_SizeT Capacity>
struct YK_InlineDynamicArray
{
public:
    YK_InlineDynamicArray() = default;
    ~YK_InlineDynamicArray();

    YK_InlineDynamicArray(YK_InlineDynamicArray const&) = delete;
    YK_InlineDynamicArray(YK_InlineDynamicArray&&) = delete;
    YK_InlineDynamicArray& operator=(YK_InlineDynamicArray const&) = delete;
    YK_InlineDynamicArray& operator=(YK_InlineDynamicArray&&) = delete;

    DataType& operator[](YK_SizeT p_index);
    DataType const& operator[](YK_SizeT p_index) const;

    DataType& Add(DataType const& p_item)
    requires(std::is_copy_constructible_v<DataType>);

    DataType& Add(DataType&& p_item)
    requires(std::is_move_constructible_v<DataType>);

    template <typename... Parameters>
    requires(std::is_constructible_v<DataType, Parameters...>)
    DataType& AddInPlace(Parameters&&... p_parameters);

    bool IsFull() const { return m_count >= Capacity; }
    bool IsEmpty() const { return m_count == 0; }

    YK_SizeT CountU() const { return m_count; }
    YK_SizeT CapacityU() const { return Capacity; }

    YK_FlatIterator<DataType> begin() { return YK_FlatIterator(GetAddress(0), 0); }
    YK_FlatIterator<DataType const> begin() const { return YK_FlatIterator(GetAddress(0), 0); }
    YK_FlatIterator<DataType> end() { return YK_FlatIterator(GetAddress(0), m_count); }
    YK_FlatIterator<DataType const> end() const { return YK_FlatIterator(GetAddress(0), m_count); }

private:
    inline DataType* GetAddress(YK_SizeT p_index)
    {
        YK_Byte* const address = &m_buffer[sizeof(DataType) * p_index];
        return static_cast<DataType*>(static_cast<void*>(address));
    }

    inline DataType const* GetAddress(YK_SizeT p_index) const
    {
        YK_Byte const* const address = &m_buffer[sizeof(DataType) * p_index];
        return static_cast<DataType const*>(static_cast<void const*>(address));
    }

private:
    YK_SizeT m_count = 0;
    alignas(alignof(DataType)) YK_Byte m_buffer[sizeof(DataType) * Capacity];
};

template <typename DataType, YK_SizeT Capacity>
YK_InlineDynamicArray<DataType, Capacity>::~YK_InlineDynamicArray()
{
    for (DataType& item : *this)
    {
        YK_PlacementNew::Destruct(item);
    }
    m_count = 0;
}

template <typename DataType, YK_SizeT Capacity>
DataType& YK_InlineDynamicArray<DataType, Capacity>::operator[](YK_SizeT p_index)
{
    YK_ASSERT(p_index < m_count, "Accessing YK_InlineDynamicArray out of bounds!");
    return *GetAddress(p_index);
}

template <typename DataType, YK_SizeT Capacity>
DataType const& YK_InlineDynamicArray<DataType, Capacity>::operator[](YK_SizeT p_index) const
{
    YK_ASSERT(p_index < m_count, "Accessing YK_InlineDynamicArray out of bounds!");
    return *GetAddress(p_index);
}

template <typename DataType, YK_SizeT Capacity>
DataType& YK_InlineDynamicArray<DataType, Capacity>::Add(DataType const& p_source)
requires(std::is_copy_constructible_v<DataType>)
{
    YK_ASSERT(!IsFull(), "Pushing an element into a full YK_InlineDynamicArray!");
    DataType* item = YK_PlacementNew::New<DataType>(static_cast<void*>(GetAddress(m_count++)), p_source);
    return *item;
}

template <typename DataType, YK_SizeT Capacity>
DataType& YK_InlineDynamicArray<DataType, Capacity>::Add(DataType&& p_source)
requires(std::is_move_constructible_v<DataType>)
{
    YK_ASSERT(!IsFull(), "Pushing an element into a full YK_InlineDynamicArray!");
    DataType* item = YK_PlacementNew::New<DataType>(static_cast<void*>(GetAddress(m_count++)), std::move(p_source));
    return *item;
}

template <typename DataType, YK_SizeT Capacity>
template <typename... Parameters>
requires(std::is_constructible_v<DataType, Parameters...>)
DataType& YK_InlineDynamicArray<DataType, Capacity>::AddInPlace(Parameters&&... p_parameters)
{
    YK_ASSERT(!IsFull(), "Pushing an element into a full YK_InlineDynamicArray!");
    DataType* item = YK_PlacementNew::New<DataType>(static_cast<void*>(GetAddress(m_count++)),
                                                    std::forward<Parameters>(p_parameters)...);
    return *item;
}