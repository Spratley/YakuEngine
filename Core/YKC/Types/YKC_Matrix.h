#pragma once

// Remove these
#include "../Math/YKC_MathUtils.h"
#include "YKC_Integer.h"

#include "YKC/Types/YKC_Vector.h"

template <typename DataType, YK_U32 RowCount, YK_U32 ColumnCount>
struct YK_Matrix_R_C
{
	using MatrixType = YK_Matrix_R_C<DataType, RowCount, ColumnCount>;
public:
	static constexpr MatrixType Identity() { return MatrixType(1); }

public:
	constexpr YK_Matrix_R_C() { *this = Identity(); }

	constexpr YK_Matrix_R_C(DataType p_diagonal) : m_rows{}
	{
		YK_U32 lowestDimension = YK_Min(RowCount, ColumnCount);
		for (YK_U32 i = 0; i < lowestDimension; ++i)
		{
			m_rows[i][i] = p_diagonal;
		}
	}

	template <typename... Args, typename = typename YK_EnableIf<(sizeof...(Args) == (RowCount * ColumnCount))>::Type>
	YK_Matrix_R_C(Args... args) : m_rows{}
	{
		DataType temp[] = { static_cast<DataType>(args)... };
		std::memcpy(GetData(), temp, sizeof(DataType) * (RowCount * ColumnCount));
	}

	// Note: Shallow copies only!
	YK_Matrix_R_C(MatrixType const& p_other)
	{
		std::memcpy(GetData(), p_other.GetData(), sizeof(DataType) * (RowCount * ColumnCount));
	}

	constexpr DataType& operator[](size_t const p_index) { return m_rows[p_index]; }
	constexpr DataType const& operator[](size_t const p_index) const { return m_rows[p_index]; }

	DataType* GetData() { return m_rows[0].GetData(); }
	constexpr DataType const* GetData() const { return m_rows[0].GetData(); }

private:
	YK_Vector_N<DataType, ColumnCount> m_rows[RowCount];
};

using YK_Matrix22 = YK_Matrix_R_C<float, 2, 2>;
using YK_Matrix33 = YK_Matrix_R_C<float, 3, 3>;
using YK_Matrix44 = YK_Matrix_R_C<float, 4, 4>;