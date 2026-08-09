#pragma once

#include "YK/Types/Math/YK_Integer.h"
#include "YK/Types/Traits/YK_TypeTraits.h"

#include <vector>

// TODO: Add an iterator that is aware of page size and jumps to the next page on its own without doing any math
template <typename DataType, YK_SizeT PageSize>
requires(IsPowerOfTwo_v<PageSize>)
struct YK_PagedArray
{
public:
    YK_PagedArray() = default;
    YK_PagedArray(YK_PagedArray&& p_other);
    // Refuse deep copies, can be implemented if the need arises
    YK_PagedArray(YK_PagedArray const&) = delete;

    ~YK_PagedArray();

    // Refuse deep copies, can be implemented if the need arises
    YK_PagedArray& operator=(YK_PagedArray const&) = delete;
    YK_PagedArray& operator=(YK_PagedArray&& p_other);

    DataType& operator[](YK_SizeT p_index);
    DataType const& operator[](YK_SizeT p_index) const;

    // TODO: Add emplace and validate that we can move construct the data type
    // TODO: Validate that we can copy the data type
    void Push(DataType const& p_item);

    void Clear(); // TODO: Move to Deallocate function and add an actual Clear function that doesn't de-allocate

    YK_SizeT Count() const { return m_count; }
    YK_SizeT Capacity() const { return m_pages.size() * PageSize; }

private:
    struct PageIndex
    {
        YK_SizeT m_pageIndex;
        YK_SizeT m_itemIndex;
    };
    constexpr PageIndex GetPageIndex(YK_SizeT p_linearIndex)
    {
        return PageIndex{ p_linearIndex / PageSize, p_linearIndex % PageSize };
    }

    void AllocatePage();

private:
    // TODO: Replace with YK_DynamicArray of YK_StaticArrays
    std::vector<std::array<DataType, PageSize>> m_pages;
    YK_SizeT m_count = 0;
};

template <typename DataType, YK_SizeT PageSize>
requires(IsPowerOfTwo_v<PageSize>)
YK_PagedArray<DataType, PageSize>::YK_PagedArray(YK_PagedArray&& p_other)
    : m_count(p_other.m_count)
{
    Clear();

    m_pages = std::move(p_other.m_pages);
    m_count = p_other.m_count;

    p_other.m_pages.clear();
    p_other.m_count = 0;
}

template <typename DataType, YK_SizeT PageSize>
requires(IsPowerOfTwo_v<PageSize>)
YK_PagedArray<DataType, PageSize>::~YK_PagedArray()
{
    Clear();
}

template <typename DataType, YK_SizeT PageSize>
requires(IsPowerOfTwo_v<PageSize>)
inline YK_PagedArray<DataType, PageSize>& YK_PagedArray<DataType, PageSize>::operator=(
  YK_PagedArray<DataType, PageSize>&& p_other)
{
    Clear();

    m_pages = std::move(p_other.m_pages);
    m_count = p_other.m_count;

    p_other.m_pages.clear();
    p_other.m_count = 0;
    return *this;
}

template <typename DataType, YK_SizeT PageSize>
requires(IsPowerOfTwo_v<PageSize>)
void YK_PagedArray<DataType, PageSize>::Push(DataType const& p_item)
{
    // Note: This isn't thread safe - If Push is called concurrently we get undefined behavior
    // TODO: Add a lock? Do I care?
    PageIndex insertPageIndex = GetPageIndex(m_count);
    if (insertPageIndex.m_pageIndex >= m_pages.size())
    {
        // We've wrapped to a new page! Append it
        AllocatePage();
    }
    m_pages[insertPageIndex.m_pageIndex][insertPageIndex.m_itemIndex] = p_item;
    m_count++;
}

template <typename DataType, YK_SizeT PageSize>
requires(IsPowerOfTwo_v<PageSize>)
void YK_PagedArray<DataType, PageSize>::Clear()
{
    for (DataType*& page : m_pages)
    {
        // Note: This will break for non-trivially destructable objects since we're basically just discarding the memory
        free(page);
    }
    m_pages.clear();
}

template <typename DataType, YK_SizeT PageSize>
requires(IsPowerOfTwo_v<PageSize>)
DataType& YK_PagedArray<DataType, PageSize>::operator[](YK_SizeT p_index)
{
    if (p_index >= m_count)
    {
        YK_LOG_ERROR("Accessing YK_PagedArray out of bounds!");
        // TODO: Assert
        static DataType dummy{};
        return dummy;
        // return *static_cast<DataType*>(nullptr);
    }
    PageIndex pageIndex = GetPageIndex(p_index);
    return m_pages[pageIndex.m_pageIndex][pageIndex.m_itemIndex];
}

template <typename DataType, YK_SizeT PageSize>
requires(IsPowerOfTwo_v<PageSize>)
DataType const& YK_PagedArray<DataType, PageSize>::operator[](YK_SizeT p_index) const
{
    if (p_index >= m_count)
    {
        YK_LOG_ERROR("Accessing YK_PagedArray out of bounds!");
        // TODO: Assert
        static DataType dummy{};
        return dummy;
        // return *static_cast<DataType*>(nullptr);
    }
    PageIndex pageIndex = GetPageIndex(p_index);
    return m_pages[pageIndex.m_pageIndex][pageIndex.m_itemIndex];
}

template <typename DataType, YK_SizeT PageSize>
requires(IsPowerOfTwo_v<PageSize>)
void YK_PagedArray<DataType, PageSize>::AllocatePage()
{
    constexpr YK_SizeT PageSizeBytes = sizeof(DataType) * PageSize;
    m_pages.push_back(static_cast<DataType*>(malloc(PageSizeBytes)));
}