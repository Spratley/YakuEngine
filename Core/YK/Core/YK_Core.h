#pragma once

#include "YK/EngineSystem/YK_EngineSystemManager.h"
#include "YK/IO/Asset/YK_AssetManager.h"
#include "YK/IO/Display/YK_DisplaySurface.h"
#include "YK/Libraries/Zen/Zen_Garden.h"

class YK_Core
{
public:
    static YK_Core& GetEngine() { return *s_engine; }

protected:
    static inline YK_Core* s_engine = nullptr;

public:
    YK_DisplaySurface& GetMainDisplaySurface() { return m_displaySurface; }
    YK_DisplaySurface const& GetMainDisplaySurface() const { return m_displaySurface; }

    YK_AssetManager& GetAssetManager() { return m_assetManager; }
    YK_EngineSystemManager& GetEngineSystemManager() { return m_engineSystemManager; }

    Zen::Garden& GetZenGarden() { return m_zenGarden; }

protected:
    template <typename ComponentTypes, typename SystemTypes>
    constexpr YK_Core(ComponentTypes p_componentTypes, SystemTypes p_systemTypes)
        : m_displaySurface()
        , m_zenGarden(p_componentTypes, p_systemTypes)
    {
        m_zenGarden.Initialize(1024 * 512);

        YK_ASSERT(!s_engine, "Attempting to create two YK_Core objects in a single program!!!");
        s_engine = this;
    }

    bool Init();
    void ShutDown();

    using CoreLoopSignature = void (*)(void*);
    void LaunchCoreLoop(CoreLoopSignature p_coreLoop, void* p_contextData) const;

    void OnFrameStart() const;

protected:
    YK_DisplaySurface m_displaySurface;
    YK_AssetManager m_assetManager;
    YK_EngineSystemManager m_engineSystemManager;
    Zen::Garden m_zenGarden;
};