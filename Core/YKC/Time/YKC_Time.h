#pragma once

#include <chrono>

class YKC_Time
{
private:
    using Clock = std::chrono::steady_clock;

public:
    static float DeltaTime() { return s_deltaTime; }

    static void OnFrameEnd() 
    {
        Clock::time_point now = Clock::now();
        std::chrono::duration<float> delta = now - s_lastFrameEnd;
        s_lastFrameEnd = now;
        s_deltaTime = delta.count();
    }

private:
    static inline Clock::time_point s_lastFrameEnd = Clock::now();
    static inline float s_deltaTime = 0.00001f;
};