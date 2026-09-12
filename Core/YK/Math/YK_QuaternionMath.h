#pragma once

#include "YK/Types/Math/YK_Quaternion.h"

inline constexpr YK_Quaternion YK_Lerp(YK_Quaternion const& p_a, YK_Quaternion const& p_b, float p_t)
{
    // Overload instead of specialization. Thanks Raymond Chen for the reality check!
    YK_Quaternion result{ YK_Lerp(p_a.x, p_b.x, p_t),
                          YK_Lerp(p_a.y, p_b.y, p_t),
                          YK_Lerp(p_a.z, p_b.z, p_t),
                          YK_Lerp(p_a.w, p_b.w, p_t) };
    return result.GetNormalized();
}