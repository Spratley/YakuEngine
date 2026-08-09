#pragma once

#include "YKC/Types/Math/YKC_Quaternion.h"
#include "YKC/Types/Math/YKC_Vector.h"

struct YK_TransformComponent
{
    YK_Vector3f m_position;
    YK_Quaternion m_orientation;
    YK_Vector3f m_scale = YK_Vector3f::One();
};