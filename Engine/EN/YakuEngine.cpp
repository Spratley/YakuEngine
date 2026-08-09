#include "PCH/YakuEngine_PCH.h"
#include "YakuEngine.h"

#include "CG/CG_RenderModule.h"

#include "YKC/Time/YKC_Time.h"

// Temp
#include "ECS/EN_TEST_BobbingComponent.h"

#include "CG/Camera/CG_Camera.h"
#include "CG/Renderable/CG_Renderable.h"
#include "CG/Resource/Mesh/CG_MeshFactory.h"
#include "CG/Resource/Texture/CG_TextureFactory.h"

#include "YKC/ECS/Components/YKC_TransformComponent.h"
#include "YKC/Math/YKC_MatrixMath.h"
#include "YKC/Types/Containers/YKC_PagedArray.h"

#include <cstdlib>
#include <ctime>

// Temp
Zen::Entity g_camera;
Zen::Entity g_lastHeart;

YK_Matrix44 g_viewMatrix;

CG_Mesh* g_heartMesh;
CG_Mesh* g_quadMesh;

CG_Texture* g_heartTexture;
CG_Texture* g_groundTexture;

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
#if YK_PLATFORM == YK_WINDOWS
    initData.m_mainWindowHandle = m_platformCore.GetMainDisplaySurface().GetNativeHandle();
#endif // YK_PLATFORM == YK_WINDOWS

    g_hidraCore.Init(initData);

    m_renderModule = new CG_RenderModule(m_platformCore.GetMainDisplaySurface()); // TODO: Don't do this

    // Temp
    g_camera = m_zenGarden.Spawn<YK_TransformComponent, CG_CameraComponent>({}, {});
    g_camera.GetComponent<YK_TransformComponent>()->m_position.y = 1.0f;

    g_heartMesh = CG_MeshFactory::LoadOBJ("J:/Harbourfront/Data/Models/HeartTest.obj");
    g_quadMesh = CG_MeshFactory::Quad();

    g_heartTexture = CG_TextureFactory::LoadPNG("J:/Harbourfront/Data/Textures/HeartTest.png");
    g_groundTexture = CG_TextureFactory::LoadPNG("J:/Harbourfront/Data/Textures/Prototype_Ground.png");

    std::srand(static_cast<unsigned int>(time(NULL)));
    auto GetRandomFloat = [](float p_max) {
        auto randomValue = std::rand() % 10000;
        return static_cast<float>(randomValue) / 10000.0f * p_max;
    };

    Zen::Entity groundPlane =
      m_zenGarden.Spawn<YK_TransformComponent, CG_MeshComponent, CG_RendererComponent>({}, {}, {});
    YK_TransformComponent* groundTransform = groundPlane.GetComponent<YK_TransformComponent>();
    constexpr float angle = 90 * (3.14159265f / 180.0f);
    groundTransform->m_orientation = YK_Quaternion(YK_Vector3f::Right(), angle);
    groundTransform->m_scale = YK_Vector3f(30.0f);

    groundPlane.GetComponent<CG_MeshComponent>()->m_mesh = g_quadMesh;
    groundPlane.GetComponent<CG_RendererComponent>()->m_texture = g_groundTexture;

    for (auto i : Zen::LoopUtils::CountTo(5))
    {
        YK_Unused(i);

        Zen::Entity bobber =
          m_zenGarden.Spawn<YK_TransformComponent, CG_MeshComponent, CG_RendererComponent, BobbingComponent>({},
                                                                                                             {},
                                                                                                             {},
                                                                                                             {});
        YK_TransformComponent* bobberTransform = bobber.GetComponent<YK_TransformComponent>();

        float x = GetRandomFloat(10.0f) - 5.0f;
        float y = GetRandomFloat(10.0f) - 5.0f;
        float z = GetRandomFloat(10.0f) - 5.0f;
        bobberTransform->m_position = YK_Vector3f(x, y, z);

        float bobOffset = GetRandomFloat(10.0f);
        bobber.GetComponent<BobbingComponent>()->m_phase = bobOffset;

        bobber.GetComponent<CG_MeshComponent>()->m_mesh = g_heartMesh;
        bobber.GetComponent<CG_RendererComponent>()->m_texture = g_heartTexture;

        g_lastHeart = bobber;
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
    if (HIDra::GetKey(HIDra::KEYCODE_S) || HIDra::GetButton(HIDra::BID_DPAD_SOUTH))
    {
        input.m_y = -1;
    }
    else if (HIDra::GetKey(HIDra::KEYCODE_W) || HIDra::GetButton(HIDra::BID_DPAD_NORTH))
    {
        input.m_y = 1;
    }

    if (HIDra::GetKey(HIDra::KEYCODE_A) || HIDra::GetButton(HIDra::BID_DPAD_WEST))
    {
        input.m_x = -1;
    }
    else if (HIDra::GetKey(HIDra::KEYCODE_D) || HIDra::GetButton(HIDra::BID_DPAD_EAST))
    {
        input.m_x = 1;
    }

    float raise = HIDra::GetButton(HIDra::BID_BUMPER_L) ? -1.0f : HIDra::GetButton(HIDra::BID_BUMPER_R) ? 1.0f : 0.0f;

    YK_TransformComponent* cameraTransform = g_camera.GetComponent<YK_TransformComponent>();

    // TODO: Converter function
    YK_Vector3f const flatForward = [&]() {
        YK_Vector3f result = cameraTransform->m_orientation * YK_Vector3f::Forward();
        result.y = 0.0f;
        return YK_Vector::GetNormalized(result);
    }();
    YK_Vector3f const right = YK_Vector::Cross(YK_Vector3f::Up(), flatForward);

    YK_Vector3f frameDelta = (right * input.m_x) + (flatForward * -input.m_y) + (YK_Vector3f(0.0f, raise, 0.0f));
    frameDelta *= YKC_Time::DeltaTime() * 2.0f;
    cameraTransform->m_position += frameDelta;
    cameraTransform->m_position += frameDelta;

    HIDra::Vec2f leftRight = HIDra::GetAxis2D(HIDra::GamepadAxisID::AID_STICK_R);
    YK_Vector3f newLookTarget = -flatForward + cameraTransform->m_position;
    newLookTarget += (right * leftRight.m_x) * YKC_Time::DeltaTime();
    cameraTransform->m_orientation = YK_Matrix::LookAt(cameraTransform->m_position, newLookTarget);

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