#pragma once

#if YAKU_RETAIL
#define YK_ASSERT(Condition, Message)
#else
static inline void YK_AssertImpl(bool p_condition, const char* p_message)
{
    if (!p_condition)
    {
        YK_LOG_ERROR(p_message);
#if !YK_WEB_ASSEMBLY
        __debugbreak();
#endif // YK_WEB_ASSEMBLY
    }
}

#define YK_ASSERT(Condition, Message) YK_AssertImpl(Condition, Message)
#endif // YAKU_RETAIL