#include "PCH/PPhys_PCH.h"
#include "PP_PhysicsModule.h"

#include "YK/Debugging/YK_Assert.h"
#include "YK/ECS/Components/YK_TransformComponent.h"
#include "YK/IO/Logging/YK_Logger.h"
#include "YK/Libraries/Zen/Entity/Zen_EntityView.h"
#include "YK/Libraries/Zen/Zen_Garden.h"
#include "YK/Time/YK_Time.h"
#include "YK/Types/Math/YK_Integer.h"
#include "YK/Types/Math/YK_Quaternion.h"
#include "YK/Types/Math/YK_Vector.h"

#include "PP/ECS/PP_RigidBodyComponent.h"

#include <Jolt/Jolt.h>
#include "Jolt/Core/Factory.h"
#include "Jolt/Core/IssueReporting.h"
#include "Jolt/Core/Memory.h"
#include "Jolt/Math/Quat.h"
#include "Jolt/Math/Real.h"
#include "Jolt/Physics/Body/BodyID.h"
#include "Jolt/Physics/Body/BodyInterface.h"
#include "Jolt/Physics/PhysicsSettings.h"
#include "Jolt/RegisterTypes.h"

#include <cstdarg>
#include <cstdio>
#include <thread>

namespace PP_PhysicsModule_Private
{
#if !YAKU_RETAIL
    void LogJoltMessage(const char* p_format, ...)
    {
        va_list list;
        va_start(list, p_format);
        char buffer[1024];
        vsnprintf(buffer, sizeof(buffer), p_format, list);
        va_end(list);

        YK_LOG(buffer);
    }
#endif // !YAKU_RETAIL

#if defined(JPH_ENABLE_ASSERTS)
    // Callback for asserts, connect this to your own assert handler if you have one
    static bool LogJoltAssert(const char* p_expression, const char* p_message, const char* p_file, YK_U32 p_line)
    {
        YK_LOG_ERROR_PARAM("Jolt Assert Failed on line {} in {}\n{}\n\"{}\n", p_line, p_file, p_expression, p_message);
        return true;
    };
#endif // JPH_ENABLE_ASSERTS
} // namespace PP_PhysicsModule_Private

PP_PhysicsModule::PP_PhysicsModule(YK_SizeT p_allocatedMemory)
{
    YK_ASSERT(JPH::Factory::sInstance == nullptr,
              "Attempting to re-initialize the physics module! There should only be one per-instance of the game");

    JPH::RegisterDefaultAllocator();

#if !YAKU_RETAIL
    JPH::Trace = PP_PhysicsModule_Private::LogJoltMessage;
#endif // !YAKU_RETAIL
    JPH_IF_ENABLE_ASSERTS(JPH::AssertFailed = PP_PhysicsModule_Private::LogJoltAssert);

    JPH::Factory::sInstance = new JPH::Factory;
    JPH::RegisterTypes();

    m_tempAllocator.Construct(p_allocatedMemory);

    // TODO: Come back to this. This is straight from the Jolt hello world
    // I need to actually break down what it's doing and evaluate what's best for YakuEn
    m_jobSystem.Construct(JPH::cMaxPhysicsJobs,
                          JPH::cMaxPhysicsBarriers,
                          static_cast<int>(std::thread::hardware_concurrency()) - 1);

    // TODO: Move to better place. Configuration?
    constexpr YK_U32 MAX_BODIES = 1024;
    constexpr YK_U32 NUM_BODY_MUTEXES = 0;
    constexpr YK_U32 MAX_BODY_PAIRS = 1024;
    constexpr YK_U32 MAX_CONTACT_CONSTRAINTS = 1024;

    m_joltInstance.Init(MAX_BODIES,
                        NUM_BODY_MUTEXES,
                        MAX_BODY_PAIRS,
                        MAX_CONTACT_CONSTRAINTS,
                        m_broadPhaseLayerInterface,
                        m_objectVsBroadPhaseLayerFilter,
                        m_objectLayerPairFilter);

    s_instance = this;
}

PP_PhysicsModule::~PP_PhysicsModule()
{
    // TODO: Determine if theres any objects left alive and assert

    m_jobSystem.Destruct();
    m_tempAllocator.Destruct();

    JPH::UnregisterTypes();
    delete JPH::Factory::sInstance;
    JPH::Factory::sInstance = nullptr;

    s_instance = nullptr;
}

void PP_PhysicsModule::Update(Zen::Garden& p_entityGarden)
{
    float const deltaTime = YK_Time::PhysicsDeltaTime();
    m_joltInstance.Update(deltaTime, 1, &m_tempAllocator.Get(), &m_jobSystem.Get());

    // Resyncrhonize transforms with underlying physics positions for bodies
    JPH::BodyInterface const& bodyInterface = m_joltInstance.GetBodyInterface();

    Zen::EntityView<YK_TransformComponent, PP_RigidBodyComponent> entityView =
      p_entityGarden.ViewComponents<YK_TransformComponent, PP_RigidBodyComponent>();
    for (auto [transform, rigidBody] : entityView)
    {
        JPH::BodyID const id{ rigidBody.m_joltBodyID };
        if (bodyInterface.IsActive(id))
        {
            JPH::RVec3 position;
            JPH::Quat orientation;
            bodyInterface.GetPositionAndRotation(id, position, orientation);
            // TODO: Find a better way to convert between Jolt's and my types
            transform.m_orientation =
              YK_Quaternion(orientation.GetX(), orientation.GetY(), orientation.GetZ(), orientation.GetW());

            transform.m_position = YK_Vector3f(position.GetX(), position.GetY(), position.GetZ())
                                   - (transform.m_orientation * rigidBody.m_bodyOffset);
        }
    }
}