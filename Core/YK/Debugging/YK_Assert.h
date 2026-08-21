#pragma once

#include "YK/IO/Logging/YK_Logger.h"

#if !YAKU_RETAIL
static inline void YK_AssertImpl(bool p_condition, char const* p_message)
{
    if (!p_condition)
    {
        YK_LOG_ERROR(p_message);
#if _MSC_VER
        __debugbreak();
#endif // YK_PLATFORM != YK_WASM
        abort();
    }
}

#define YK_ASSERT(Condition, Message) YK_AssertImpl(Condition, Message)
#define YK_ASSERT_PARAM(Condition, Message, ...) YK_AssertImpl(Condition, std::format(Message, __VA_ARGS__).c_str())
#else
#define YK_ASSERT(Condition, Message)
#define YK_ASSERT_PARAM(Condition, Message, ...)
#endif // YAKU_RETAIL