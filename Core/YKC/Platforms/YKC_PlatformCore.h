#pragma once

#include "YKC/Interfaces/YKC_Singleton.h"
#include "YKC/IO/Display/YKC_DisplaySurface.h"

// Does this need to be a singleton? Can we just rely on requesting access via the main engine object?
class YKC_PlatformCore : public YKC_Singleton<YKC_PlatformCore>
{
public:
    YKC_PlatformCore()
    {
        if (s_instance)
        {
            YK_LOG_ERROR("Re-creating platform core instance! Something is VERY wrong!");
            return;
        }
        s_instance = this;
    }

    bool Init();
    void ShutDown();
    void LaunchCoreLoop(void (*p_coreLoop)(void*), void* p_contextData) const; // TODO: Strongly define callback type

    // TODO: Find a better home for this?
    void OnFrameStart() const;

    YK_DisplaySurface& GetMainDisplaySurface() { return m_displaySurface; }
    YK_DisplaySurface const& GetMainDisplaySurface() const { return m_displaySurface; }

private:
    YK_DisplaySurface m_displaySurface;
};