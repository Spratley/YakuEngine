#pragma once

#include "YKC/Types/Math/YKC_Integer.h"
#include "YKC/Types/Traits/YKC_Concepts.h"

#include <concepts>
#include <type_traits>

// A vector type requires:
//  - A nested typename "DataType_T" that is numeric
//  - The ability to be indexed, returning an instance of DataType_T
//  - A static member "Count" that should return the dimension count of the vector
template <typename Vector>
concept YK_VectorType = requires(Vector v) {
    typename Vector::DataType_T;
    requires YK_NumericType<typename Vector::DataType_T>;
    { v[0] } -> std::convertible_to<typename Vector::DataType_T>;
    Vector::Count;
};

// Vectors are compatible for math when:
//  - They have the same count
//  - Their underlying data types (Regardless of CV) match
template <typename VectorA, typename VectorB>
struct YK_AreVectorsCompatible
{
    static constexpr bool value = VectorA::Count == VectorB::Count
                                  && std::is_same_v<typename std::remove_cv_t<typename VectorA::DataType_T>,
                                                    typename std::remove_cv_t<typename VectorB::DataType_T>>;
};

// Represents a vector whose data is stored in an external interleaved array
// Only use if you can guarantee you won't get read errors
// Does not manage its own data, but can mutate it
template <typename DataType, YK_U32 DimensionCount, YK_U32 Offset>
struct YK_VectorView
{
public:
    using DataType_T = DataType;
    static constexpr YK_U32 Count = DimensionCount;

public:
    constexpr YK_VectorView(DataType* p_start)
        : m_start(p_start)
    {}

    constexpr DataType& operator[](size_t const p_index) const { return *(m_start + (p_index * Offset)); }

private:
    DataType* m_start;
};

#pragma warning(push)
#pragma warning(disable : 4201)

template <typename DataType, YK_U32 DimensionCount>
struct YK_Vector_N
{
public:
    using DataType_T = DataType;
    static constexpr YK_U32 Count = DimensionCount;

public:
    static constexpr YK_Vector_N Zero() { return YK_Vector_N(0); }
    static constexpr YK_Vector_N One() { return YK_Vector_N(1); }

    constexpr YK_Vector_N()
        : m_data{}
    {}

    constexpr YK_Vector_N(DataType p_defaultValue)
        : m_data{}
    {
        for (YK_U32 i = 0; i < DimensionCount; ++i)
        {
            m_data[i] = p_defaultValue;
        }
    }

    constexpr YK_Vector_N(YK_Vector_N const& p_other)
        : m_data{}
    {
        for (YK_U32 i = 0; i < DimensionCount; ++i)
        {
            m_data[i] = p_other[i];
        }
    }

    constexpr DataType& operator[](size_t const p_index) { return m_data[p_index]; }
    constexpr DataType const& operator[](size_t const p_index) const { return m_data[p_index]; }

public:
    DataType m_data[DimensionCount];
};

template <typename DataType>
struct YK_Vector_N<DataType, 2>
{
public:
    using DataType_T = DataType;
    static constexpr YK_U32 Count = 2;

public:
    static constexpr inline YK_Vector_N Zero() { return YK_Vector_N(0); }
    static constexpr inline YK_Vector_N One() { return YK_Vector_N(1); }

    constexpr YK_Vector_N()
        : m_data{}
    {}
    constexpr YK_Vector_N(DataType p_defaultValue)
        : m_data{ p_defaultValue, p_defaultValue }
    {}
    constexpr YK_Vector_N(YK_Vector_N const& p_other)
        : m_data{ p_other.m_data[0], p_other.m_data[1] }
    {}
    constexpr YK_Vector_N(DataType p_x, DataType p_y)
        : m_data{ p_x, p_y }
    {}

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

template <typename DataType>
struct YK_Vector_N<DataType, 3>
{
public:
    using DataType_T = DataType;
    static constexpr YK_U32 Count = 3;

public:
    static constexpr inline YK_Vector_N Zero() { return YK_Vector_N(0); }
    static constexpr inline YK_Vector_N One() { return YK_Vector_N(1); }

    // Basis Vectors
    static constexpr YK_Vector_N Right() { return YK_Vector_N{ 1, 0, 0 }; }
    static constexpr YK_Vector_N Up() { return YK_Vector_N{ 0, 1, 0 }; }
    static constexpr YK_Vector_N Forward() { return YK_Vector_N{ 0, 0, 1 }; }

    constexpr YK_Vector_N()
        : m_data{}
    {}
    constexpr YK_Vector_N(DataType p_defaultValue)
        : m_data{ p_defaultValue, p_defaultValue, p_defaultValue }
    {}
    constexpr YK_Vector_N(YK_Vector_N const& p_other)
        : m_data{ p_other.m_data[0], p_other.m_data[1], p_other.m_data[2] }
    {}
    constexpr YK_Vector_N(DataType p_x, DataType p_y, DataType p_z)
        : m_data{ p_x, p_y, p_z }
    {}

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

template <typename DataType>
struct YK_Vector_N<DataType, 4>
{
public:
    using DataType_T = DataType;
    static constexpr YK_U32 Count = 4;

public:
    static constexpr inline YK_Vector_N Zero() { return YK_Vector_N(0); }
    static constexpr inline YK_Vector_N One() { return YK_Vector_N(1); }

    constexpr YK_Vector_N()
        : m_data{}
    {}
    constexpr YK_Vector_N(DataType p_defaultValue)
        : m_data{ p_defaultValue, p_defaultValue, p_defaultValue, p_defaultValue }
    {}
    constexpr YK_Vector_N(YK_Vector_N const& p_other)
        : m_data{ p_other.m_data[0], p_other.m_data[1], p_other.m_data[2], p_other.m_data[3] }
    {}
    constexpr YK_Vector_N(DataType p_x, DataType p_y, DataType p_z, DataType p_w)
        : m_data{ p_x, p_y, p_z, p_w }
    {}

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

using YK_Vector2f = YK_Vector_N<float, 2>;
using YK_Vector3f = YK_Vector_N<float, 3>;
using YK_Vector4f = YK_Vector_N<float, 4>;

using YK_Vector2i = YK_Vector_N<YK_Int32, 2>;
using YK_Vector3i = YK_Vector_N<YK_Int32, 3>;
using YK_Vector4i = YK_Vector_N<YK_Int32, 4>;

#include "YKC_Vector.inl"