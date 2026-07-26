#include "PCH/YakuEngine_PCH.h"
#include "YakuEngine.h"

#include "CG/CG_RenderModule.h"

#include "YKC/Time/YKC_Time.h"

// Temp
Zen::Entity g_entity;

// Temp 2x
HIDra::Core g_hidraCore;

bool YakuEngine::Init()
{
    if (!m_platformCore.Init())
    {
        return false;
    }

    // TODO: Make this platform agnostic
    HIDra::Core_PlatformInitData initData;
#if YK_WINDOWS
    initData.m_mainWindowHandle = m_platformCore.GetMainDisplaySurface().GetNativeHandle();
#endif

    g_hidraCore.Init(initData);

    m_zenGarden.Initialize(1024);

    // Also make THIS platform agnostic
    m_renderModule =
      new CG_RenderModule(m_platformCore.GetMainDisplaySurface().GetContents().m_glfwWindow); // TODO: Don't do this

    // Temp
    g_entity = m_zenGarden.Spawn<TransformComponent>({});

    TransformComponent* modelTransform = g_entity.GetComponent<TransformComponent>();
    YK_Matrix::Translate(modelTransform->m_transform, YK_Vector3f(0.0f, -0.5f, -1.0f));

    Zen::Entity entityTwo = m_zenGarden.Spawn<TransformComponent>({});
    TransformComponent* e2T = entityTwo.GetComponent<TransformComponent>();
    YK_Matrix::Translate(e2T->m_transform, YK_Vector3f(1.0f, -0.5f, -3.0f));

    Zen::Entity entityThree = m_zenGarden.Spawn<TransformComponent>({});
    TransformComponent* e3 = entityThree.GetComponent<TransformComponent>();
    YK_Matrix::Translate(e3->m_transform, YK_Vector3f(-1.0f, -0.5f, -2.0f));

    return true;
}

void YakuEngine::ShutDown()
{
    YK_SAFE_DELETE(m_renderModule);

    m_platformCore.ShutDown();
}

void YakuEngine::EngineLoop()
{
    // Run Game Loop
    BeginFrame();
    m_zenGarden.Tick();

    HIDra::Vec2f input = HIDra::GetAxis2D(HIDra::GamepadAxisID::AID_STICK_L);
    if (HIDra::GetKey(HIDra::KEYCODE_S))
    {
        input.m_y = -1;
    }
    else if (HIDra::GetKey(HIDra::KEYCODE_W))
    {
        input.m_y = 1;
    }

    // TODO: Converter function
    YK_Vector3f frameDelta(input.m_x, 0.0f, input.m_y);
    frameDelta *= YKC_Time::DeltaTime();

    TransformComponent* modelTransform = g_entity.GetComponent<TransformComponent>();
    YK_Matrix44& modelMatrix = modelTransform->m_transform;
    YK_Matrix::Translate(modelMatrix, frameDelta);

    m_renderModule->Render(m_zenGarden);

    EndFrame();
}

void YakuEngine::BeginFrame() { m_platformCore.OnFrameStart(); }

void YakuEngine::EndFrame()
{
    HIDra::Flush();
    YKC_Time::OnFrameEnd();
}