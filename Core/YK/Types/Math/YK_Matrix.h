#pragma once

#include "YK/Types/Math/YK_Integer.h"
#include "YK/Types/Math/YK_Vector.h"
#include "YK/Types/Traits/YK_Concepts.h"

#include <concepts>

// A matrix type requires:
//  - A nested typename "DataType_T" that is numeric
//  - The ability to be indexed, returning an instance of DataType_T
//  - A static member "RowCount_V" that should return the dimension count of the matrix rows
//  - A static member "ColumnCount_V" that should return the dimension count of the matrix columns
template <typename Matrix>
concept YK_MatrixType = requires(Matrix m) {
    typename Matrix::DataType_T;
    requires YK_NumericType<typename Matrix::DataType_T>;
    { m[0][0] } -> std::convertible_to<typename Matrix::DataType_T>;
    Matrix::RowCount_V;
    Matrix::ColumnCount_V;
};

// TODO: Do a once over and improve where possible with SIMD
template <YK_NumericType DataType, YK_U32 RowCount, YK_U32 ColumnCount>
struct YK_Matrix_R_C
{
public:
    using ColumnVector = YK_Vector_N<DataType, RowCount>;
    using DataType_T = DataType;
    static constexpr YK_U32 RowCount_V = RowCount;
    static constexpr YK_U32 ColumnCount_V = ColumnCount;

    static consteval YK_Matrix_R_C Identity() { return YK_Matrix_R_C{ static_cast<DataType>(1) }; }

public:
    constexpr YK_Matrix_R_C(); // Returns Identity
    constexpr YK_Matrix_R_C(DataType p_diagonal);

    template <typename... Args>
    requires(sizeof...(Args) == (RowCount * ColumnCount))
    constexpr YK_Matrix_R_C(Args... args);

    constexpr YK_Matrix_R_C(YK_Matrix_R_C const& p_other);

    constexpr ColumnVector& operator[](YK_SizeT const p_index) { return m_columns[p_index]; }
    constexpr ColumnVector const& operator[](YK_SizeT const p_index) const { return m_columns[p_index]; }

    constexpr ColumnVector operator*(YK_Vector_N<DataType, ColumnCount> const& p_vector) const;

    template <YK_U32 OtherRowCount, YK_U32 OtherColumnCount>
    requires(OtherRowCount == ColumnCount)
    constexpr YK_Matrix_R_C<DataType, RowCount, OtherColumnCount> operator*(
      YK_Matrix_R_C<DataType, OtherRowCount, OtherColumnCount> const& p_rhs) const;

    constexpr DataType* GetData() { return m_columns[0].m_data; }
    constexpr DataType const* GetData() const { return m_columns[0].m_data; }

    constexpr YK_VectorView<DataType const, ColumnCount, RowCount> GetRow(YK_SizeT p_rowIndex) const
    {
        return YK_VectorView<DataType const, ColumnCount, RowCount>(m_columns[0].m_data + p_rowIndex);
    }

private:
    YK_Vector_N<DataType, RowCount> m_columns[ColumnCount];
};

using YK_Matrix22 = YK_Matrix_R_C<float, 2, 2>;
using YK_Matrix33 = YK_Matrix_R_C<float, 3, 3>;
using YK_Matrix44 = YK_Matrix_R_C<float, 4, 4>;

#include "YK_Matrix.inl"