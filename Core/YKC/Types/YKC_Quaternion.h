#pragma once

#include "YKC/Types/YKC_Vector.h"

#pragma warning(push)
#pragma warning(disable : 4201)

template <typename DataType>
struct YK_Quaternion_T
{
public:
    constexpr YK_Quaternion_T()
        : w(1)
        , x(0)
        , y(0)
        , z(0)
    {}

    constexpr YK_Quaternion_T(DataType p_w, DataType p_x, DataType p_y, DataType p_z)
        : w(p_w)
        , x(p_x)
        , y(p_y)
        , z(p_z)
    {}

    // TODO: Re-make constepxr when C++26 finally comes (sob)
    // Why were sin and cos not constexpr before??
    /*constexpr*/ YK_Quaternion_T(YK_Vector_N<DataType, 3> const& p_axis, DataType p_angle)
    {
        DataType const halfAngle = p_angle / static_cast<DataType>(2);
        DataType const cosAngle = std::cos(halfAngle);
        DataType const sinAngle = std::sin(halfAngle);
        w = cosAngle;
        x = p_axis[0] * sinAngle;
        y = p_axis[1] * sinAngle;
        z = p_axis[2] * sinAngle;
    }

    constexpr YK_Quaternion_T operator*(YK_Quaternion_T const& p_rhs) const
    {
        return YK_Quaternion_T{ (w * p_rhs.w) - (x * p_rhs.x) - (y * p_rhs.y) - (z * p_rhs.z),
                                (w * p_rhs.x) + (x * p_rhs.w) + (y * p_rhs.z) - (z * p_rhs.y),
                                (w * p_rhs.y) - (x * p_rhs.z) + (y * p_rhs.w) + (z * p_rhs.x),
                                (w * p_rhs.z) + (x * p_rhs.y) - (y * p_rhs.x) + (z * p_rhs.w) };
    }

    constexpr YK_Vector_N<DataType, 3> operator*(YK_Vector_N<DataType, 3> const& p_vector) const
    {
        YK_Vector_N<DataType, 3> const vectorizedQuaternion{ x, y, z };
        DataType const quatDotVector = static_cast<DataType>(2) * YK_Vector::Dot(vectorizedQuaternion, p_vector);
        DataType const quatDotQuat = YK_Vector::Dot(vectorizedQuaternion, vectorizedQuaternion);

        auto const term1 = vectorizedQuaternion * quatDotVector;
        auto const term2 = p_vector * ((w * w) - quatDotQuat);
        auto const term3 = YK_Vector::Cross(vectorizedQuaternion, p_vector) * static_cast<DataType>(2) * w;

        return term1 + term2 + term3;
    }

public:
    DataType w;
    DataType x;
    DataType y;
    DataType z;
};

using YK_Quaternion = YK_Quaternion_T<float>;

#pragma warning(pop)