#pragma once

#include "YK/Math/YK_NumericLimits.h"
#include "YK/Types/Math/YK_Integer.h"
#include "YK/Types/Traits/YK_TypeTraits.h"

#include <memory>

// I build YakuEn projects with all warnings as errors, this is to suppress unused object warnings
template <typename... Args>
constexpr void YK_Unused(Args&&...)
{}

// Is this really needed?
#define YK_SAFE_DELETE(ptr) do { delete ptr; ptr = nullptr; } while(false)
#define YK_SAFE_DELETE_ARRAY(X) { delete[] X; X = nullptr; }

namespace YK_PlacementNew
{
    template <typename Type, typename... Parameters>
    [[nodiscard]] constexpr Type* New(Type* p_address, Parameters&&... p_parameters)
    {
        return std::construct_at<Type, Parameters...>(p_address, std::forward<Parameters>(p_parameters)...);
    }

    template <typename Type>
    constexpr void Destruct(Type& p_item)
    {
        std::destroy_at<Type>(&p_item);
    }
} // namespace YK_PlacementNew

// Chooses the smallest unsigned integer required to store a value
template <YK_SizeT Number>
struct YK_SmallestStorage
{
private:
    static constexpr auto GetType()
    {
        if constexpr (Number <= YK_NumericLimits<YK_U8>::Max)
        {
            return YK_TypeType<YK_U8>{};
        }
        else if constexpr (Number <= YK_NumericLimits<YK_U16>::Max)
        {
            return YK_TypeType<YK_U16>{};
        }
        else if constexpr (Number <= YK_NumericLimits<YK_U32>::Max)
        {
            return YK_TypeType<YK_U32>{};
        }
        else
        {
            return YK_TypeType<YK_U64>{};
        }
    }

public:
    using Type = typename decltype(GetType())::Type;
};
template <YK_SizeT Number>
using YK_SmallestStorage_T = typename YK_SmallestStorage<Number>::Type;