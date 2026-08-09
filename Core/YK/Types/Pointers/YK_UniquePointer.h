#pragma once

#include <type_traits>

template <typename Type>
struct YK_UniquePointer
{
public:
    template <typename... Parameters>
    requires(std::is_constructible_v<Type, Parameters...>)
    static YK_UniquePointer Make(Parameters&&... p_parameters)
    {
        return YK_UniquePointer(new Type(std::forward<Parameters>(p_parameters)...));
    }

public:
    YK_UniquePointer() = default;

    YK_UniquePointer(YK_UniquePointer&& p_other)
    {
        delete m_item;
        m_item = p_other.m_item;
        p_other.m_item = nullptr;
    }

    YK_UniquePointer& operator=(YK_UniquePointer&& p_other)
    {
        delete m_item;
        m_item = p_other.m_item;
        p_other.m_item = nullptr;
        return *this;
    }

    YK_UniquePointer(YK_UniquePointer const&) = delete;
    YK_UniquePointer& operator=(YK_UniquePointer const&) = delete;

    ~YK_UniquePointer()
    {
        delete m_item;
        m_item = nullptr;
    }

    Type* operator->() const { return m_item; }

    bool IsValid() const { return m_item != nullptr; }

private:
    YK_UniquePointer(Type* p_rawPointer)
        : m_item(p_rawPointer)
    {}

private:
    Type* m_item = nullptr;
};