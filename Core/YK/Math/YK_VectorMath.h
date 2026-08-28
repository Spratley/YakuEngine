#pragma once

#include "YK/Types/Math/YK_Vector.h"
#include "YK/Utils/YK_AlgorithmUtils.h"

#include <cmath>

namespace YK_Vector
{
    template <YK_VectorType Vector>
    constexpr float SqrMagnitude(Vector const& p_vector)
    {
        float result = 0;
        for (auto i : YK_CountTo(Vector::Count))
        {
            result += p_vector.m_data[i] * p_vector.m_data[i];
        }
        return result;
    }

    template <YK_VectorType Vector>
    /*constexpr*/ float Magnitude(Vector const& p_vector)
    {
        return std::sqrt(SqrMagnitude(p_vector));
    }

    template <YK_VectorType Vector>
    /*constexpr*/ YK_Vector_N<typename Vector::DataType_T, Vector::Count> GetNormalized(Vector const& p_vector)
    {
        return p_vector / Magnitude(p_vector);
    }

    template <YK_VectorType Vector>
    /*constexpr*/ Vector& Normalize(Vector& p_vector)
    {
        p_vector /= Magnitude(p_vector);
        return p_vector;
    }

    template <YK_VectorType Vector>
    /*constexpr*/ typename Vector::DataType_T NormalizeAndGetMagnitude(Vector& p_vector)
    {
        float const magnitude = Magnitude(p_vector);
        p_vector /= magnitude;
        return magnitude;
    }

    template <YK_VectorType VectorA, YK_VectorType VectorB>
    requires(YK_AreVectorsCompatible<VectorA, VectorB>::value)
    constexpr YK_Vector_N<typename VectorA::DataType_T, VectorA::Count> Scale(VectorA const& p_lhs, VectorB const& p_rhs)
    {
        YK_Vector_N<typename VectorA::DataType_T, VectorA::Count> result;
        for (auto i : YK_CountTo(VectorA::Count))
        {
            result[i] = p_lhs[i] * p_rhs[i];
        }
        return result;
    }

    template <YK_VectorType VectorA, YK_VectorType VectorB>
    requires(YK_AreVectorsCompatible<VectorA, VectorB>::value)
    constexpr VectorA::DataType_T Dot(VectorA const& p_lhs, VectorB const& p_rhs)
    {
        using DataType = typename VectorA::DataType_T;
        DataType result = static_cast<DataType>(0);
        for (auto i : YK_CountTo(VectorA::Count))
        {
            result += p_lhs[i] * p_rhs[i];
        }
        return result;
    }

    template <YK_VectorType VectorA, YK_VectorType VectorB>
    requires(YK_AreVectorsCompatible<VectorA, VectorB>::value && VectorA::Count == 3)
    constexpr YK_Vector_N<typename VectorA::DataType_T, VectorA::Count> Cross(VectorA const& p_lhs,
                                                                              VectorB const& p_rhs)
    {
        return YK_Vector_N<typename VectorA::DataType_T, VectorA::Count>((p_lhs[1] * p_rhs[2]) - (p_lhs[2] * p_rhs[1]),
                                                                         (p_lhs[2] * p_rhs[0]) - (p_lhs[0] * p_rhs[2]),
                                                                         (p_lhs[0] * p_rhs[1]) - (p_lhs[1] * p_rhs[0]));
    }
} // namespace YK_Vector