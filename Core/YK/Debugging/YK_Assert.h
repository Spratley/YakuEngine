#pragma once

#include "YK/IO/Logging/YK_Logger.h"

#if !YAKU_RETAIL
static constexpr inline void YK_AssertImpl(bool p_condition, char const* p_message)
{
    if (!p_condition)
    {
        if consteval
        {
            // Constexpr Assert
            static_cast<void const*>(p_message);
            throw "Failed Constexpr Assertion";
        }
        else
        {
            // Runtime Assert
            YK_LOG_ERROR(p_message);
#if _MSC_VER
            __debugbreak();
#endif // _MSC_VER
            abort();
        }
    }
}

#define YK_ASSERT(Condition, Message) YK_AssertImpl(Condition, Message)
#define YK_ASSERT_PARAM(Condition, Message, ...) YK_AssertImpl(Condition, std::format(Message, __VA_ARGS__).c_str())
#else
#define YK_ASSERT(Condition, Message)
#define YK_ASSERT_PARAM(Condition, Message, ...)
#endif // YAKU_RETAIL