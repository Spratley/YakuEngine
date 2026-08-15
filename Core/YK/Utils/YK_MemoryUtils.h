#pragma once

#include <new>

// I build YakuEn projects with all warnings as errors, this is to suppress unused object warnings
template <typename... Args>
constexpr void YK_Unused(Args&&...)
{}

// Is this really needed?
#define YK_SAFE_DELETE(ptr) do { delete ptr; ptr = nullptr; } while(false)
#define YK_SAFE_DELETE_ARRAY(X) { delete[] X; X = nullptr; }

// This just seems lazy, past Jacob
#define YK_STEAL_MEMBER(MEMBER, SOURCE, CLEAR_VALUE) { MEMBER = SOURCE.MEMBER; SOURCE.MEMBER = CLEAR_VALUE; }

namespace YK_PlacementNew
{
    template <typename Type, typename... Parameters>
    [[nodiscard]] inline Type* New(void* p_address, Parameters&&... p_parameters)
    {
        Type* result = ::new (p_address) Type(std::forward<Parameters>(p_parameters)...);
        return std::launder(result);
    }

    template <typename Type>
    inline void Destruct(Type& p_item)
    {
        p_item.~Type();
    }
} // namespace YK_PlacementNew