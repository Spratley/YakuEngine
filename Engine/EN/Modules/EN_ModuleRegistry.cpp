#include "PCH/YakuEngine_PCH.h"
#include "EN_ModuleRegistry.h"

#include "EN/Libraries/HIDra/Core/HIDraCore.h"
#include "EN/YakuEngine.h"

#include "CG/CG_RenderModule.h"

#include "YK/IO/Display/YK_DisplaySurface.h"
#include "YK/Types/Pointers/YK_UniquePointer.h"

EN_ModuleRegistry::EN_ModuleRegistry() = default;
EN_ModuleRegistry::~EN_ModuleRegistry() = default;

bool EN_ModuleRegistry::InitializeModules(YakuEngine& p_engine)
{
    YK_DisplaySurface& mainDisplaySurface = p_engine.GetMainDisplaySurface();

    m_renderModule = YK_UniquePointer<CG_RenderModule>::Make(mainDisplaySurface);

    // HIDra isn't really a module, but this is convenient
    HIDra::Core_PlatformInitData initData;
#if YK_PLATFORM == YK_WINDOWS
    initData.m_mainWindowHandle = mainDisplaySurface.GetNativeHandle();
#endif // YK_PLATFORM == YK_WINDOWS
    m_hidraCore = YK_UniquePointer<HIDra::Core>::Make();
    if (!m_hidraCore->Init(initData))
    {
        YK_LOG_ERROR("Failed to initialize HIDra!");
        return false;
    }

    return true;
}