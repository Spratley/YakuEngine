#pragma once

template <typename DataType, YK_U32 PageSize>
struct YKC_HandledStorage;

template <typename DataType, YK_U32 PageSize>
struct YKC_HandleBase
{
    friend struct YKC_HandledStorage<DataType, PageSize>;

public:
    inline static constexpr YKC_HandleBase<DataType, PageSize> InvalidHandle() { return {YK_U32_Max, YK_U32_Max}; }

public:
    constexpr YKC_HandleBase() : m_index(YK_U32_Max), m_generation(YK_U32_Max) {}
    constexpr YKC_HandleBase(YKC_HandleBase<DataType, PageSize> const& p_other)
        : m_index(p_other.m_index)
        , m_generation(p_other.m_generation)
    {}

    inline DataType* operator->() { return Get(); }
    inline DataType* Get() const
    {
        return YKC_HandledStorage<DataType, PageSize>::GetInstance()->Get(m_index, m_generation);
    }

    // TODO: Implement
    bool IsValid() { return false; }

private:
    constexpr YKC_HandleBase(YK_U32 p_index, YK_U32 p_generation) : m_index(p_index), m_generation(p_generation) {}

private:
    YK_U32 m_index;
    YK_U32 m_generation;
};