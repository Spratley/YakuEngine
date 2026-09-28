#pragma once

#include "YK/Math/YK_VectorMath.h"
#include "YK/Types/Math/YK_Quaternion.h"
#include "YK/Types/Math/YK_Vector.h"
#include "YK/Types/Traits/YK_Concepts.h"

template <YK_NumericType DataType>
struct YK_Transform_T
{
    using Vector3Type = YK_Vector_N<DataType, 3>;

    constexpr Vector3Type Forward() const
    {
        // Mathematical simplification of {m_orientation * Vector3::Forward()}
        // It's able to be compacted because we know that X and Y are always 0
        constexpr DataType two = static_cast<DataType>(2);
        Vector3Type const vectorizedQuaternion{ m_orientation.x, m_orientation.y, m_orientation.z };
        DataType const quatDotQuat = YK_Vector::Dot(vectorizedQuaternion, vectorizedQuaternion);
        DataType const z2 = m_orientation.z * two;
        DataType const w2 = m_orientation.w * two;

        return Vector3Type{ (z2 * m_orientation.x) + (w2 * m_orientation.y),
                            (z2 * m_orientation.y) - (w2 * m_orientation.x),
                            (z2 * m_orientation.z) + (m_orientation.w * m_orientation.w) - quatDotQuat };
    }

    constexpr Vector3Type Forward2D() const
    {
        Vector3Type forward = Forward();
        forward.y = 0.0f;
        return YK_Vector::GetNormalized(forward);
    }

    Vector3Type m_position = {};
    YK_Quaternion_T<DataType> m_orientation = {};
    Vector3Type m_scale = Vector3Type::One();
};

using YK_Transform = YK_Transform_T<float>;
using YK_TransformComponent = YK_Transform;