#pragma once

#if !YAKU_RETAIL
#include <format>
#include <string_view>

#include "YK/Types/Math/YK_MathTypeFormatters.h"
#endif // !YAKU_RETAIL

// TODO: Log Improvements
// - Add logging service for platform independence
// - Make logging store and write messages to a file
// - Allow runtime analysis of logs from in engine GUI
// - Sort and hide runtime logs based on severity and location

#if !YAKU_RETAIL
#define YK_LOG(Message) YK_Logger::Log(YK_Logger::ErrorLevel::Log, std::format("{}", Message).c_str())
#define YK_LOG_PARAM(Message, ...) YK_Logger::Log(YK_Logger::ErrorLevel::Log, std::format(Message, __VA_ARGS__).c_str())
#else
#define YK_LOG(Message) YK_Unused(Message)
#define YK_LOG_PARAM(Message, ...) YK_Unused(Message, __VA_ARGS__)
#endif // !YAKU_RETAIL

#if !YAKU_RETAIL
#define YK_LOG_WARNING(Message) YK_Logger::Log(YK_Logger::ErrorLevel::Warning, std::format("{}", Message).c_str())
#define YK_LOG_WARNING_PARAM(Message, ...) YK_Logger::Log(YK_Logger::ErrorLevel::Warning, std::format(Message, __VA_ARGS__).c_str())
#else
#define YK_LOG_WARNING(Message) YK_Unused(Message)
#define YK_LOG_WARNING_PARAM(Message, ...) YK_Unused(Message, __VA_ARGS__)
#endif // !YAKU_RETAIL

#if !YAKU_RETAIL
#define YK_LOG_ERROR(Message) YK_Logger::Log(YK_Logger::ErrorLevel::Error, std::format("{}", Message).c_str())
#define YK_LOG_ERROR_PARAM(Message, ...) YK_Logger::Log(YK_Logger::ErrorLevel::Error, std::format(Message, __VA_ARGS__).c_str())
#else
#define YK_LOG_ERROR(Message) YK_Unused(Message)
#define YK_LOG_ERROR_PARAM(Message, ...) YK_Unused(Message, __VA_ARGS__)
#endif // !YAKU_RETAIL

#if !YAKU_RETAIL
class YK_Logger
{
public:
    enum class ErrorLevel
    {
        Log,
        Warning,
        Error
    };

    static void Log(ErrorLevel p_errorLevel, char const* message);
};
#endif // !YAKU_RETAIL