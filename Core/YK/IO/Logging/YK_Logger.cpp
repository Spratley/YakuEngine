#include "PCH/YakuCore_PCH.h"
#include "YK_Logger.h"

#if !YAKU_RETAIL
#include "YK/Platforms/YK_PlatformDefines.h"
#include "YK/Types/Math/YK_Integer.h"

namespace YK_Logger_Private
{
    static constexpr char const* s_errorLevelStrings[] = { "Log", "Warning", "Error" };
}

// TODO: Do a more formal split - I'm just doing this because it's one definition per platform
#if YK_PLATFORM == YK_WINDOWS
#include <print>

void YK_Logger::Log(ErrorLevel p_errorLevel, char const* p_message)
{
    std::println("{}: {}", YK_Logger_Private::s_errorLevelStrings[static_cast<YK_U32>(p_errorLevel)], p_message);
}

#elif YK_PLATFORM == YK_WASM
#include <emscripten/console.h>

void YK_Logger::Log(ErrorLevel p_errorLevel, char const* p_message)
{
    switch (p_errorLevel)
    {
        case ErrorLevel::Log:     emscripten_console_log(p_message); break;
        case ErrorLevel::Warning: emscripten_console_warn(p_message); break;
        case ErrorLevel::Error:   emscripten_console_error(p_message); break;
    }
}
#endif

#endif // !YAKU_RETAIL