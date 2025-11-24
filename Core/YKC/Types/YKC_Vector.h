#pragma once

#include <cstring>

// TODO: Merge with YK_Vector_N somehow so it can be used interchangibly and seamlessly with all vector operators and
// math functions
// -> Ideally YK_Vector_N and YK_Vector_View should both be able to call anything in any combination of the two so long
// as they share a length
// -> YK_Vector_N.Dot(YK_VectorView) and YK_Vector_N.Dot(YK_Vector_N) should both be valid ways to call

// Represents a vector whose data is stored in an external interleaved array
// Only use if you can guarantee you won't get read errors
// Does not manage its own data, but can mutate the
template<typename DataType, YK_U32 DimensionCount, YK_U32 Offset>
struct YK_VectorView
{
public:
    constexpr YK_VectorView(DataType* p_start) : m_start(p_start) {}

    constexpr DataType& operator[](size_t const p_index) { return *(m_start + (p_index * Offset)); }
    constexpr DataType const& operator[](size_t const p_index) const { return *(m_start + (p_index * Offset)); }

private:
    DataType* m_start;
};

template<typename DataType, int DimensionCount>
struct YK_Vector_N
{
private:
    using VectorType = YK_Vector_N<DataType, DimensionCount>;

public:
    static constexpr inline VectorType Zero() { return VectorType(0); }
    static constexpr inline VectorType One() { return VectorType(1); }

    constexpr YK_Vector_N() : m_data {} {}
    constexpr YK_Vector_N(DataType p_defaultValue)
    {
        for (YK_U32 i = 0; i < DimensionCount; ++i)
        {
            m_data[i] = p_defaultValue;
        }
    }
    YK_Vector_N(VectorType const& p_other) { std::memcpy(m_data, p_other.m_data, sizeof(DataType) * DimensionCount); }

    constexpr DataType& operator[](size_t const p_index) { return m_data[p_index]; }
    constexpr DataType const& operator[](size_t const p_index) const { return m_data[p_index]; }

public:
    DataType m_data[DimensionCount];
};

// TODO: Move this to CMake so the entire project doesn't have to worry about 4201 (Ayo why is that warning even a
// thing)
#pragma warning(push)
#pragma warning(disable : 4201)

template<typename DataType>
struct YK_Vector_N<DataType, 2>
{
private:
    using VectorType = YK_Vector_N<DataType, 2>;

public:
    static constexpr inline VectorType Zero() { return VectorType(0); }
    static constexpr inline VectorType One() { return VectorType(1); }

    constexpr YK_Vector_N() : m_data {} {}
    constexpr YK_Vector_N(DataType p_defaultValue) : m_data {p_defaultValue, p_defaultValue} {}
    constexpr YK_Vector_N(VectorType const& p_other) : m_data {p_other.x, p_other.y} {}
    constexpr YK_Vector_N(DataType p_x, DataType p_y) : m_data {p_x, p_y} {}

    constexpr DataType& operator[](size_t const p_index) { return m_data[p_index]; }
    constexpr DataType const& operator[](size_t const p_index) const { return m_data[p_index]; }

public:
    union
    {
        DataType m_data[2];
        struct
        {
            DataType x, y;
        };
        struct
        {
            DataType r, g;
        };
    };
};

template<typename DataType>
struct YK_Vector_N<DataType, 3>
{
private:
    using VectorType = YK_Vector_N<DataType, 3>;

public:
    static constexpr inline VectorType Zero() { return VectorType(0); }
    static constexpr inline VectorType One() { return VectorType(1); }

    constexpr YK_Vector_N() : m_data {} {}
    constexpr YK_Vector_N(DataType p_defaultValue) : m_data {p_defaultValue, p_defaultValue, p_defaultValue} {}
    constexpr YK_Vector_N(VectorType const& p_other) : m_data {p_other.x, p_other.y, p_other.z} {}
    constexpr YK_Vector_N(DataType p_x, DataType p_y, DataType p_z) : m_data {p_x, p_y, p_z} {}

    constexpr DataType& operator[](size_t const p_index) { return m_data[p_index]; }
    constexpr DataType const& operator[](size_t const p_index) const { return m_data[p_index]; }

public:
    union
    {
        DataType m_data[3];
        struct
        {
            DataType x, y, z;
        };
        struct
        {
            DataType r, g, b;
        };
        YK_Vector_N<DataType, 2> xy;
        YK_Vector_N<DataType, 2> rg;
    };
};

template<typename DataType>
struct YK_Vector_N<DataType, 4>
{
private:
    using VectorType = YK_Vector_N<DataType, 4>;

public:
    static constexpr inline VectorType Zero() { return VectorType(0); }
    static constexpr inline VectorType One() { return VectorType(1); }

    constexpr YK_Vector_N() : m_data {} {}
    constexpr YK_Vector_N(DataType p_defaultValue)
        : m_data {p_defaultValue, p_defaultValue, p_defaultValue, p_defaultValue}
    {}
    constexpr YK_Vector_N(VectorType const& p_other) : m_data {p_other.x, p_other.y, p_other.z, p_other.w} {}
    constexpr YK_Vector_N(DataType p_x, DataType p_y, DataType p_z, DataType p_w) : m_data {p_x, p_y, p_z, p_w} {}

    constexpr DataType& operator[](size_t const p_index) { return m_data[p_index]; }
    constexpr DataType const& operator[](size_t const p_index) const { return m_data[p_index]; }

public:
    union
    {
        DataType m_data[4];
        struct
        {
            DataType x, y, z, w;
        };
        struct
        {
            DataType r, g, b, a;
        };
        YK_Vector_N<DataType, 2> xy;
        YK_Vector_N<DataType, 2> rg;
        YK_Vector_N<DataType, 3> xyz;
        YK_Vector_N<DataType, 3> rgb;
    };
};

#pragma warning(pop)

// Free shared vector operators
template<typename DataType, int DimensionCount>
constexpr YK_Vector_N<DataType, DimensionCount>
  operator+(YK_Vector_N<DataType, DimensionCount> p_lhs, YK_Vector_N<DataType, DimensionCount> const& p_rhs)
{
    return p_lhs += p_rhs;
}

template<typename DataType, int DimensionCount>
constexpr YK_Vector_N<DataType, DimensionCount>
  operator-(YK_Vector_N<DataType, DimensionCount> p_lhs, YK_Vector_N<DataType, DimensionCount> const& p_rhs)
{
    return p_lhs -= p_rhs;
}

template<typename DataType, int DimensionCount>
constexpr YK_Vector_N<DataType, DimensionCount>&
  operator+=(YK_Vector_N<DataType, DimensionCount>& p_lhs, YK_Vector_N<DataType, DimensionCount> const& p_rhs)
{
    for (YK_U32 i = 0; i < DimensionCount; ++i)
    {
        p_lhs.m_data[i] += p_rhs.m_data[i];
    }
    return p_lhs;
}

template<typename DataType, int DimensionCount>
constexpr YK_Vector_N<DataType, DimensionCount>&
  operator-=(YK_Vector_N<DataType, DimensionCount>& p_lhs, YK_Vector_N<DataType, DimensionCount> const& p_rhs)
{
    for (YK_U32 i = 0; i < DimensionCount; ++i)
    {
        p_lhs.m_data[i] -= p_rhs.m_data[i];
    }
    return p_lhs;
}

template<typename DataType, int DimensionCount>
constexpr YK_Vector_N<DataType, DimensionCount>&
  operator*=(YK_Vector_N<DataType, DimensionCount>& p_vector, float const& p_scalar)
{
    for (YK_U32 i = 0; i < DimensionCount; ++i)
    {
        p_vector.m_data[i] *= p_scalar;
    }
    return p_vector;
}

template<typename DataType, int DimensionCount>
constexpr YK_Vector_N<DataType, DimensionCount>
  operator*(YK_Vector_N<DataType, DimensionCount> p_vector, float p_scalar)
{
    return p_vector *= p_scalar;
}

template<typename DataType, int DimensionCount>
constexpr YK_Vector_N<DataType, DimensionCount>
  operator*(float p_scalar, YK_Vector_N<DataType, DimensionCount> p_vector)
{
    return p_vector *= p_scalar;
}

template<typename DataType, int DimensionCount>
constexpr YK_Vector_N<DataType, DimensionCount>&
  operator/=(YK_Vector_N<DataType, DimensionCount>& p_vector, float const& p_scalar)
{
    for (YK_U32 i = 0; i < DimensionCount; ++i)
    {
        p_vector.m_data[i] /= p_scalar;
    }
    return *this;
}

template<typename DataType, int DimensionCount>
constexpr YK_Vector_N<DataType, DimensionCount>
  operator/(YK_Vector_N<DataType, DimensionCount> p_vector, float p_scalar)
{
    return p_vector /= p_scalar;
}

template<typename DataType, int DimensionCount>
constexpr YK_Vector_N<DataType, DimensionCount>
  operator/(float p_scalar, YK_Vector_N<DataType, DimensionCount> p_vector)
{
    return p_vector /= p_scalar;
}

// Free vector manipulation functions
namespace YK_Vector
{
    template<typename DataType, int DimensionCount>
    static constexpr YK_Vector_N<DataType, DimensionCount> Scale(YK_Vector_N<DataType, DimensionCount> p_lhs,
                                                                 YK_Vector_N<DataType, DimensionCount> const& p_rhs)
    {
        for (YK_U32 i = 0; i < DimensionCount; ++i)
        {
            p_lhs.m_data[i] *= p_rhs.m_data[i];
        }
        return p_lhs;
    }

    // Vector3 specific
    template<typename DataType>
    constexpr YK_Vector_N<DataType, 3> Cross(YK_Vector_N<DataType, 3> const& p_lhs,
                                             YK_Vector_N<DataType, 3> const& p_rhs)
    {
        return YK_Vector_N<DataType, 3>((p_lhs.m_data[1] * p_rhs.m_data[2]) - (p_lhs.m_data[2] * p_rhs.m_data[1]),
                                        (p_lhs.m_data[2] * p_rhs.m_data[0]) - (p_lhs.m_data[0] * p_rhs.m_data[2]),
                                        (p_lhs.m_data[0] * p_rhs.m_data[1]) - (p_lhs.m_data[1] * p_rhs.m_data[0]));
    }

    template<typename DataType, int DimensionCount>
    constexpr float Dot(YK_Vector_N<DataType, DimensionCount> const& p_lhs,
                        YK_Vector_N<DataType, DimensionCount> const& p_rhs)
    {
        float result = 0;
        for (YK_U32 i = 0; i < DimensionCount; ++i)
        {
            result += p_lhs.m_data[i] * p_rhs.m_data[i];
        }
        return result;
    }

    // Temp until I can figure out how to make this a single unified function
    // Dot against a VectorView
    template<typename DataType, int DimensionCount, YK_U32 ViewOffset>
    constexpr float Dot(YK_Vector_N<DataType, DimensionCount> const& p_vector,
                        YK_VectorView<DataType const, DimensionCount, ViewOffset> const& p_vectorView)
    {
        float result = 0;
        for (YK_U32 i = 0; i < DimensionCount; ++i)
        {
            result += p_vector[i] * p_vectorView[i];
        }
        return result;
    }

    template<typename DataType, int DimensionCount>
    constexpr float SqrMagnitude(YK_Vector_N<DataType, DimensionCount> const& p_vector)
    {
        float result = 0;
        for (YK_U32 i = 0; i < DimensionCount; ++i)
        {
            result += p_vector.m_data[i] * p_vector.m_data[i];
        }
        return result;
    }

    template<typename DataType, int DimensionCount>
    /*constexpr*/ float Magnitude(YK_Vector_N<DataType, DimensionCount> const& p_vector)
    {
        return std::sqrt(SqrMagnitude(p_vector));
    }

    template<typename DataType, int DimensionCount>
    /*constexpr*/ YK_Vector_N<DataType, DimensionCount> GetNormalized(
      YK_Vector_N<DataType, DimensionCount> const& p_vector)
    {
        return p_vector / Magnitude(p_vector);
    }

    template<typename DataType, int DimensionCount>
    /*constexpr*/ YK_Vector_N<DataType, DimensionCount>& Normalize(YK_Vector_N<DataType, DimensionCount>& p_vector)
    {
        p_vector /= Magnitude(p_vector);
        return p_vector;
    }
} // namespace YK_Vector

using YK_Vector2f = YK_Vector_N<float, 2>;
using YK_Vector3f = YK_Vector_N<float, 3>;
using YK_Vector4f = YK_Vector_N<float, 4>;

using YK_Vector2i = YK_Vector_N<YK_Int32, 2>;
using YK_Vector3i = YK_Vector_N<YK_Int32, 3>;
using YK_Vector4i = YK_Vector_N<YK_Int32, 4>;