#pragma once

#include <cmath>
#include <cstring>
#include "YKC_TypeTraits.h"

#define DECL_VECTOR_ACCESSOR(AccessorName, MinimumDimensionCount)																					\
template <typename VectorBase, typename DataType, YK_uint32 DimensionCount, typename Enable = void>														\
struct Vector_Accessor_##AccessorName {};																											\
template <typename VectorBase, typename DataType, YK_uint32 DimensionCount>																				\
struct Vector_Accessor_##AccessorName <VectorBase, DataType, DimensionCount, typename YK_EnableIf<(DimensionCount > MinimumDimensionCount)>::Type>	\
{																																					\
	constexpr inline DataType& AccessorName() { return static_cast<VectorBase*>(this)->m_data[MinimumDimensionCount]; }								\
	constexpr inline DataType const& AccessorName() const { return static_cast<VectorBase const*>(this)->m_data[MinimumDimensionCount]; }			\
}

DECL_VECTOR_ACCESSOR(x, 0);
DECL_VECTOR_ACCESSOR(y, 1);
DECL_VECTOR_ACCESSOR(z, 2);
DECL_VECTOR_ACCESSOR(w, 3);

#undef DECL_VECTOR_ACCESSOR

#define APPLY_VECTOR_ACCESSOR(AccessorName) public Vector_Accessor_##AccessorName<YK_Vector_N<DataType, DimensionCount>, DataType, DimensionCount>

template <typename DataType, YK_uint32 DimensionCount>
struct YK_Vector_N
	: APPLY_VECTOR_ACCESSOR(x)
	, APPLY_VECTOR_ACCESSOR(y)
	, APPLY_VECTOR_ACCESSOR(z)
	, APPLY_VECTOR_ACCESSOR(w)
{
private:
	using VectorType = YK_Vector_N<DataType, DimensionCount>;
public:
	static constexpr VectorType Zero() { return VectorType(0); }
	static constexpr VectorType One() { return VectorType(1); }

public:
	constexpr YK_Vector_N() : m_data{} {}
	constexpr YK_Vector_N(DataType p_defaultValue) : m_data{}
	{
		for (YK_uint32 i = 0; i < DimensionCount; ++i)
		{
			m_data[i] = p_defaultValue;
		}
	}

	template <typename... Args, typename = typename YK_EnableIf<(sizeof...(Args) == DimensionCount)>::Type>
	constexpr YK_Vector_N(Args... args) : m_data { static_cast<DataType>(args)... } 
	{}

	// Note: Shallow copies only! If for SOME reason a vector is defined that uses a non-shallow-copy-able type, this will fail
	YK_Vector_N(VectorType const& p_other)
	{
		std::memcpy(m_data, p_other.m_data, sizeof(DataType) * DimensionCount);
	}

	constexpr VectorType& operator+=(VectorType const& p_rhs)
	{
		for (YK_uint32 i = 0; i < DimensionCount; ++i)
		{
			m_data[i] += p_rhs.m_data[i];
		}
		return *this;
	}

	constexpr VectorType& operator-=(VectorType const& p_rhs)
	{
		for (YK_uint32 i = 0; i < DimensionCount; ++i)
		{
			m_data[i] -= p_rhs.m_data[i];
		}
		return *this;
	}

	friend constexpr VectorType operator+(VectorType p_lhs, VectorType const& p_rhs)
	{
		return p_lhs += p_rhs;
	}

	friend constexpr VectorType operator-(VectorType p_lhs, VectorType const& p_rhs)
	{
		return p_lhs -= p_rhs;
	}

	constexpr VectorType& operator*=(float const& p_scalar)
	{
		for (YK_uint32 i = 0; i < DimensionCount; ++i)
		{
			m_data[i] *= p_scalar;
		}
		return *this;
	}

	friend constexpr VectorType operator*(VectorType p_vector, float p_scalar)
	{
		return p_vector *= p_scalar;
	}

	friend constexpr VectorType operator*(float p_scalar, VectorType p_vector)
	{
		return p_vector *= p_scalar;
	}

	constexpr VectorType& operator/=(float const& p_scalar)
	{
		for (YK_uint32 i = 0; i < DimensionCount; ++i)
		{
			m_data[i] /= p_scalar;
		}
		return *this;
	}

	friend constexpr VectorType operator/(VectorType p_vector, float p_scalar)
	{
		return p_vector /= p_scalar;
	}

	friend constexpr VectorType operator/(float p_scalar, VectorType p_vector)
	{
		return p_vector /= p_scalar;
	}

	constexpr DataType& operator[](size_t const p_index) { return m_data[p_index]; }
	constexpr DataType const& operator[](size_t const p_index) const { return m_data[p_index]; }

public:
	constexpr VectorType& Scale(VectorType const& p_other)
	{
		for (YK_uint32 i = 0; i < DimensionCount; ++i)
		{
			m_data[i] *= p_other.m_data[i];
		}
		return *this;
	}

	static constexpr VectorType Scale(VectorType p_lhs, VectorType const& p_rhs)
	{
		return p_lhs.Scale(p_rhs);
	}

	template <typename = typename YK_EnableIf<(DimensionCount == 3)>::Type>
	constexpr VectorType Cross(VectorType const& p_other) const
	{
		VectorType result;
		result.m_data[0] = (m_data[1] * p_other.m_data[2]) - (m_data[2] * p_other.m_data[1]);
		result.m_data[1] = (m_data[2] * p_other.m_data[0]) - (m_data[0] * p_other.m_data[2]);
		result.m_data[2] = (m_data[0] * p_other.m_data[1]) - (m_data[1] * p_other.m_data[0]);
		return result;
	}

	template <typename = typename YK_EnableIf<(DimensionCount == 3)>::Type>
	static constexpr VectorType Cross(VectorType const& p_lhs, VectorType const& p_rhs)
	{
		return p_lhs.Cross(p_rhs);
	}

	constexpr float Dot(VectorType const& p_other) const
	{
		float result = 0;
		for (YK_uint32 i = 0; i < DimensionCount; ++i)
		{
			result += m_data[i] * p_other.m_data[i];
		}
		return result;
	}

	static constexpr float Dot(VectorType const& p_lhs, VectorType const& p_rhs)
	{
		return p_lhs.Dot(p_rhs);
	}

	constexpr float SqrMagnitude() const
	{
		float result = 0;
		for (YK_uint32 i = 0; i < DimensionCount; ++i)
		{
			result += m_data[i] * m_data[i];
		}
		return result;
	}

	/*constexpr*/ float Magnitude() const
	{
		return std::sqrt(SqrMagnitude());
	}

	/*constexpr*/ VectorType GetNormalized() const
	{
		return *this / Magnitude();
	}

	/*constexpr*/ VectorType const& Normalize()
	{
		*this /= Magnitude();
		return *this;
	}

	constexpr DataType* GetData() { return m_data; }
	constexpr DataType const* GetData() const { return m_data; }

private:
	DataType m_data[DimensionCount];
};

#undef APPLY_VECTOR_ACCESSOR

using YK_Vector2f = YK_Vector_N<float, 2>;
using YK_Vector3f = YK_Vector_N<float, 3>;
using YK_Vector4f = YK_Vector_N<float, 4>;

using YK_Vector2i = YK_Vector_N<YK_int32, 2>;
using YK_Vector3i = YK_Vector_N<YK_int32, 3>;
using YK_Vector4i = YK_Vector_N<YK_int32, 4>;