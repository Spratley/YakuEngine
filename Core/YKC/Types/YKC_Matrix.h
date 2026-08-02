#pragma once

#include "YKC/Types/YKC_TypeTraits.h"
#include "YKC/Types/YKC_Vector.h"
#include "YKC/Utils/YKC_AlgorithmUtils.h"

template <typename DataType, YK_U32 RowCount, YK_U32 ColumnCount>
struct YK_Matrix_R_C
{
    using MatrixType = YK_Matrix_R_C<DataType, RowCount, ColumnCount>;

public:
    static constexpr MatrixType Identity() { return MatrixType(1); }

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
    constexpr YK_Matrix_R_C(MatrixType const& p_other)
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

    friend constexpr YK_Vector_N<DataType, RowCount> operator*(MatrixType const& p_matrix,
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
      MatrixType const& p_lhs,
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

    template <typename DataType>
    constexpr void Rotate(YK_Matrix_R_C<DataType, 4, 4>& p_matrix, YK_Vector_N<DataType, 3> const& p_eulerAngles)
    {
        // This is not true
        p_matrix[0].xyz += p_eulerAngles;
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
} // namespace YK_Matrix

using YK_Matrix22 = YK_Matrix_R_C<float, 2, 2>;
using YK_Matrix33 = YK_Matrix_R_C<float, 3, 3>;
using YK_Matrix44 = YK_Matrix_R_C<float, 4, 4>;