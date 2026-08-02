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
        : m_rows{}
    {
        YK_U32 lowestDimension = YK_Min(RowCount, ColumnCount);
        for (YK_U32 i = 0; i < lowestDimension; ++i)
        {
            m_rows[i][i] = p_diagonal;
        }
    }

    template <typename... Args, typename = typename YK_EnableIf<(sizeof...(Args) == (RowCount * ColumnCount))>::Type>
    constexpr YK_Matrix_R_C(Args... args)
        : m_rows{}
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
        for (int r = 0; r < RowCount; ++r)
        {
            for (int c = 0; c < ColumnCount; ++c)
            {
                m_rows[r][c] = p_other.m_rows[r][c];
            }
        }
    }

    constexpr YK_Vector_N<DataType, ColumnCount>& operator[](size_t const p_index) { return m_rows[p_index]; }
    constexpr YK_Vector_N<DataType, ColumnCount> const& operator[](size_t const p_index) const
    {
        return m_rows[p_index];
    }

    friend constexpr YK_Vector_N<DataType, RowCount> operator*(MatrixType const& p_matrix,
                                                               YK_Vector_N<DataType, ColumnCount> const& p_vector)
    {
        YK_Vector_N<DataType, RowCount> result;
        for (int i = 0; i < RowCount; ++i)
        {
            result[i] = YK_Vector::Dot(p_matrix[i], p_vector);
        }
        return result;
    }

    // TODO: Compiler intrinsics SIMD?
    template <YK_U32 OtherMatrixRowCount,
              YK_U32 OtherMatrixColumnCount,
              typename = typename YK_EnableIf<OtherMatrixRowCount == ColumnCount>::Type>
    friend constexpr YK_Matrix_R_C<DataType, RowCount, OtherMatrixColumnCount> operator*(
      MatrixType const& p_lhs,
      YK_Matrix_R_C<DataType, OtherMatrixRowCount, OtherMatrixColumnCount> const& p_rhs)
    {
        YK_Matrix_R_C<DataType, RowCount, OtherMatrixColumnCount> result;
        for (int i = 0; i < RowCount; ++i)
        {
            for (int j = 0; j < OtherMatrixColumnCount; ++j)
            {
                result[i][j] = YK_Vector::Dot(p_lhs[i], p_rhs.GetColumn(j));
            }
        }
        return result;
    }

    constexpr DataType* GetData() { return m_rows[0].m_data; }
    constexpr DataType const* GetData() const { return m_rows[0].m_data; }

    constexpr YK_VectorView<DataType const, RowCount, ColumnCount> GetColumn(int p_columnIndex) const
    {
        return YK_VectorView<DataType const, RowCount, ColumnCount>(m_rows[0].m_data + p_columnIndex);
    }

private:
    YK_Vector_N<DataType, ColumnCount> m_rows[RowCount];
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
        // Data is stored as a transposed row major matrix
        // Technically the same as a column major matrix...
        // Maybe I should rename it?
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
        YK_Matrix_R_C<DataType, 3, 3> rotationMatrix(result[0][0],
                                                     result[0][1],
                                                     result[0][2],
                                                     result[1][0],
                                                     result[1][1],
                                                     result[1][2],
                                                     result[2][0],
                                                     result[2][1],
                                                     result[2][2]);

        result[3].xyz = rotationMatrix * -(result[3].xyz);
        return result;
    }
} // namespace YK_Matrix

using YK_Matrix22 = YK_Matrix_R_C<float, 2, 2>;
using YK_Matrix33 = YK_Matrix_R_C<float, 3, 3>;
using YK_Matrix44 = YK_Matrix_R_C<float, 4, 4>;