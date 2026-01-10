#pragma once

// TODO: Come back and consider the lifetime of objects in storage
// Should handles be ref-counters to automatically clean up stored objects once they are dereferenced?

template <typename DataType, YK_U32 PageSize>
struct YKC_HandleBase;

template <typename DataType, YK_U32 PageSize>
struct YKC_HandledStorage
{
public:
    // TODO: Migrate this into a singleton interface that's not JIT
    // (Also make a separate JIT singleton since that's also useful)
    static YKC_HandledStorage* GetInstance() { return s_instance; }

private:
    inline static YKC_HandledStorage* s_instance = nullptr;

public:
    YKC_HandledStorage();
    YKC_HandledStorage(YKC_HandledStorage<DataType, PageSize> const&) = delete;
    YKC_HandledStorage(YKC_HandledStorage<DataType, PageSize>&&) = delete;
    ~YKC_HandledStorage();

    YKC_HandleBase<DataType, PageSize> EmplaceHandled(DataType&& p_object);

    DataType* Get(YK_U32 p_index, YK_U32 p_generation);
    DataType const* Get(YK_U32 p_index, YK_U32 p_generation) const;

private:
    struct HandledObject
    {
        DataType m_object;
        YK_U32 m_generation;
    };
    YKC_PagedArray<HandledObject, PageSize> m_storage;
};

template <typename DataType, YK_U32 PageSize>
YKC_HandledStorage<DataType, PageSize>::YKC_HandledStorage() : m_storage()
{
    if (s_instance)
    {
        // TODO: Assert
        YK_LOG_ERROR("Re-creating YKC_HandledStorage! Only one should exist at any time!");
    }
    s_instance = this;
}

template <typename DataType, YK_U32 PageSize>
inline YKC_HandledStorage<DataType, PageSize>::~YKC_HandledStorage()
{
    s_instance = nullptr;
}

template <typename DataType, YK_U32 PageSize>
YKC_HandleBase<DataType, PageSize> YKC_HandledStorage<DataType, PageSize>::EmplaceHandled(DataType&& p_object)
{
    YK_U32 const objectIndex = m_storage.Count();
    m_storage.Push(HandledObject {p_object, 0}); // Don't push, just emplace
    return YKC_HandleBase<DataType, PageSize>(objectIndex, 0);
}

template <typename DataType, YK_U32 PageSize>
inline DataType* YKC_HandledStorage<DataType, PageSize>::Get(YK_U32 p_index, YK_U32 p_generation)
{
    if (m_storage.Count() <= p_index)
    {
        // Attempting to access out of bounds!
        return nullptr;
    }

    HandledObject& object = m_storage[p_index];
    if (object.m_generation != p_generation)
    {
        return nullptr;
    }
    return &object.m_object;
}

template <typename DataType, YK_U32 PageSize>
inline DataType const* YKC_HandledStorage<DataType, PageSize>::Get(YK_U32 p_index, YK_U32 p_generation) const
{
    if (m_storage.Count() <= p_index)
    {
        // Attempting to access out of bounds!
        return nullptr;
    }

    HandledObject const& object = m_storage[p_index];
    if (object.m_generation != p_generation)
    {
        return nullptr;
    }
    return &object.m_object;
}
