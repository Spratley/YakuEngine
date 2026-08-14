#pragma once

#include "YK/Types/Pointers/YK_UniquePointer.h"

class YakuEngine;
class CG_RenderModule;

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

    HIDra::Core& GetHIDra() { return *m_hidraCore; }
    HIDra::Core const& GetHIDra() const { return *m_hidraCore; }

private:
    YK_UniquePointer<CG_RenderModule> m_renderModule;
    YK_UniquePointer<HIDra::Core> m_hidraCore;
};