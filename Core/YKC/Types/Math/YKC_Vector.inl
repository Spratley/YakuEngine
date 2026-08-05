#include "YKC/Utils/YKC_AlgorithmUtils.h"

#include <type_traits>

// Free shared vector operators
// Arithmetic
template <YK_VectorType VectorA, YK_VectorType VectorB>
requires(YK_AreVectorsCompatible<VectorA, VectorB>::value)
constexpr YK_Vector_N<typename VectorA::DataType_T, VectorA::Count> operator+(VectorA const& p_lhs,
                                                                              VectorB const& p_rhs)
{
    YK_Vector_N<typename VectorA::DataType_T, VectorA::Count> result;
    for (auto i : YK_CountTo(VectorA::Count))
    {
        result[i] = p_lhs[i] + p_rhs[i];
    }
    return result;
}

template <YK_VectorType VectorA, YK_VectorType VectorB>
requires(YK_AreVectorsCompatible<VectorA, VectorB>::value)
constexpr YK_Vector_N<typename VectorA::DataType_T, VectorA::Count> operator-(VectorA const& p_lhs,
                                                                              VectorB const& p_rhs)
{
    YK_Vector_N<typename VectorA::DataType_T, VectorA::Count> result;
    for (auto i : YK_CountTo(VectorA::Count))
    {
        result[i] = p_lhs[i] - p_rhs[i];
    }
    return result;
}

// Negation
template <YK_VectorType Vector>
constexpr YK_Vector_N<typename Vector::DataType_T, Vector::Count> operator-(Vector const& p_vector)
{
    static_assert(std::is_signed_v<typename Vector::DataType_T>,
                  "Unary negation operator not supported for unsigned types!");
    YK_Vector_N<typename Vector::DataType_T, Vector::Count> result;
    for (auto i : YK_CountTo(Vector::Count))
    {
        result[i] = p_vector[i] * static_cast<typename Vector::DataType_T>(-1);
    }
    return result;
}

template <YK_VectorType VectorA, YK_VectorType VectorB>
requires(YK_AreVectorsCompatible<VectorA, VectorB>::value)
constexpr VectorA& operator+=(VectorA& p_lhs, VectorB const& p_rhs)
{
    for (auto i : YK_CountTo(VectorA::Count))
    {
        p_lhs[i] += p_rhs[i];
    }
    return p_lhs;
}

template <YK_VectorType VectorA, YK_VectorType VectorB>
requires(YK_AreVectorsCompatible<VectorA, VectorB>::value)
constexpr VectorA& operator-=(VectorA& p_lhs, VectorB const& p_rhs)
{
    for (auto i : YK_CountTo(VectorA::Count))
    {
        p_lhs[i] -= p_rhs[i];
    }
    return p_lhs;
}

template <YK_VectorType Vector, typename ScalarType>
constexpr Vector& operator*=(Vector& p_vector, ScalarType const& p_scalar)
{
    for (auto i : YK_CountTo(Vector::Count))
    {
        p_vector[i] *= p_scalar;
    }
    return p_vector;
}

template <YK_VectorType Vector, typename ScalarType>
constexpr YK_Vector_N<typename Vector::DataType_T, Vector::Count> operator*(Vector const& p_vector,
                                                                            ScalarType const& p_scalar)
{
    YK_Vector_N<typename Vector::DataType_T, Vector::Count> result;
    for (auto i : YK_CountTo(Vector::Count))
    {
        result[i] = p_vector[i] * p_scalar;
    }
    return result;
}

template <YK_VectorType Vector, typename ScalarType>
constexpr YK_Vector_N<typename Vector::DataType_T, Vector::Count> operator*(ScalarType const& p_scalar,
                                                                            Vector const& p_vector)
{
    return p_vector * p_scalar;
}

template <YK_VectorType Vector, typename ScalarType>
constexpr Vector& operator/=(Vector& p_vector, ScalarType const& p_scalar)
{
    for (auto i : YK_CountTo(Vector::Count))
    {
        p_vector[i] /= p_scalar;
    }
    return p_vector;
}

template <YK_VectorType Vector, typename ScalarType>
constexpr YK_Vector_N<typename Vector::DataType_T, Vector::Count> operator/(Vector const& p_vector,
                                                                            ScalarType const& p_scalar)
{
    YK_Vector_N<typename Vector::DataType_T, Vector::Count> result;
    for (auto i : YK_CountTo(Vector::Count))
    {
        result[i] = p_vector[i] / p_scalar;
    }
    return result;
}

template <YK_VectorType Vector, typename ScalarType>
constexpr YK_Vector_N<typename Vector::DataType_T, Vector::Count> operator/(ScalarType const& p_scalar,
                                                                            Vector const& p_vector)
{
    return p_vector / p_scalar;
}

// Comparison
template <YK_VectorType VectorA, YK_VectorType VectorB>
requires(YK_AreVectorsCompatible<VectorA, VectorB>::value)
constexpr bool operator==(VectorA const& p_lhs, VectorB const& p_rhs)
{
    for (auto i : YK_CountTo(VectorA::Count))
    {
        if (p_lhs[i] != p_rhs[i])
        {
            return false;
        }
    }
    return true;
}

template <YK_VectorType VectorA, YK_VectorType VectorB>
requires(YK_AreVectorsCompatible<VectorA, VectorB>::value)
constexpr bool operator!=(VectorA const& p_lhs, VectorB const& p_rhs)
{
    return !(p_lhs == p_rhs);
}