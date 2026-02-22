#pragma once

#if !YAKU_RETAIL
#include <print>
#include <cstdio>
#endif // !YAKU_RETAIL

// TODO: Log Improvements
// - Add logging service for platform independence
// - Make logging store and write messages to a file
// - Allow runtime analysis of logs from in engine GUI
// - Sort and hide runtime logs based on severity and location

#if !YAKU_RETAIL
#define YK_LOG(Message) std::println("{}", Message)
#define YK_LOG_PARAM(Message, Param) std::println(Message, Param)
#else
#define YK_LOG(Message) YK_UNUSED(Message)
#define YK_LOG_PARAM(Message, Param) YK_UNUSED(Message); YK_UNUSED(Param)
#endif // !YAKU_RETAIL

#if !YAKU_RETAIL
#define YK_LOG_ERROR(Message) std::println(stderr, "Error: {}", Message)
#define YK_LOG_ERROR_PARAM(Message, Param) std::println(stderr, "Error: " Message, Param)
#else
#define YK_LOG_ERROR(Message) YK_UNUSED(Message)
#define YK_LOG_ERROR_PARAM(Message, Param) YK_UNUSED(Message); YK_UNUSED(Param)
#endif // !YAKU_RETAIL

#if !YAKU_RETAIL
class YK_Logger
{
    // Get logger service...
};
#endif // !YAKU_RETAIL