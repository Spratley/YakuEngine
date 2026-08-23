#pragma once

#include "YK/Debugging/YK_Assert.h"
#include "YK/Math/YK_NumericLimits.h"
#include "YK/Types/Containers/YK_StaticArray.h"
#include "YK/Types/Math/YK_Integer.h"
#include "YK/Types/Pointers/YK_UniquePointer.h"
#include "YK/Types/Traits/YK_Concepts.h"
#include "YK/Types/Traits/YK_TypeTraits.h"
#include "YK/Utils/YK_MemoryUtils.h"

#include <concepts>
#include <type_traits>
#include <vector>

template <typename DataType, YK_SizeT BlockCapacity>
struct YK_ColonyArrayIterator;

template <typename DataType, YK_SizeT BlockCapacity>
requires(YK_IsPowerOfTwo<BlockCapacity>)
struct YK_ColonyArray
{
    friend struct YK_ColonyArrayIterator<DataType, BlockCapacity>;
    friend struct YK_ColonyArrayIterator<DataType const, BlockCapacity>;
    static constexpr YK_SizeT InvalidIndex = YK_NumericLimits<YK_SizeT>::Max;
    struct Index
    {
        YK_SizeT m_blockIndex = InvalidIndex;
        YK_SizeT m_localIndex = InvalidIndex;

        bool operator!=(Index const& p_rhs) const
        {
            return m_blockIndex != p_rhs.m_blockIndex || m_localIndex != p_rhs.m_localIndex;
        }

        constexpr bool IsValid() const { return m_blockIndex != InvalidIndex && m_localIndex != InvalidIndex; }
    };

public:
    YK_ColonyArray() = default;
    ~YK_ColonyArray();

    YK_ColonyArray(YK_ColonyArray const&) = delete;
    YK_ColonyArray(YK_ColonyArray&&) = delete;
    YK_ColonyArray& operator=(YK_ColonyArray const&) = delete;
    YK_ColonyArray& operator=(YK_ColonyArray&&) = delete;

    DataType* operator[](Index p_index);
    DataType const* operator[](Index p_index) const;

    Index Add(DataType const& p_item)
    requires(std::is_copy_constructible_v<DataType>);

    Index Add(DataType&& p_item)
    requires(std::is_move_constructible_v<DataType>);

    template <typename... Parameters>
    requires(std::is_constructible_v<DataType, Parameters...>)
    DataType& AddInPlace(Parameters&&... p_parameters);

    void Remove(Index const& p_index);
    void Remove(DataType const& p_item)
    requires(std::equality_comparable<DataType>)
    {
        Remove(Find(p_item));
    }

    Index Find(DataType const& p_item) const
    requires(std::equality_comparable<DataType>);

    YK_ColonyArrayIterator<DataType, BlockCapacity> begin()
    {
        Index startIndex{ 0, 0 };
        if (!ValidateIndex(startIndex))
        {
            return end();
        }
        if (!Exists(startIndex))
        {
            IncrementIndex(startIndex);
        }
        return YK_ColonyArrayIterator<DataType, BlockCapacity>(*this, startIndex);
    }
    YK_ColonyArrayIterator<DataType const, BlockCapacity> begin() const
    {
        Index startIndex{ 0, 0 };
        if (!ValidateIndex(startIndex))
        {
            return end();
        }
        if (!Exists(startIndex))
        {
            IncrementIndex(startIndex);
        }
        return YK_ColonyArrayIterator<DataType const, BlockCapacity>(*this, startIndex);
    }
    YK_ColonyArrayIterator<DataType, BlockCapacity> end()
    {
        return YK_ColonyArrayIterator<DataType, BlockCapacity>(*this, Index{ InvalidIndex, InvalidIndex });
    }
    YK_ColonyArrayIterator<DataType const, BlockCapacity> end() const
    {
        return YK_ColonyArrayIterator<DataType const, BlockCapacity>(*this, Index{ InvalidIndex, InvalidIndex });
    }

private:
    static constexpr YK_SizeT ToFlatIndex(Index const& p_index)
    {
        return (p_index.m_blockIndex * BlockCapacity) + p_index.m_localIndex;
    }

    inline void IncrementIndex(Index& p_index) const;

    inline bool ValidateIndex(Index const& p_index) const
    {
        return p_index.m_blockIndex < m_blocks.size() && p_index.m_localIndex < BlockCapacity;
    }

    inline bool Exists(Index& p_index) const { return m_skipField.Exists(ToFlatIndex(p_index)); }

    DataType* FindNextFreeAddress(Index& p_outIndex);
    void AllocateBlock();

private:
    union BlockItem
    {
        DataType m_item;

        BlockItem() {}
        ~BlockItem() {}
    };

    struct SkipField
    {
        struct SkipFieldEntry
        {
            YK_SizeT m_skip = InvalidIndex;
            Index m_nextEmptyIndex;
            Index m_previousEmptyIndex;
        };

        void AllocateBlock();

        bool InsertSkip(Index const& p_index, Index& p_firstEmptyIndex);
        void RemoveSkip(Index& p_firstEmptyIndex);

        void SetNextIndex(Index const& p_item, Index const& p_nextIndex);
        void SetPreviousIndex(Index const& p_item, Index const& p_previousIndex);

        bool Exists(YK_SizeT p_flatIndex) const { return m_data[p_flatIndex].m_skip == 0; }

        std::vector<SkipFieldEntry> m_data;
    };

    std::vector<YK_UniquePointer<YK_StaticArray<BlockItem, BlockCapacity>>> m_blocks;
    SkipField m_skipField;
    Index m_firstEmptyIndex;
};

template <typename DataType, YK_SizeT BlockCapacity>
struct YK_ColonyArrayIterator
{
    using SourceColonyType = YK_CopyCV_T<DataType, YK_ColonyArray<std::remove_cv_t<DataType>, BlockCapacity>>;
    using Index = typename YK_ColonyArray<std::remove_cv_t<DataType>, BlockCapacity>::Index;

public:
    YK_ColonyArrayIterator(SourceColonyType& p_sourceColony, Index const& p_index)
        : m_sourceColony(p_sourceColony)
        , m_index(p_index)
    {}

    DataType& operator*() const { return *m_sourceColony[m_index]; }
    YK_ColonyArrayIterator& operator++()
    {
        m_sourceColony.IncrementIndex(m_index);
        return *this;
    }

    friend bool operator!=(YK_ColonyArrayIterator const& p_lhs, YK_ColonyArrayIterator const& p_rhs)
    {
        return p_lhs.m_index != p_rhs.m_index || &p_lhs.m_sourceColony != &p_rhs.m_sourceColony;
    }

    Index const& GetIndex() const { return m_index; }

private:
    SourceColonyType& m_sourceColony;
    Index m_index;
};

template <typename DataType, YK_SizeT BlockCapacity>
requires(YK_IsPowerOfTwo<BlockCapacity>)
YK_ColonyArray<DataType, BlockCapacity>::~YK_ColonyArray()
{
    for (DataType& item : *this)
    {
        YK_PlacementNew::Destruct(item);
    }
}

template <typename DataType, YK_SizeT BlockCapacity>
requires(YK_IsPowerOfTwo<BlockCapacity>)
DataType* YK_ColonyArray<DataType, BlockCapacity>::operator[](Index p_index)
{
    YK_ASSERT(ValidateIndex(p_index), "Attempting to access YK_ColonyArray out of bounds!");
    if (!Exists(p_index))
    {
        return nullptr;
    }
    return &m_blocks[p_index.m_blockIndex]->operator[](p_index.m_localIndex).m_item;
}

template <typename DataType, YK_SizeT BlockCapacity>
requires(YK_IsPowerOfTwo<BlockCapacity>)
DataType const* YK_ColonyArray<DataType, BlockCapacity>::operator[](Index p_index) const
{
    YK_ASSERT(ValidateIndex(p_index), "Attempting to access YK_ColonyArray out of bounds!");
    if (!Exists(p_index))
    {
        return nullptr;
    }
    return &m_blocks[p_index.m_blockIndex]->operator[](p_index.m_localIndex).m_item;
}

template <typename DataType, YK_SizeT BlockCapacity>
requires(YK_IsPowerOfTwo<BlockCapacity>)
YK_ColonyArray<DataType, BlockCapacity>::Index YK_ColonyArray<DataType, BlockCapacity>::Add(DataType const& p_item)
requires(std::is_copy_constructible_v<DataType>)
{
    Index outIndex;
    DataType* dummy = YK_PlacementNew::New(FindNextFreeAddress(outIndex), p_item);
    YK_Unused(dummy);
    return outIndex;
}

template <typename DataType, YK_SizeT BlockCapacity>
requires(YK_IsPowerOfTwo<BlockCapacity>)
YK_ColonyArray<DataType, BlockCapacity>::Index YK_ColonyArray<DataType, BlockCapacity>::Add(DataType&& p_item)
requires(std::is_move_constructible_v<DataType>)
{
    Index outIndex;
    DataType* dummy = YK_PlacementNew::New(FindNextFreeAddress(outIndex), std::move(p_item));
    YK_Unused(dummy);
    return outIndex;
}

template <typename DataType, YK_SizeT BlockCapacity>
requires(YK_IsPowerOfTwo<BlockCapacity>)
template <typename... Parameters>
requires(std::is_constructible_v<DataType, Parameters...>)
DataType& YK_ColonyArray<DataType, BlockCapacity>::AddInPlace(Parameters&&... p_parameters)
{
    Index dummy;
    return *YK_PlacementNew::New(FindNextFreeAddress(dummy), std::forward<Parameters>(p_parameters)...);
}

template <typename DataType, YK_SizeT BlockCapacity>
requires(YK_IsPowerOfTwo<BlockCapacity>)
void YK_ColonyArray<DataType, BlockCapacity>::Remove(Index const& p_index)
{
    if (!ValidateIndex(p_index))
    {
        return;
    }

    DataType* item = (*this)[p_index];
    if (!m_skipField.InsertSkip(p_index, m_firstEmptyIndex))
    {
        return;
    }
    YK_PlacementNew::Destruct(*item);
}

template <typename DataType, YK_SizeT BlockCapacity>
requires(YK_IsPowerOfTwo<BlockCapacity>)
YK_ColonyArray<DataType, BlockCapacity>::Index YK_ColonyArray<DataType, BlockCapacity>::Find(
  DataType const& p_item) const
requires(std::equality_comparable<DataType>)
{
    using Iterator = YK_ColonyArrayIterator<DataType const, BlockCapacity>;
    for (Iterator _begin = begin(), _end = end(); _begin != _end; ++_begin)
    {
        if (p_item == *_begin)
        {
            return _begin.GetIndex();
        }
    }
    return Index();
}

template <typename DataType, YK_SizeT BlockCapacity>
requires(YK_IsPowerOfTwo<BlockCapacity>)
void YK_ColonyArray<DataType, BlockCapacity>::AllocateBlock()
{
    m_blocks.push_back(std::move(YK_UniquePointer<YK_StaticArray<BlockItem, BlockCapacity>>::Make()));
    m_skipField.AllocateBlock();

    Index newBlockIndex = Index{ .m_blockIndex = m_blocks.size() - 1, .m_localIndex = 0 };
    if (ValidateIndex(m_firstEmptyIndex))
    {
        m_skipField.SetNextIndex(newBlockIndex, m_firstEmptyIndex);
        m_skipField.SetPreviousIndex(m_firstEmptyIndex, newBlockIndex);
    }
    m_firstEmptyIndex = newBlockIndex;
}

template <typename DataType, YK_SizeT BlockCapacity>
requires(YK_IsPowerOfTwo<BlockCapacity>)
void YK_ColonyArray<DataType, BlockCapacity>::IncrementIndex(Index& p_index) const
{
    auto Increment = [&p_index](YK_SizeT p_amount) {
        p_index.m_localIndex += p_amount;
        while (p_index.m_localIndex >= BlockCapacity)
        {
            p_index.m_localIndex -= BlockCapacity;
            ++p_index.m_blockIndex;
        }
    };

    YK_SizeT flatIndex = ToFlatIndex(p_index);
    do
    {
        YK_SizeT skipAmount = m_skipField.Exists(flatIndex) ? 1 : m_skipField.m_data[flatIndex].m_skip;
        Increment(skipAmount);
        flatIndex += skipAmount;
        if (!ValidateIndex(p_index))
        {
            p_index.m_blockIndex = InvalidIndex;
            p_index.m_localIndex = InvalidIndex;
            return;
        }
    } while (!m_skipField.Exists(flatIndex));
}

template <typename DataType, YK_SizeT BlockCapacity>
requires(YK_IsPowerOfTwo<BlockCapacity>)
DataType* YK_ColonyArray<DataType, BlockCapacity>::FindNextFreeAddress(Index& p_outIndex)
{
    if (!ValidateIndex(m_firstEmptyIndex))
    {
        AllocateBlock();
    }
    p_outIndex = m_firstEmptyIndex;
    BlockItem& item = m_blocks[m_firstEmptyIndex.m_blockIndex]->operator[](m_firstEmptyIndex.m_localIndex);
    m_skipField.RemoveSkip(m_firstEmptyIndex);
    return &item.m_item;
}

template <typename DataType, YK_SizeT BlockCapacity>
requires(YK_IsPowerOfTwo<BlockCapacity>)
void YK_ColonyArray<DataType, BlockCapacity>::SkipField::AllocateBlock()
{
    YK_SizeT const newBlockStart = m_data.size();
    YK_SizeT const newSize = newBlockStart + BlockCapacity;
    YK_SizeT const newBlockEnd = newSize - 1;
    m_data.resize(newSize);

    SkipFieldEntry& fieldStart = m_data[newBlockStart];
    SkipFieldEntry& fieldEnd = m_data[newBlockEnd];

    fieldStart.m_skip = BlockCapacity;
    fieldEnd.m_skip = BlockCapacity;
}

template <typename DataType, YK_SizeT BlockCapacity>
requires(YK_IsPowerOfTwo<BlockCapacity>)
bool YK_ColonyArray<DataType, BlockCapacity>::SkipField::InsertSkip(Index const& p_index, Index& p_firstEmptyIndex)
{
    YK_SizeT const flatIndex = ToFlatIndex(p_index);

    if (m_data[flatIndex].m_skip != 0)
    {
        // Index is already marked to be skipped!
        return false;
    }

    auto GetNeighbor = [this, &flatIndex](bool aNextNeighbor) -> SkipFieldEntry* {
        if (aNextNeighbor && flatIndex == m_data.size() - 1)
        {
            return nullptr;
        }
        if (!aNextNeighbor && flatIndex == 0)
        {
            return nullptr;
        }
        return &m_data[flatIndex + (aNextNeighbor ? 1 : -1)];
    };

    SkipFieldEntry& item = m_data[flatIndex];
    SkipFieldEntry* nextItem = GetNeighbor(true);
    SkipFieldEntry* previousItem = GetNeighbor(false);

    bool hasSkipNext = nextItem && nextItem->m_skip != 0;
    bool hasSkipPrevious = previousItem && previousItem->m_skip != 0;

    if (!hasSkipNext && !hasSkipPrevious)
    {
        item.m_skip = 1;

        item.m_nextEmptyIndex = p_firstEmptyIndex;
        item.m_previousEmptyIndex = Index{};

        if (p_firstEmptyIndex.IsValid())
        {
            SetPreviousIndex(p_firstEmptyIndex, p_index);
        }
        p_firstEmptyIndex = p_index;
    }
    else if (hasSkipNext && !hasSkipPrevious)
    {
        if (nextItem->m_skip == 1)
        {
            // Next item is just a head! Make it our tail
            item.m_skip = 2;
            nextItem->m_skip = 2;
        }
        else
        {
            item.m_skip = nextItem->m_skip + 1;
            nextItem->m_skip = InvalidIndex;
            m_data[flatIndex + item.m_skip - 1].m_skip = item.m_skip;
        }

        item.m_nextEmptyIndex = nextItem->m_nextEmptyIndex;
        item.m_previousEmptyIndex = nextItem->m_previousEmptyIndex;

        if (item.m_previousEmptyIndex.IsValid())
        {
            SetNextIndex(item.m_previousEmptyIndex, p_index);
        }
        else
        {
            // This was the start of the empty list! Update the head index
            p_firstEmptyIndex = p_index;
        }

        if (item.m_nextEmptyIndex.IsValid())
        {
            SetPreviousIndex(item.m_nextEmptyIndex, p_index);
        }
    }
    else if (!hasSkipNext && hasSkipPrevious)
    {
        if (previousItem->m_skip == 1)
        {
            // Next item is just a head! Become its tail
            item.m_skip = 2;
            previousItem->m_skip = 2;
        }
        else
        {
            item.m_skip = previousItem->m_skip + 1;
            previousItem->m_skip = InvalidIndex;
            m_data[flatIndex - item.m_skip + 1].m_skip = item.m_skip;
        }

        item.m_nextEmptyIndex = previousItem->m_nextEmptyIndex;
        item.m_previousEmptyIndex = previousItem->m_previousEmptyIndex;
    }
    else
    {
        // Has skip fields in both directions! We gotta merge!
        // Previous will expand to absorb next
        // Next's neighbors will attach to each other
        item.m_skip = InvalidIndex;

        // Link together the next block's connections to sever it from the list
        if (nextItem->m_nextEmptyIndex.IsValid())
        {
            SetPreviousIndex(nextItem->m_nextEmptyIndex, nextItem->m_previousEmptyIndex);
        }
        if (nextItem->m_previousEmptyIndex.IsValid())
        {
            SetNextIndex(nextItem->m_previousEmptyIndex, nextItem->m_nextEmptyIndex);
        }
        else
        {
            // Next item was the head of the list!
            p_firstEmptyIndex = nextItem->m_nextEmptyIndex;
        }

        // Update previous item to encompass the whole block
        YK_SizeT const totalSkip = previousItem->m_skip + 1 + nextItem->m_skip;
        SkipFieldEntry& previousHead = m_data[flatIndex - previousItem->m_skip];
        SkipFieldEntry& nextTail = m_data[flatIndex + nextItem->m_skip];
        nextItem->m_skip = InvalidIndex;
        previousItem->m_skip = InvalidIndex;

        previousHead.m_skip = totalSkip;
        nextTail.m_nextEmptyIndex = previousHead.m_nextEmptyIndex;
        nextTail.m_previousEmptyIndex = previousHead.m_previousEmptyIndex;
        nextTail.m_skip = totalSkip;
    }

    return true;
}

template <typename DataType, YK_SizeT BlockCapacity>
requires(YK_IsPowerOfTwo<BlockCapacity>)
void YK_ColonyArray<DataType, BlockCapacity>::SkipField::RemoveSkip(Index& p_firstEmptyIndex)
{
    YK_ASSERT(p_firstEmptyIndex.IsValid(), "Attempting to call RemoveSkip() when there's no space left!");
    YK_SizeT const flatIndex = ToFlatIndex(p_firstEmptyIndex);
    SkipFieldEntry& item = m_data[flatIndex];

    if (item.m_skip == 1)
    {
        item.m_skip = 0;
        if (item.m_nextEmptyIndex.IsValid())
        {
            SetPreviousIndex(item.m_nextEmptyIndex, Index{}); // Invalidate previous index pointer for next
        }
        p_firstEmptyIndex = item.m_nextEmptyIndex;
        item.m_nextEmptyIndex = Index{};
        return;
    }

    SkipFieldEntry& newHead = m_data[flatIndex + 1];
    newHead.m_skip = item.m_skip - 1;
    newHead.m_previousEmptyIndex = Index{};
    m_data[flatIndex + newHead.m_skip].m_skip = newHead.m_skip;
    item.m_skip = 0;

    p_firstEmptyIndex.m_localIndex++;
    if (p_firstEmptyIndex.m_localIndex == BlockCapacity)
    {
        p_firstEmptyIndex.m_blockIndex++;
        p_firstEmptyIndex.m_localIndex = 0;
    }

    SetNextIndex(p_firstEmptyIndex, item.m_nextEmptyIndex);
    if (item.m_nextEmptyIndex.IsValid())
    {
        SetPreviousIndex(item.m_nextEmptyIndex, p_firstEmptyIndex);
    }
}

template <typename DataType, YK_SizeT BlockCapacity>
requires(YK_IsPowerOfTwo<BlockCapacity>)
inline void YK_ColonyArray<DataType, BlockCapacity>::SkipField::SetNextIndex(Index const& p_item,
                                                                             Index const& p_nextIndex)
{
    YK_SizeT flatIndex = ToFlatIndex(p_item);
    SkipFieldEntry& entry = m_data[flatIndex];
    entry.m_nextEmptyIndex = p_nextIndex;
    m_data[flatIndex + entry.m_skip - 1].m_nextEmptyIndex = p_nextIndex;
}

template <typename DataType, YK_SizeT BlockCapacity>
requires(YK_IsPowerOfTwo<BlockCapacity>)
inline void YK_ColonyArray<DataType, BlockCapacity>::SkipField::SetPreviousIndex(Index const& p_item,
                                                                                 Index const& p_previousIndex)
{
    YK_SizeT flatIndex = ToFlatIndex(p_item);
    SkipFieldEntry& entry = m_data[flatIndex];
    entry.m_previousEmptyIndex = p_previousIndex;
    m_data[flatIndex + entry.m_skip - 1].m_previousEmptyIndex = p_previousIndex;
}