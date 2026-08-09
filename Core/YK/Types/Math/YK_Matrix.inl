#include "YK/Math/YK_VectorMath.h"
#include "YK/Utils/YK_AlgorithmUtils.h"

template <YK_NumericType DataType, YK_U32 RowCount, YK_U32 ColumnCount>
inline constexpr YK_Matrix_R_C<DataType, RowCount, ColumnCount>
  YK_Matrix_R_C<DataType, RowCount, ColumnCount>::Identity{ 1 };

template <YK_NumericType DataType, YK_U32 RowCount, YK_U32 ColumnCount>
constexpr YK_Matrix_R_C<DataType, RowCount, ColumnCount>::YK_Matrix_R_C()
    : YK_Matrix_R_C(1)
{}

template <YK_NumericType DataType, YK_U32 RowCount, YK_U32 ColumnCount>
constexpr YK_Matrix_R_C<DataType, RowCount, ColumnCount>::YK_Matrix_R_C(DataType p_diagonal)
    : m_columns{}
{
    constexpr YK_U32 lowestDimension = YK_Min(RowCount, ColumnCount);
    for (auto i : YK_CountTo(lowestDimension))
    {
        m_columns[i][i] = p_diagonal;
    }
}

template <YK_NumericType DataType, YK_U32 RowCount, YK_U32 ColumnCount>
template <typename... Args>
requires(sizeof...(Args) == (RowCount * ColumnCount))
constexpr YK_Matrix_R_C<DataType, RowCount, ColumnCount>::YK_Matrix_R_C(Args... args)
    : m_columns{}
{
    DataType temp[] = { static_cast<DataType>(args)... };
    DataType* data = GetData();
    for (auto i : YK_CountTo(RowCount * ColumnCount))
    {
        data[i] = temp[i];
    }
}

template <YK_NumericType DataType, YK_U32 RowCount, YK_U32 ColumnCount>
constexpr YK_Matrix_R_C<DataType, RowCount, ColumnCount>::YK_Matrix_R_C(YK_Matrix_R_C const& p_other)
{
    for (auto c : YK_CountTo(ColumnCount))
    {
        for (auto r : YK_CountTo(RowCount))
        {
            m_columns[c][r] = p_other.m_columns[c][r];
        }
    }
}

template <YK_NumericType DataType, YK_U32 RowCount, YK_U32 ColumnCount>
constexpr YK_Matrix_R_C<DataType, RowCount, ColumnCount>::ColumnVector
  YK_Matrix_R_C<DataType, RowCount, ColumnCount>::operator*(YK_Vector_N<DataType, ColumnCount> const& p_vector) const
{
    ColumnVector result;
    for (auto i : YK_CountTo(RowCount))
    {
        result[i] = YK_Vector::Dot(p_vector, GetRow(i));
    }
    return result;
}

template <YK_NumericType DataType, YK_U32 RowCount, YK_U32 ColumnCount>
template <YK_U32 OtherRowCount, YK_U32 OtherColumnCount>
requires(OtherRowCount == ColumnCount)
constexpr YK_Matrix_R_C<DataType, RowCount, OtherColumnCount> YK_Matrix_R_C<DataType, RowCount, ColumnCount>::operator*(
  YK_Matrix_R_C<DataType, OtherRowCount, OtherColumnCount> const& p_rhs) const
{
    YK_Matrix_R_C<DataType, RowCount, OtherColumnCount> result;
    for (auto c : YK_CountTo(OtherColumnCount))
    {
        for (auto r : YK_CountTo(RowCount))
        {
            result[c][r] = YK_Vector::Dot(p_rhs[c], GetRow(r));
        }
    }
    return result;
}