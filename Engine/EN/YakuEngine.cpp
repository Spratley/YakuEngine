#include "PCH/YakuEngine_PCH.h"
#include "YakuEngine.h"

#include "EN/Libraries/HIDra/HIDra.h"

#include "CG/CG_RenderModule.h"
#include "CG/Camera/CG_CameraComponent.h"
#include "CG/Resource/CG_ResourceRegistry.h"

#include "YK/Core/YK_Core.h"
#include "YK/Libraries/Zen/Entity/Zen_EntityView.h"
#include "YK/Libraries/Zen/Zen_Garden.h"
#include "YK/Time/YK_Time.h"

namespace YakuEngine_Private
{
    // Temporary until I need to find a better solution
    // This WILL return invalid data if there's no camera in the garden
    CG_CameraComponent const& FindCamera(Zen::Garden const& p_entityGarden)
    {
        Zen::EntityView<CG_CameraComponent> cameras = p_entityGarden.ViewComponents<CG_CameraComponent>();
        return *cameras.begin();
    }
} // namespace YakuEngine_Private

void YakuEngine::Run()
{
    // Init Engine
    if (!Init())
    {
        ShutDown();
        return;
    }

    // TODO: Come back to this. WebAssembly requires that we don't manually loop,
    // and so this ugly back and forth needs to be here to support it.
    // However, it takes up a callstack frame that wouldn't need to be if we were directly looping ourselves
    // Since most platforms DON'T have an asynchronous loop,
    // we can probably figure out a more elegant way to get that one single precious stack frame!
    LaunchCoreLoop([](void* p_context) { static_cast<YakuEngine*>(p_context)->EngineLoop(); }, this);

    ShutDown();
}

bool YakuEngine::Init()
{
    if (!YK_Core::Init())
    {
        return false;
    }
    RegisterAssetTypes();

    m_modules.InitializeModules(*this);
    m_engineSystemManager.InitSystems(*this);

    return true;
}

void YakuEngine::ShutDown() { YK_Core::ShutDown(); }

void YakuEngine::EngineLoop()
{
    // Run Game Loop
    BeginFrame();
    m_engineSystemManager.UpdateSystems(*this);
    m_zenGarden.Tick();
    m_modules.GetRenderModule().Render(YakuEngine_Private::FindCamera(m_zenGarden), m_zenGarden);
    EndFrame();
}

void YakuEngine::BeginFrame() { OnFrameStart(); }

void YakuEngine::EndFrame()
{
    HIDra::Flush();
    YK_Time::OnFrameEnd();
}

void YakuEngine::RegisterAssetTypes() { CG_ResourceRegistry::RegisterAssetTypes(m_assetManager); }