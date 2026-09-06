#pragma once

#include "YK/ECS/Components/YK_TransformComponent.h"
#include "YK/Math/YK_NumericLimits.h"
#include "YK/Types/Math/YK_Integer.h"
#include "YK/Types/Math/YK_Vector.h"
#include "YK/Utils/YK_AlgorithmUtils.h"

// This is very cluttered, this should be moved to a factory class
namespace JPH
{
    class Shape;
}

// Not good to have this separate definition of the same Jolt enum
enum PP_BodyType
{
    Static,
    Kinematic,
    Dynamic,
};

struct PP_RigidBodyComponent
{
    PP_RigidBodyComponent() = default;
    PP_RigidBodyComponent(JPH::Shape* p_shape,
                          YK_TransformComponent const& p_transform,
                          PP_BodyType p_bodyType,
                          bool p_active = true);

    PP_RigidBodyComponent(PP_RigidBodyComponent const&) = delete;
    PP_RigidBodyComponent(PP_RigidBodyComponent&& p_other) noexcept
        : m_joltBodyID(YK_Exchange(p_other.m_joltBodyID, YK_NumericLimits<YK_U32>::Max))
    {}

    ~PP_RigidBodyComponent();

    PP_RigidBodyComponent& operator=(PP_RigidBodyComponent const&) = delete;
    PP_RigidBodyComponent& operator=(PP_RigidBodyComponent&& p_other) noexcept
    {
        m_joltBodyID = YK_Exchange(p_other.m_joltBodyID, YK_NumericLimits<YK_U32>::Max);
        return *this;
    }

    YK_U32 m_joltBodyID = YK_NumericLimits<YK_U32>::Max;
    YK_Vector3f m_bodyOffset = YK_Vector3f::Zero();
};