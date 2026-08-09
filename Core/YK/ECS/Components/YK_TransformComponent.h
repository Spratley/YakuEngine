#pragma once

#include "YK/Types/Math/YK_Quaternion.h"
#include "YK/Types/Math/YK_Vector.h"

struct YK_TransformComponent
{
    YK_Vector3f m_position;
    YK_Quaternion m_orientation;
    YK_Vector3f m_scale = YK_Vector3f::One();
};