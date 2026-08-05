#pragma once

#include "YKC/Types/Math/YKC_Matrix.h"
#include "YKC/Math/YKC_MathUtils.h"

namespace YK_Matrix
{
    // Affine Transformation Matrices
    template <typename DataType>
    constexpr void Translate(YK_Matrix_R_C<DataType, 4, 4>& p_matrix, YK_Vector_N<DataType, 3> const& p_translation)
    {
        p_matrix[3].xyz += p_translation;
    }

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