#pragma once

#include "YK/Types/Pointers/YK_UniquePointer.h"

class YakuEngine;
class CG_RenderModule;
class PP_PhysicsModule;

namespace HIDra
{
    class Core;
}

class EN_ModuleRegistry
{
public:
    EN_ModuleRegistry();
    ~EN_ModuleRegistry();

    bool InitializeModules(YakuEngine& p_engine);

    CG_RenderModule& GetRenderModule() { return *m_renderModule; }
    CG_RenderModule const& GetRenderModule() const { return *m_renderModule; }

    PP_PhysicsModule& GetPhysicsModule() { return *m_physicsModule; }
    PP_PhysicsModule const& GetPhysicsModule() const { return *m_physicsModule; }

    HIDra::Core& GetHIDra() { return *m_hidraCore; }
    HIDra::Core const& GetHIDra() const { return *m_hidraCore; }

private:
    YK_UniquePointer<CG_RenderModule> m_renderModule;
    YK_UniquePointer<PP_PhysicsModule> m_physicsModule;
    YK_UniquePointer<HIDra::Core> m_hidraCore;
};