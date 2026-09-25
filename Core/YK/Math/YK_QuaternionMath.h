#pragma once

#include "YK/Types/Math/YK_Quaternion.h"

namespace YK_QuaternionMath
{
    constexpr float Dot(YK_Quaternion const& p_a, YK_Quaternion const& p_b)
    {
        return YK_Vector::Dot(YK_Vector4f(p_a.x, p_a.y, p_a.z, p_a.w), YK_Vector4f(p_b.x, p_b.y, p_b.z, p_b.w));
    }
} // namespace YK_QuaternionMath

inline constexpr YK_Quaternion YK_Lerp(YK_Quaternion const& p_a,
                                       YK_Quaternion /* Pass by copy so we can invert if needed */ p_b,
                                       float p_t)
{
    // Overload instead of specialization. Thanks Raymond Chen for the reality check!
    float dotQuat = YK_QuaternionMath::Dot(p_a, p_b);
    if (dotQuat < 0.0f)
    {
        p_b = -p_b;
    }
    YK_Quaternion result{ YK_Lerp(p_a.x, p_b.x, p_t),
                          YK_Lerp(p_a.y, p_b.y, p_t),
                          YK_Lerp(p_a.z, p_b.z, p_t),
                          YK_Lerp(p_a.w, p_b.w, p_t) };
    return result.GetNormalized();
}