#pragma once

#include "YKC/Types/Math/YKC_Integer.h"
#include "YKC/Types/Math/YKC_Vector.h"
#include "YKC/Types/Traits/YKC_Concepts.h"

// TODO: Do a once over and improve where possible with SIMD
template <YK_NumericType DataType, YK_U32 RowCount, YK_U32 ColumnCount>
struct YK_Matrix_R_C
{
private:
    using ColumnVector = YK_Vector_N<DataType, RowCount>;

public:
    static const YK_Matrix_R_C Identity;

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

#include "YKC_Matrix.inl"