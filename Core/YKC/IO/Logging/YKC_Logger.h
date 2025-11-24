#pragma once

#if !YAKU_RETAIL
    #include <iostream>
#endif // !YAKU_RETAIL

// TODO: Log Improvements
// - Add logging service for platform independence
// - Make logging store and write messages to a file
// - Allow runtime analysis of logs from in engine GUI
// - Sort and hide runtime logs based on severity and location

#if !YAKU_RETAIL
    #define YK_LOG(Message) std::cout << Message << std::endl;
#else
    #define YK_LOG(Message) YK_UNUSED(Message)
#endif // !YAKU_RETAIL

#if !YAKU_RETAIL
    #define YK_LOG_ERROR(Message) std::cerr << Message << std::endl;
#else
    #define YK_LOG_ERROR(Message) YK_UNUSED(Message);
#endif // !YAKU_RETAIL

#if !YAKU_RETAIL
class YK_Logger
{
    // Get logger service...
};
#endif // !YAKU_RETAIL