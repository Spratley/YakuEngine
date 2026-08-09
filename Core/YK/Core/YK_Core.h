#pragma once

#include "YK/IO/Display/YK_DisplaySurface.h"
#include "YK/Libraries/Zen/Zen_Garden.h"

class YK_Core
{
public:
protected:
    template <typename ComponentTypes, typename SystemTypes>
    constexpr YK_Core(ComponentTypes p_componentTypes, SystemTypes p_systemTypes)
        : m_displaySurface()
        , m_zenGarden(p_componentTypes, p_systemTypes)
    {
        m_zenGarden.Initialize(1024 * 512);
    }

    bool Init();
    void ShutDown();

    using CoreLoopSignature = void (*)(void*);
    void LaunchCoreLoop(CoreLoopSignature p_coreLoop, void* p_contextData) const;

    YK_DisplaySurface& GetMainDisplaySurface() { return m_displaySurface; }
    YK_DisplaySurface const& GetMainDisplaySurface() const { return m_displaySurface; }

    void OnFrameStart() const;

protected:
    YK_DisplaySurface m_displaySurface;
    Zen::Garden m_zenGarden;
};