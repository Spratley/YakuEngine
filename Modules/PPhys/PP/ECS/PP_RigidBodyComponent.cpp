#include "PCH/PPhys_PCH.h"
#include "PP_RigidBodyComponent.h"

#include "YK/ECS/Components/YK_TransformComponent.h"
#include "YK/Math/YK_NumericLimits.h"
#include "YK/Types/Math/YK_Integer.h"

#include "PP/JoltConfig/PP_JoltLayers.h"
#include "PP/PP_PhysicsModule.h"

#include <Jolt/Jolt.h>
#include "Jolt/Math/Quat.h"
#include "Jolt/Math/Real.h"
#include "Jolt/Physics/Body/BodyCreationSettings.h"
#include "Jolt/Physics/Body/BodyID.h"
#include "Jolt/Physics/Body/BodyInterface.h"
#include "Jolt/Physics/Body/MotionType.h"
#include "Jolt/Physics/Collision/Shape/Shape.h"
#include "Jolt/Physics/EActivation.h"

PP_RigidBodyComponent::PP_RigidBodyComponent(JPH::Shape* p_shape,
                                             YK_TransformComponent const& p_transform,
                                             PP_BodyType p_bodyType,
                                             bool p_active)
{
    PP_JoltLayers::Layers const layer =
      (p_bodyType == PP_BodyType::Static) ? PP_JoltLayers::STATIC : PP_JoltLayers::DYNAMIC;

    JPH::BodyCreationSettings bodySettings(
      p_shape,
      JPH::RVec3(p_transform.m_position.x, p_transform.m_position.y, p_transform.m_position.z),
      JPH::Quat(p_transform.m_orientation.x,
                p_transform.m_orientation.y,
                p_transform.m_orientation.z,
                p_transform.m_orientation.w),
      static_cast<JPH::EMotionType>(p_bodyType),
      layer);

    PP_PhysicsModule& physicsInstance = PP_PhysicsModule::GetPhysicsInstance();
    JPH::BodyInterface& bodyInterface = physicsInstance.GetJoltInstance().GetBodyInterface();
    JPH::BodyID body =
      bodyInterface.CreateAndAddBody(bodySettings,
                                     p_active ? JPH::EActivation::Activate : JPH::EActivation::DontActivate);
    m_joltBodyID = body.GetIndexAndSequenceNumber();
}

PP_RigidBodyComponent::~PP_RigidBodyComponent()
{
    JPH::BodyID joltID{ m_joltBodyID };
    if (joltID.IsInvalid())
    {
        return;
    }

    PP_PhysicsModule& physicsInstance = PP_PhysicsModule::GetPhysicsInstance();
    JPH::BodyInterface& bodyInterface = physicsInstance.GetJoltInstance().GetBodyInterface();
    bodyInterface.RemoveBody(joltID);
    bodyInterface.DestroyBody(joltID);
    m_joltBodyID = YK_NumericLimits<YK_U32>::Max;
}