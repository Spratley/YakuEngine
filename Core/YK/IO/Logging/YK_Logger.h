#pragma once

#if !YAKU_RETAIL
#include <format>
#include <string_view>

#include "YK/Types/Math/YK_MathTypeFormatters.h"

// TODO: Log Improvements
// - Add logging service for platform independence
// - Make logging store and write messages to a file
// - Allow runtime analysis of logs from in engine GUI
// - Sort and hide runtime logs based on severity and location

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

#if !YAKU_RETAIL
#define YK_LOG(...) YK_Logger::Log(YK_Logger::ErrorLevel::Log, std::format(__VA_ARGS__).c_str())
#define YK_LOG_WARNING(...) YK_Logger::Log(YK_Logger::ErrorLevel::Warning, std::format(__VA_ARGS__).c_str())
#define YK_LOG_ERROR(...) YK_Logger::Log(YK_Logger::ErrorLevel::Error, std::format(__VA_ARGS__).c_str())
#else
#define YK_LOG(...) YK_Unused(__VA_ARGS__)
#define YK_LOG_WARNING(...) YK_Unused(__VA_ARGS__)
#define YK_LOG_ERROR(...) YK_Unused(__VA_ARGS__)
#endif // !YAKU_RETAIL