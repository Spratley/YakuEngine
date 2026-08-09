#pragma once

#include "YK/IO/Logging/YK_Logger.h"

#if !YAKU_RETAIL
static inline void YK_AssertImpl(bool p_condition, const char* p_message)
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
#else
#define YK_ASSERT(Condition, Message)
#endif // YAKU_RETAIL