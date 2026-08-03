#pragma once

#include "YKC/Types/YKC_Quaternion.h"
#include "YKC/Types/YKC_TypeTraits.h"
#include "YKC/Types/YKC_Vector.h"
#include "YKC/Utils/YKC_AlgorithmUtils.h"
#include "YKC/Utils/YKC_MathUtils.h"

#include <cmath>

template <typename DataType, YK_U32 RowCount, YK_U32 ColumnCount>
struct YK_Matrix_R_C
{
public:
    static constexpr YK_Matrix_R_C Identity() { return YK_Matrix_R_C(1); }

public:
    constexpr YK_Matrix_R_C() { *this = Identity(); }

    constexpr YK_Matrix_R_C(DataType p_diagonal)
        : m_columns{}
    {
        constexpr YK_U32 lowestDimension = YK_Min(RowCount, ColumnCount);
        for (YK_U32 i = 0; i < lowestDimension; ++i)
        {
            m_columns[i][i] = p_diagonal;
        }
    }

    template <typename... Args, typename = typename YK_EnableIf<(sizeof...(Args) == (RowCount * ColumnCount))>::Type>
    constexpr YK_Matrix_R_C(Args... args)
        : m_columns{}
    {
        DataType temp[] = { static_cast<DataType>(args)... };
        // Memcpy is replaced with a fixed loop because according to godbolt they get optimized to the same thing, but
        // loops are constepxr
        DataType* data = GetData();
        for (int i = 0; i < RowCount * ColumnCount; ++i)
        {
            data[i] = temp[i];
        }
    }

    // Note: Shallow copies only!
    // TODO: Concept to prevent non-trivially copyable types
    constexpr YK_Matrix_R_C(YK_Matrix_R_C const& p_other)
    {
        // Memcpy is replaced with a fixed loop because according to godbolt they get optimized to the same thing, but
        // loops are constepxr
        for (int c = 0; c < ColumnCount; ++c)
        {
            for (int r = 0; r < RowCount; ++r)
            {
                m_columns[c][r] = p_other.m_columns[c][r];
            }
        }
    }

    constexpr YK_Vector_N<DataType, RowCount>& operator[](size_t const p_index) { return m_columns[p_index]; }
    constexpr YK_Vector_N<DataType, RowCount> const& operator[](size_t const p_index) const
    {
        return m_columns[p_index];
    }

    friend constexpr YK_Vector_N<DataType, RowCount> operator*(YK_Matrix_R_C const& p_matrix,
                                                               YK_Vector_N<DataType, ColumnCount> const& p_vector)
    {
        YK_Vector_N<DataType, RowCount> result;
        for (int i = 0; i < RowCount; ++i)
        {
            result[i] = YK_Vector::Dot(p_vector, p_matrix.GetRow(i));
        }
        return result;
    }

    // TODO: Compiler intrinsics SIMD?
    template <YK_U32 OtherRowCount,
              YK_U32 OtherColumnCount,
              typename = typename YK_EnableIf<OtherRowCount == ColumnCount>::Type>
    friend constexpr YK_Matrix_R_C<DataType, RowCount, OtherColumnCount> operator*(
      YK_Matrix_R_C const& p_lhs,
      YK_Matrix_R_C<DataType, OtherRowCount, OtherColumnCount> const& p_rhs)
    {
        YK_Matrix_R_C<DataType, RowCount, OtherColumnCount> result;
        for (int c = 0; c < OtherColumnCount; ++c)
        {
            for (int r = 0; r < RowCount; ++r)
            {
                result[c][r] = YK_Vector::Dot(p_rhs[c], p_lhs.GetRow(r));
            }
        }
        return result;
    }

    constexpr DataType* GetData() { return m_columns[0].m_data; }
    constexpr DataType const* GetData() const { return m_columns[0].m_data; }

    constexpr YK_VectorView<DataType const, ColumnCount, RowCount> GetRow(int p_rowIndex) const
    {
        return YK_VectorView<DataType const, ColumnCount, RowCount>(m_columns[0].m_data + p_rowIndex);
    }

private:
    YK_Vector_N<DataType, RowCount> m_columns[ColumnCount];
};

namespace YK_Matrix
{
    template <typename DataType>
    constexpr void Translate(YK_Matrix_R_C<DataType, 4, 4>& p_matrix, YK_Vector_N<DataType, 3> const& p_translation)
    {
        p_matrix[3].xyz += p_translation;
    }

    template <typename DataType>
    constexpr YK_Vector_N<DataType, 3> const& GetPosition(YK_Matrix_R_C<DataType, 4, 4> const& p_matrix)
    {
        return p_matrix[3].xyz;
    }

    template <typename DataType>
    constexpr void SetPosition(YK_Matrix_R_C<DataType, 4, 4>& p_matrix, YK_Vector_N<DataType, 3> const& p_position)
    {
        p_matrix[3].xyz = p_position;
    }

    // TODO: Come back and figure this out
    // template <typename DataType>
    // constexpr void Rotate(YK_Matrix_R_C<DataType, 4, 4>& p_matrix, YK_Vector_N<DataType, 3> const& p_eulerAngles)
    //{
    //    float const cosPitch = std::cos(p_eulerAngles.x);
    //    float const sinPitch = std::sin(p_eulerAngles.x);

    //    float const cosYaw = std::cos(p_eulerAngles.y);
    //    float const sinYaw = std::sin(p_eulerAngles.y);

    //    float const cosRoll = std::cos(p_eulerAngles.z);
    //    float const sinRoll = std::sin(p_eulerAngles.z);

    //    YK_Matrix_R_C<DataType, 3, 3> roll{ cosRoll, sinRoll, 0, -sinRoll, cosRoll, 0, 0, 0, 1 };
    //    YK_Matrix_R_C<DataType, 3, 3> pitch{ 1, 0, 0, 0, cosPitch, sinPitch, 0, -sinPitch, cosPitch };
    //    YK_Matrix_R_C<DataType, 3, 3> yaw{ cosYaw, 0, -sinYaw, 0, 1, 0, sinYaw, 0, cosYaw };

    //    YK_Matrix_R_C<DataType, 3, 3> rotationMatrix;
    //    rotationMatrix[0] = p_matrix[0].xyz;
    //    rotationMatrix[1] = p_matrix[1].xyz;
    //    rotationMatrix[2] = p_matrix[2].xyz;

    //    rotationMatrix = yaw * rotationMatrix * pitch * roll;

    //    p_matrix[0].xyz = rotationMatrix[0];
    //    p_matrix[1].xyz = rotationMatrix[1];
    //    p_matrix[2].xyz = rotationMatrix[2];
    //}

    template <typename DataType>
    constexpr YK_Matrix_R_C<DataType, 4, 4> Inverse(YK_Matrix_R_C<DataType, 4, 4> const& p_matrix)
    {
        // Affine transformation matrix
        // Data is stored as a column major matrix
        // Inverse of the rotation 3x3 is the same as the transpose
        // We can then multiply the negative of that into the translation

        // | [0, 0] [1, 0] [2, 0] | [3, 0]
        // | [0, 1] [1, 1] [2, 1] | [3, 1]
        // | [0, 2] [1. 2] [2, 2] | [3, 2]
        // ( [0, 3] [1, 3] [2, 3] ) {3, 3}

        YK_Matrix_R_C<DataType, 4, 4> result = p_matrix;

        // Transpose rotation matrix
        YK_Swap(result[0][1], result[1][0]);
        YK_Swap(result[0][2], result[2][0]);
        YK_Swap(result[1][2], result[2][1]);

        // Combine into translation
        // Should this be combined into a matrix view so we're not creating a temporary?
        YK_Matrix_R_C<DataType, 3, 3> rotationMatrix;
        rotationMatrix[0] = result[0].xyz;
        rotationMatrix[1] = result[1].xyz;
        rotationMatrix[2] = result[2].xyz;

        result[3].xyz = rotationMatrix * -(result[3].xyz);
        return result;
    }

    template <typename DataType>
    /*constexpr*/ YK_Quaternion_T<DataType> ToQuaternion(YK_Matrix_R_C<DataType, 3, 3> const& p_matrix)
    {
        // Based on GLM
        DataType const fourXSquaredMinus1 = p_matrix[0][0] - p_matrix[1][1] - p_matrix[2][2];
        DataType const fourYSquaredMinus1 = p_matrix[1][1] - p_matrix[0][0] - p_matrix[2][2];
        DataType const fourZSquaredMinus1 = p_matrix[2][2] - p_matrix[0][0] - p_matrix[1][1];
        DataType const fourWSquaredMinus1 = p_matrix[0][0] + p_matrix[1][1] + p_matrix[2][2];

        int biggestIndex = 0;
        DataType fourBiggestSquaredMinus1 = fourWSquaredMinus1;
        if (fourXSquaredMinus1 > fourBiggestSquaredMinus1)
        {
            fourBiggestSquaredMinus1 = fourXSquaredMinus1;
            biggestIndex = 1;
        }
        if (fourYSquaredMinus1 > fourBiggestSquaredMinus1)
        {
            fourBiggestSquaredMinus1 = fourYSquaredMinus1;
            biggestIndex = 2;
        }
        if (fourZSquaredMinus1 > fourBiggestSquaredMinus1)
        {
            fourBiggestSquaredMinus1 = fourZSquaredMinus1;
            biggestIndex = 3;
        }

        DataType const biggestValue =
          std::sqrt(fourBiggestSquaredMinus1 + static_cast<DataType>(1)) * static_cast<DataType>(0.5);
        DataType multiplier = static_cast<DataType>(0.25) / biggestValue;

        switch (biggestIndex)
        {
            default: return YK_Quaternion_T<DataType>{};
            case 0:
                return YK_Quaternion_T<DataType>{ biggestValue,
                                                  (p_matrix[1][2] - p_matrix[2][1]) * multiplier,
                                                  (p_matrix[2][0] - p_matrix[0][2]) * multiplier,
                                                  (p_matrix[0][1] - p_matrix[1][0]) * multiplier };
            case 1:
                return YK_Quaternion_T<DataType>{ (p_matrix[1][2] - p_matrix[2][1]) * multiplier,
                                                  biggestValue,
                                                  (p_matrix[0][1] + p_matrix[1][0]) * multiplier,
                                                  (p_matrix[2][0] + p_matrix[0][2]) * multiplier };
            case 2:
                return YK_Quaternion_T<DataType>{ (p_matrix[2][0] - p_matrix[0][2]) * multiplier,
                                                  (p_matrix[0][1] + p_matrix[1][0]) * multiplier,
                                                  biggestValue,
                                                  (p_matrix[1][2] + p_matrix[2][1]) * multiplier };
            case 3:
                return YK_Quaternion_T<DataType>{ (p_matrix[0][1] - p_matrix[1][0]) * multiplier,
                                                  (p_matrix[2][0] + p_matrix[0][2]) * multiplier,
                                                  (p_matrix[1][2] + p_matrix[2][1]) * multiplier,
                                                  biggestValue };
        }
    }

    template <typename DataType>
    constexpr void LookAt(YK_Matrix_R_C<DataType, 4, 4>& p_matrix,
                          YK_Vector_N<DataType, 3> const& p_lookTarget,
                          YK_Vector_N<DataType, 3> const& p_up = YK_Vector_N<DataType, 3>::Up())
    {
        YK_Vector_N<DataType, 3> const forward = -YK_Vector::GetNormalized(p_lookTarget - p_matrix[3].xyz);
        YK_Vector_N<DataType, 3> const right = YK_Vector::Cross(p_up, forward);
        YK_Vector_N<DataType, 3> const up = YK_Vector::Cross(forward, right);

        p_matrix[0].xyz = right;
        p_matrix[1].xyz = up;
        p_matrix[2].xyz = forward;
    }

    template <typename DataType>
    constexpr YK_Quaternion_T<DataType> LookAt(YK_Vector_N<DataType, 3> const& p_eyePosition,
                                               YK_Vector_N<DataType, 3> const& p_lookTarget,
                                               YK_Vector_N<DataType, 3> const& p_up = YK_Vector_N<DataType, 3>::Up())
    {
        YK_Vector_N<DataType, 3> const forward = -YK_Vector::GetNormalized(p_lookTarget - p_eyePosition);
        YK_Vector_N<DataType, 3> const right = YK_Vector::Cross(p_up, forward);
        YK_Vector_N<DataType, 3> const up = YK_Vector::Cross(forward, right);

        return ToQuaternion(YK_Matrix_R_C<DataType, 3, 3>{ right.x,
                                                           right.y,
                                                           right.z,
                                                           up.x,
                                                           up.y,
                                                           up.z,
                                                           forward.x,
                                                           forward.y,
                                                           forward.z });
    }

    template <typename DataType>
    constexpr YK_Matrix_R_C<DataType, 3, 3> Rotation(YK_Quaternion_T<DataType> const& p_orientation)
    {
        DataType const xSquare = YK_Square(p_orientation.x) * 2;
        DataType const ySquare = YK_Square(p_orientation.y) * 2;
        DataType const zSquare = YK_Square(p_orientation.z) * 2;

        DataType const xy = p_orientation.x * p_orientation.y * 2;
        DataType const xz = p_orientation.x * p_orientation.z * 2;

        DataType const yz = p_orientation.y * p_orientation.z * 2;

        DataType const wx = p_orientation.w * p_orientation.x * 2;
        DataType const wy = p_orientation.w * p_orientation.y * 2;
        DataType const wz = p_orientation.w * p_orientation.z * 2;

        return YK_Matrix_R_C<DataType, 3, 3>{ (1 - ySquare - zSquare), (xy + wz), (xz - wy), (xy - wz),
                                              (1 - xSquare - zSquare), (yz + wx), (xz + wy), (yz - wx),
                                              (1 - xSquare - ySquare) };
    }

    template <typename DataType>
    constexpr YK_Matrix_R_C<DataType, 4, 4> Construct(YK_Vector_N<DataType, 3> const& p_position,
                                                      YK_Quaternion_T<DataType> const& p_orientation,
                                                      YK_Vector_N<DataType, 3> const& p_scale)
    {
        YK_Matrix_R_C<DataType, 4, 4> result;
        // TODO: Replace with in-place construction for 4x4
        YK_Matrix_R_C<DataType, 3, 3> rotation = Rotation(p_orientation);
        result[0].xyz = rotation[0] * p_scale.x;
        result[1].xyz = rotation[1] * p_scale.y;
        result[2].xyz = rotation[2] * p_scale.z;
        result[3].xyz = p_position;

        return result;
    }
} // namespace YK_Matrix

using YK_Matrix22 = YK_Matrix_R_C<float, 2, 2>;
using YK_Matrix33 = YK_Matrix_R_C<float, 3, 3>;
using YK_Matrix44 = YK_Matrix_R_C<float, 4, 4>;