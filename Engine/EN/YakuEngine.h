#pragma once

#include "EN/Modules/EN_ModuleRegistry.h"

#include "YK/Core/YK_Core.h"

class YakuEngine : public YK_Core
{
public:
    template <typename ComponentTypes, typename SystemTypes>
    YakuEngine(ComponentTypes p_componentTypes, SystemTypes p_systemTypes)
        : YK_Core(p_componentTypes, p_systemTypes)
    {}
    ~YakuEngine() = default;

    void Run();

private:
    bool Init();
    void ShutDown();
    void EngineLoop();

    void BeginFrame();
    void EndFrame();

    void RegisterAssetTypes();

private:
    EN_ModuleRegistry m_modules;
};