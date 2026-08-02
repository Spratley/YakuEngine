#include "PCH/YakuEngine_PCH.h"
#include "YakuEngine.h"

#include "CG/CG_RenderModule.h"

#include "YKC/Time/YKC_Time.h"

// Temp
#include "ECS/YK_TEST_BobbingComponent.h"

#include "CG/Camera/CG_Camera.h"

#include <cstdlib>
#include <ctime>

// Temp
Zen::Entity g_camera;

YK_Matrix44 g_viewMatrix;

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

    // Also make THIS platform agnostic
    m_renderModule =
      new CG_RenderModule(m_platformCore.GetMainDisplaySurface().GetContents().m_glfwWindow); // TODO: Don't do this

    // Temp
    g_camera = m_zenGarden.Spawn<TransformComponent, CG_CameraComponent>({}, {});


    std::srand(static_cast<unsigned int>(time(NULL)));
    auto GetRandomFloat = [](float p_max) {
        auto randomValue = std::rand() % 10000;
        return static_cast<float>(randomValue) / 10000.0f * p_max;
    };

    for (auto i : Zen::LoopUtils::CountTo(5))
    {
        YK_Unused(i);

        Zen::Entity bobber = m_zenGarden.Spawn<TransformComponent, RenderableComponent, BobbingComponent>({}, {}, {});
        TransformComponent* bobberTransform = bobber.GetComponent<TransformComponent>();

        float x = GetRandomFloat(10.0f) - 5.0f;
        float y = GetRandomFloat(10.0f) - 5.0f;
        float z = GetRandomFloat(10.0f) - 5.0f;
        YK_Matrix::Translate(bobberTransform->m_transform, YK_Vector3f(x, y, z));

        float bobOffset = GetRandomFloat(10.0f);
        bobber.GetComponent<BobbingComponent>()->m_phase = bobOffset;
    }

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

    HIDra::Vec2f input = HIDra::GetAxis2D(HIDra::GamepadAxisID::AID_STICK_L);
    if (HIDra::GetKey(HIDra::KEYCODE_S))
    {
        input.m_y = -1;
    }
    else if (HIDra::GetKey(HIDra::KEYCODE_W))
    {
        input.m_y = 1;
    }

    if (HIDra::GetKey(HIDra::KEYCODE_A))
    {
        input.m_x = -1;
    }
    else if (HIDra::GetKey(HIDra::KEYCODE_D))
    {
        input.m_x = 1;
    }

    // TODO: Converter function
    YK_Vector3f frameDelta(input.m_x, 0.0f, -input.m_y);
    frameDelta *= YKC_Time::DeltaTime();

    TransformComponent* cameraTransform = g_camera.GetComponent<TransformComponent>();
    YK_Matrix44& cameraWorldMatrix = cameraTransform->m_transform;
    YK_Matrix::Translate(cameraWorldMatrix, frameDelta);

    m_zenGarden.Tick();

    CG_CameraComponent* cameraComponent = g_camera.GetComponent<CG_CameraComponent>();
    YK_Matrix44 const& viewMatrix = cameraComponent->m_viewMatrix;

    m_renderModule->Render(viewMatrix, m_zenGarden);

    EndFrame();
}

void YakuEngine::BeginFrame() { m_platformCore.OnFrameStart(); }

void YakuEngine::EndFrame()
{
    HIDra::Flush();
    YKC_Time::OnFrameEnd();
}