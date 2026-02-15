#pragma once

// TODO: Make a hybrid array? So that the first page array is guaranteed to be inline with the class and we only NEED to
// dynamically allocate storage if we exceed the page size
template <typename DataType, YK_U32 PageSize> // TODO: Enforce Power of 2 PageSize
struct YKC_PagedArray
{
public:
    YKC_PagedArray() = default;
    YKC_PagedArray(YKC_PagedArray<DataType, PageSize> const&) = delete; // Refuse deep copies
    YKC_PagedArray(YKC_PagedArray<DataType, PageSize>&& p_other);

    ~YKC_PagedArray();

    YKC_PagedArray<DataType, PageSize>& operator=(YKC_PagedArray<DataType, PageSize>&& p_other);

    DataType& operator[](YK_U32 p_index);
    DataType const& operator[](YK_U32 p_index) const;

    // TODO: Add emplace and validate that we can move construct the data type
    // TODO: Validate that we can copy the data type
    void Push(DataType const& p_item);
    void Clear(); // TODO: Move to Deallocate function and add an actual Clear function that doesn't de-allocate

    YK_U32 Count() const { return m_count; }

private:
    struct PageIndex
    {
        YK_U32 m_pageIndex;
        YK_U32 m_itemIndex;
    };
    PageIndex GetPageIndex(YK_U32 p_linearIndex)
    {
        return PageIndex {p_linearIndex / PageSize, p_linearIndex % PageSize};
    }

    void AllocatePage();

private:
    // TODO: Replace with YKC_DynamicArray of YKC_StaticArrays?
    std::vector<DataType*> m_pages;
    YK_U32 m_count = 0; // TODO: Make Count and Capacity distinct
};

template <typename DataType, YK_U32 PageSize>
YKC_PagedArray<DataType, PageSize>::YKC_PagedArray(YKC_PagedArray<DataType, PageSize>&& p_other)
    : m_count(p_other.m_count)
{
    m_pages = std::move(p_other.m_pages);

    p_other.m_pages.clear();
    p_other.m_count = 0;
}

template <typename DataType, YK_U32 PageSize>
YKC_PagedArray<DataType, PageSize>::~YKC_PagedArray()
{
    Clear();
}

template <typename DataType, YK_U32 PageSize>
inline YKC_PagedArray<DataType, PageSize>& YKC_PagedArray<DataType, PageSize>::operator=(
  YKC_PagedArray<DataType, PageSize>&& p_other)
{
    if (m_count != 0)
    {
        Clear();
    }

    m_pages = std::move(p_other.m_pages);
    m_count = p_other.m_count;

    p_other.m_pages.clear();
    p_other.m_count = 0;
    return *this;
}

template <typename DataType, YK_U32 PageSize>
void YKC_PagedArray<DataType, PageSize>::Push(DataType const& p_item)
{
    // Note: This isn't thread safe - If Push is called concurrently we get undefined behavior
    PageIndex insertPageIndex = GetPageIndex(m_count);
    if (insertPageIndex.m_pageIndex >= m_pages.size())
    {
        // We've wrapped to a new page! Append it
        AllocatePage();
    }
    m_pages[insertPageIndex.m_pageIndex][insertPageIndex.m_itemIndex] = p_item;
    m_count++;
}

template <typename DataType, YK_U32 PageSize>
void YKC_PagedArray<DataType, PageSize>::Clear()
{
    for (DataType*& page : m_pages)
    {
        // Note: This will break for non-trivially destructable objects since we're basically just discarding the memory
        free(page);
    }
    m_pages.clear();
}

template <typename DataType, YK_U32 PageSize>
DataType& YKC_PagedArray<DataType, PageSize>::operator[](YK_U32 p_index)
{
    if (p_index >= m_count)
    {
        YK_LOG_ERROR("Accessing YKC_PagedArray out of bounds!");
        // TODO: Assert
        static DataType dummy{};
        return dummy;
        // return *static_cast<DataType*>(nullptr);
    }
    PageIndex pageIndex = GetPageIndex(p_index);
    return m_pages[pageIndex.m_pageIndex][pageIndex.m_itemIndex];
}

template <typename DataType, YK_U32 PageSize>
DataType const& YKC_PagedArray<DataType, PageSize>::operator[](YK_U32 p_index) const
{
    if (p_index >= m_count)
    {
        YK_LOG_ERROR("Accessing YKC_PagedArray out of bounds!");
        // TODO: Assert
        static DataType dummy{};
        return dummy;
        //return *static_cast<DataType*>(nullptr);
    }
    PageIndex pageIndex = GetPageIndex(p_index);
    return m_pages[pageIndex.m_pageIndex][pageIndex.m_itemIndex];
}

template <typename DataType, YK_U32 PageSize>
void YKC_PagedArray<DataType, PageSize>::AllocatePage()
{
    constexpr YK_U32 PageSizeBytes = sizeof(DataType) * PageSize;
    m_pages.push_back(static_cast<DataType*>(malloc(PageSizeBytes)));
}