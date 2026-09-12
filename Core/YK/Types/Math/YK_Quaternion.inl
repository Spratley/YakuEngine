#include "YK/Math/YK_AngleMath.h"
#include "YK/Math/YK_VectorMath.h"
#include "YK/Types/Traits/YK_Concepts.h"
#include "YK_Vector.h"

template <YK_NumericType DataType>
constexpr YK_Quaternion_T<DataType>::YK_Quaternion_T()
    : x(0)
    , y(0)
    , z(0)
    , w(1)
{}

template <YK_NumericType DataType>
constexpr YK_Quaternion_T<DataType>::YK_Quaternion_T(DataType p_x, DataType p_y, DataType p_z, DataType p_w)
    : x(p_x)
    , y(p_y)
    , z(p_z)
    , w(p_w)
{}

template <YK_NumericType DataType>
inline constexpr YK_Quaternion_T<DataType>::YK_Quaternion_T(YK_Vector_N<DataType, 4> const& p_data)
    : x(p_data[0])
    , y(p_data[1])
    , z(p_data[2])
    , w(p_data[3])
{}

template <YK_NumericType DataType>
constexpr YK_Quaternion_T<DataType>::YK_Quaternion_T(YK_Vector_N<DataType, 3> const& p_axis, DataType p_angle)
{
    DataType const halfAngle = p_angle / static_cast<DataType>(2);
    DataType const cosAngle = YK_Cosine<DataType>(halfAngle);
    DataType const sinAngle = YK_Sine<DataType>(halfAngle);
    x = p_axis[0] * sinAngle;
    y = p_axis[1] * sinAngle;
    z = p_axis[2] * sinAngle;
    w = cosAngle;
}

template <YK_NumericType DataType>
constexpr YK_Quaternion_T<DataType> YK_Quaternion_T<DataType>::operator*(YK_Quaternion_T const& p_rhs) const
{
    return YK_Quaternion_T{ (w * p_rhs.x) + (x * p_rhs.w) + (y * p_rhs.z) - (z * p_rhs.y),
                            (w * p_rhs.y) - (x * p_rhs.z) + (y * p_rhs.w) + (z * p_rhs.x),
                            (w * p_rhs.z) + (x * p_rhs.y) - (y * p_rhs.x) + (z * p_rhs.w),
                            (w * p_rhs.w) - (x * p_rhs.x) - (y * p_rhs.y) - (z * p_rhs.z) };
}

template <YK_NumericType DataType>
constexpr YK_Vector_N<DataType, 3> YK_Quaternion_T<DataType>::operator*(YK_Vector_N<DataType, 3> const& p_vector) const
{
    YK_Vector_N<DataType, 3> const vectorizedQuaternion{ x, y, z };
    DataType const quatDotVector = static_cast<DataType>(2) * YK_Vector::Dot(vectorizedQuaternion, p_vector);
    DataType const quatDotQuat = YK_Vector::Dot(vectorizedQuaternion, vectorizedQuaternion);
    return (vectorizedQuaternion * quatDotVector) + (p_vector * ((w * w) - quatDotQuat))
           + (YK_Vector::Cross(vectorizedQuaternion, p_vector) * static_cast<DataType>(2) * w);
}

template <YK_NumericType DataType>
constexpr YK_Quaternion_T<DataType> YK_Quaternion_T<DataType>::GetNormalized() const
{
    return YK_Quaternion_T{ YK_Vector::GetNormalized(YK_Vector_N<DataType, 4>{ x, y, z, w }) };
}
