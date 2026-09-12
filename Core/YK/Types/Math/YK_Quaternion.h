#pragma once

#include "YK/Types/Math/YK_Vector.h"
#include "YK/Types/Traits/YK_Concepts.h"

template <YK_NumericType DataType>
struct YK_Quaternion_T
{
public:
    constexpr YK_Quaternion_T();
    constexpr YK_Quaternion_T(DataType p_x, DataType p_y, DataType p_z, DataType p_w);
    constexpr YK_Quaternion_T(YK_Vector_N<DataType, 4> const& p_data);
    constexpr YK_Quaternion_T(YK_Vector_N<DataType, 3> const& p_axis, DataType p_angle);

    constexpr YK_Quaternion_T operator*(YK_Quaternion_T const& p_rhs) const;
    constexpr YK_Vector_N<DataType, 3> operator*(YK_Vector_N<DataType, 3> const& p_vector) const;

    constexpr YK_Quaternion_T GetNormalized() const;

public:
    DataType x;
    DataType y;
    DataType z;
    DataType w;
};

using YK_Quaternion = YK_Quaternion_T<float>;

#include "YK_Quaternion.inl"