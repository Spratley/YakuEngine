#include "PCH/YakuEngine_PCH.h"
#include "YakuEngine.h"

#include "CG/CG_RenderModule.h"

#include "YK/Time/YK_Time.h"

// Temp
#include "ECS/EN_TEST_BobbingComponent.h"

#include "CG/Camera/CG_CameraComponent.h"
#include "CG/Renderable/CG_Renderable.h"
#include "CG/Resource/Material/CG_Material.h"
#include "CG/Resource/Material/CG_MaterialLoader.h"
#include "CG/Resource/Mesh/CG_Mesh.h"
#include "CG/Resource/Mesh/CG_MeshFactory.h"
#include "CG/Resource/Shader/CG_Shader.h"
#include "CG/Resource/Shader/CG_ShaderLoader.h"
#include "CG/Resource/Texture/CG_Texture.h"
#include "CG/Resource/Texture/CG_TextureFactory.h"

#include "YK/ECS/Components/YK_TransformComponent.h"
#include "YK/IO/File/YK_FilePath.h"
#include "YK/Math/YK_MatrixMath.h"

#include <cstdlib>
#include <ctime>
#include <tuple>

// Temp
Zen::Entity g_camera;
Zen::Entity g_lastHeart;

YK_Matrix44 g_viewMatrix;

CG_Mesh* g_quadMesh;

namespace YakuEngine_Private
{
    // Temporary until I need to find a better solution
    // This WILL return invalid data if there's no camera in the garden
    CG_CameraComponent const& FindCamera(Zen::Garden const& p_entityGarden)
    {
        Zen::EntityView<CG_CameraComponent> cameras = p_entityGarden.ViewComponents<CG_CameraComponent>();
        return std::get<CG_CameraComponent>(*cameras.begin());
    }
} // namespace YakuEngine_Private

bool YakuEngine::Init()
{
    if (!YK_Core::Init())
    {
        return false;
    }

    RegisterAssetTypes();

    m_modules.InitializeModules(*this);

    // Temp
    g_camera = m_zenGarden.Spawn<YK_TransformComponent, CG_CameraComponent>();
    g_camera.GetComponent<YK_TransformComponent>()->m_position.y = 1.0f;
    CG_CameraComponent* camera = g_camera.GetComponent<CG_CameraComponent>();
    camera->m_fov = 60.0f;
    camera->m_nearPlane = 0.1f;
    camera->m_farPlane = 100.0f;

    CG_Mesh const& heartMesh = m_assetManager.GetAsset<CG_Mesh>(YK_FilePath("Models/HeartTest.obj"));
    g_quadMesh = CG_MeshFactory::Quad();

    CG_Material const& heartMaterial = m_assetManager.GetAsset<CG_Material>(YK_FilePath("Materials/Main.YKM"));
    CG_Material const& groundMaterial = m_assetManager.GetAsset<CG_Material>(YK_FilePath("Materials/Ground.YKM"));

    std::srand(static_cast<unsigned int>(time(NULL)));
    auto GetRandomFloat = [](float p_max) {
        auto randomValue = std::rand() % 10000;
        return static_cast<float>(randomValue) / 10000.0f * p_max;
    };

    Zen::Entity groundPlane = m_zenGarden.Spawn<YK_TransformComponent, CG_MeshComponent, CG_RendererComponent>();
    YK_TransformComponent* groundTransform = groundPlane.GetComponent<YK_TransformComponent>();
    constexpr float angle = 90 * (3.14159265f / 180.0f);
    groundTransform->m_orientation = YK_Quaternion(YK_Vector3f::Right(), angle);
    groundTransform->m_scale = YK_Vector3f(30.0f);

    groundPlane.GetComponent<CG_MeshComponent>()->m_mesh = g_quadMesh;
    groundPlane.GetComponent<CG_RendererComponent>()->m_material = &groundMaterial;

    for (auto i : Zen::LoopUtils::CountTo(5))
    {
        YK_Unused(i);

        Zen::Entity bobber =
          m_zenGarden.Spawn<YK_TransformComponent, CG_MeshComponent, CG_RendererComponent, BobbingComponent>();
        YK_TransformComponent* bobberTransform = bobber.GetComponent<YK_TransformComponent>();

        float x = GetRandomFloat(10.0f) - 5.0f;
        float y = GetRandomFloat(10.0f) - 5.0f;
        float z = GetRandomFloat(10.0f) - 5.0f;
        bobberTransform->m_position = YK_Vector3f(x, y, z);

        float bobOffset = GetRandomFloat(10.0f);
        bobber.GetComponent<BobbingComponent>()->m_phase = bobOffset;

        bobber.GetComponent<CG_MeshComponent>()->m_mesh = &heartMesh;
        bobber.GetComponent<CG_RendererComponent>()->m_material = &heartMaterial;

        g_lastHeart = bobber;
    }

    return true;
}

void YakuEngine::ShutDown() { YK_Core::ShutDown(); }

void YakuEngine::EngineLoop()
{
    // Run Game Loop
    BeginFrame();

    HIDra::Vec2f input /*= HIDra::GetAxis2D(HIDra::GamepadAxisID::AID_STICK_L)*/ = { 0.0f, 0.0f };
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

    frameDelta *= YK_Time::DeltaTime() * 2.0f;
    cameraTransform->m_position += frameDelta;

    HIDra::Vec2f leftRight = HIDra::GetAxis2D(HIDra::GamepadAxisID::AID_STICK_R);
    YK_Vector3f newLookTarget = -flatForward + cameraTransform->m_position;
    newLookTarget += (right * leftRight.m_x) * YK_Time::DeltaTime();
    YK_Quaternion newOrientation = YK_Matrix::LookAt(cameraTransform->m_position, newLookTarget);

    cameraTransform->m_orientation = newOrientation;

    m_zenGarden.Tick();

    CG_CameraComponent* cameraComponent = g_camera.GetComponent<CG_CameraComponent>();
    m_modules.GetRenderModule().Render(*cameraComponent, m_zenGarden);

    EndFrame();
}

void YakuEngine::BeginFrame() { OnFrameStart(); }

void YakuEngine::EndFrame()
{
    HIDra::Flush();
    YK_Time::OnFrameEnd();
}

void YakuEngine::RegisterAssetTypes()
{
    m_assetManager.RegisterType<CG_Mesh, CG_MeshLoader>();
    m_assetManager.RegisterType<CG_Shader, CG_ShaderLoader>();
    m_assetManager.RegisterType<CG_Texture, CG_TextureLoader>();
    m_assetManager.RegisterType<CG_Material, CG_MaterialLoader>();
}