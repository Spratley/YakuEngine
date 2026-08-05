#include "PCH/YakuCore_PCH.h"
#include "YKC_Time.h"

void YKC_Time::OnFrameEnd()
{
    Clock::time_point now = Clock::now();
    std::chrono::duration<float> delta = now - s_lastFrameEnd;
    s_lastFrameEnd = now;
    s_deltaTime = delta.count();
}