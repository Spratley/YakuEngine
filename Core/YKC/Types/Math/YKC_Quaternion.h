#pragma once

#include "YKC/Types/Math/YKC_Vector.h"
#include "YKC/Types/Traits/YKC_Concepts.h"

template <YK_NumericType DataType>
struct YK_Quaternion_T
{
public:
    constexpr YK_Quaternion_T();
    constexpr YK_Quaternion_T(DataType p_w, DataType p_x, DataType p_y, DataType p_z);

    // TODO: Re-make constepxr when C++26 finally comes (sob)
    // Why were sin and cos not constexpr before??
    /*constexpr*/ YK_Quaternion_T(YK_Vector_N<DataType, 3> const& p_axis, DataType p_angle);

    constexpr YK_Quaternion_T operator*(YK_Quaternion_T const& p_rhs) const;
    constexpr YK_Vector_N<DataType, 3> operator*(YK_Vector_N<DataType, 3> const& p_vector) const;

public:
    DataType w;
    DataType x;
    DataType y;
    DataType z;
};

using YK_Quaternion = YK_Quaternion_T<float>;

#include "YKC_Quaternion.inl"