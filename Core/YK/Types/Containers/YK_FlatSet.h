#pragma once

#include "YK/Types/Containers/Iterator/YK_FlatIterator.h"
#include "YK/Utils/YK_AlgorithmUtils.h"

#include <vector> // TODO: Replace with YK_DynamicArray
#include <concepts>

// Set that uses a continuous array under the hood
// Used for small counts where a hash would be more work than its worth
template <typename Type>
requires(std::equality_comparable<Type>)
struct YK_FlatSet
{
public:
    YK_FlatSet() = default;
    ~YK_FlatSet() = default;

    bool Insert(Type const& p_item);
    bool Insert(Type&& p_item);

    bool Contains(Type const& p_item) const;

    void Remove(Type const& p_item);

    YK_FlatIterator<Type> begin() { return YK_FlatIterator<Type>(m_data.data(), 0); }
    YK_FlatIterator<Type const> begin() const { return YK_FlatIterator<Type const>(m_data.data(), 0); }
    YK_FlatIterator<Type const> end() const { return YK_FlatIterator<Type const>(m_data.data(), m_data.size()); }

private:
    std::vector<Type> m_data;
};

template <typename Type>
requires(std::equality_comparable<Type>)
bool YK_FlatSet<Type>::Insert(Type const& p_item)
{
    if (Contains(p_item))
    {
        return false;
    }

    m_data.push_back(p_item);
    return true;
}

template <typename Type>
requires(std::equality_comparable<Type>)
bool YK_FlatSet<Type>::Insert(Type&& p_item)
{
    if (Contains(p_item))
    {
        return false;
    }

    m_data.emplace_back(std::move(p_item));
    return true;
}

template <typename Type>
requires(std::equality_comparable<Type>)
bool YK_FlatSet<Type>::Contains(Type const& p_item) const
{
    for (Type const& item : m_data)
    {
        if (p_item == item)
        {
            return true;
        }
    }
    return false;
}

template <typename Type>
requires(std::equality_comparable<Type>)
void YK_FlatSet<Type>::Remove(Type const& p_item)
{
    for (auto i : YK_CountTo(m_data.size()))
    {
        if (p_item == m_data[i])
        {
            m_data.erase(i);
            return;
        }
    }
}