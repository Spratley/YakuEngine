#include "PCH/YakuCore_PCH.h"
#include "YK_Time.h"

#include <chrono>

void YK_Time::OnFrameEnd()
{
    Clock::time_point const now = Clock::now();
    std::chrono::duration<float> const delta = now - s_lastFrameEnd;
    s_lastFrameEnd = now;
    s_deltaTime = delta.count();
}