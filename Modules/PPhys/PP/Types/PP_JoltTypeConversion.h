#pragma once

#include "YK/Types/Math/YK_Quaternion.h"
#include "YK/Types/Math/YK_Vector.h"

#include <Jolt/Jolt.h>
#include "Jolt/Math/Quat.h"
#include "Jolt/Math/Vec3.h"

static inline YK_Vector3f PP_ToNative(JPH::RVec3 const& p_joltVector)
{ return YK_Vector3f{ p_joltVector.GetX(), p_joltVector.GetY(), p_joltVector.GetZ() }; }

static inline JPH::RVec3 PP_ToJolt(YK_Vector3f const& p_yakuVector)
{ return JPH::RVec3{ p_yakuVector.x, p_yakuVector.y, p_yakuVector.z }; }

static inline YK_Quaternion PP_ToNative(JPH::Quat const& p_joltQuaternion)
{
    return YK_Quaternion{ p_joltQuaternion.GetX(),
                          p_joltQuaternion.GetY(),
                          p_joltQuaternion.GetZ(),
                          p_joltQuaternion.GetW() };
}

static inline JPH::Quat PP_ToJolt(YK_Quaternion const& p_yakuQuaternion)
{ return JPH::Quat{ p_yakuQuaternion.x, p_yakuQuaternion.y, p_yakuQuaternion.z, p_yakuQuaternion.w }; }