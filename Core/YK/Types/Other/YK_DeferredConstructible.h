#pragma once
#include "YK/Utils/YK_MemoryUtils.h"

// A babysitter-less type, effectively an inline storage pointer that's even more dangerous because it won't segfault and just
// operate on bad data like everything's okay. Use with extreme caution, make sure Construct and Destruct are BOTH
// called. You are in charge of the lifetime of this object so treat it with the respect it deserves.
template <typename Type>
struct YK_DeferredConstructible
{
    constexpr YK_DeferredConstructible() = default;
    constexpr ~YK_DeferredConstructible() = default;

    YK_DeferredConstructible(YK_DeferredConstructible const&) = delete;
    YK_DeferredConstructible(YK_DeferredConstructible&&) = delete;

    template <typename... Parameters>
    constexpr Type& Construct(Parameters&&... p_parameters)
    {
        return *YK_PlacementNew::New<Type>(&m_storage.m_object, std::forward<Parameters>(p_parameters)...);
    }

    constexpr void Destruct() { YK_PlacementNew::Destruct<Type>(m_storage.m_object); }

    constexpr Type* operator->() { return &m_storage.m_object; }
    constexpr Type const* operator->() const { return &m_storage.m_object; }
    constexpr Type& operator*() { return m_storage.m_object; }
    constexpr Type const& operator*() const { return m_storage.m_object; }

    constexpr Type& Get() { return m_storage.m_object; }
    constexpr Type const& Get() const { return m_storage.m_object; }

    union DeferredObject
    {
        DeferredObject() {}
        ~DeferredObject() {}

        Type m_object;
    } m_storage;
};