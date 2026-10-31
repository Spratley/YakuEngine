#pragma once

#if !YAKU_RETAIL
#include <format>
#include <string_view>

#include "YK/Types/Math/YK_MathTypeFormatters.h"
#include "YK/Types/Traits/YK_TypeTraits.h"

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

    static void Log(ErrorLevel p_errorLevel, char const* p_message);

    template <typename Value>
    static std::string Format(Value&& p_value)
    { return std::format("{}", p_value); }

    template <typename... Arguments>
    static std::string Format(std::format_string<Arguments...> p_formatString, Arguments&&... p_arguments)
    { return std::format(p_formatString, std::forward<Arguments>(p_arguments)...); }
};
#endif // !YAKU_RETAIL

#if !YAKU_RETAIL
#define YK_LOG(...) YK_Logger::Log(YK_Logger::ErrorLevel::Log, YK_Logger::Format(__VA_ARGS__).c_str())
#define YK_LOG_WARNING(...) YK_Logger::Log(YK_Logger::ErrorLevel::Warning, YK_Logger::Format(__VA_ARGS__).c_str())
#define YK_LOG_ERROR(...) YK_Logger::Log(YK_Logger::ErrorLevel::Error, YK_Logger::Format(__VA_ARGS__).c_str())
#else
#define YK_LOG(...) YK_Unused(__VA_ARGS__)
#define YK_LOG_WARNING(...) YK_Unused(__VA_ARGS__)
#define YK_LOG_ERROR(...) YK_Unused(__VA_ARGS__)
#endif // !YAKU_RETAIL