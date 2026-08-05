#pragma once

#include "YKC/IO/Logging/YKC_Logger.h"

#if YAKU_RETAIL
#define YK_ASSERT(Condition, Message)
#else
static inline void YK_AssertImpl(bool p_condition, const char* p_message)
{
    if (!p_condition)
    {
        YK_LOG_ERROR(p_message);
#if YK_PLATFORM != YK_WASM
        __debugbreak();
#endif // YK_PLATFORM != YK_WASM
        abort();
    }
}

#define YK_ASSERT(Condition, Message) YK_AssertImpl(Condition, Message)
#endif // YAKU_RETAIL