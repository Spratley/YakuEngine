#pragma once

#include "YK/Types/Other/YK_Callback.h"

#include <concepts>

class YK_Core;

template <typename EngineSystemType>
concept YK_EngineSystemType = requires(YK_Core& p_engineCore) {
    { EngineSystemType::Init(p_engineCore) };
    { EngineSystemType::Update(p_engineCore) };
    { EngineSystemType::ShutDown(p_engineCore) };
};

class YK_EngineSystemManager
{
public:
    template <YK_EngineSystemType SystemType>
    void RegisterSystem()
    {
        m_initCallback.Attach<SystemType::Init>();
        m_updateCallback.Attach<SystemType::Update>();
        m_shutDownCallback.Attach<SystemType::ShutDown>();
    }

    void InitSystems(YK_Core& p_engine) const { m_initCallback(p_engine); }
    void UpdateSystems(YK_Core& p_engine) const { m_updateCallback(p_engine); }
    void ShutDownSystems(YK_Core& p_engine) const { m_shutDownCallback(p_engine); }

private:
    YK_Callback<YK_Core&> m_initCallback;
    YK_Callback<YK_Core&> m_updateCallback;
    YK_Callback<YK_Core&> m_shutDownCallback;
};