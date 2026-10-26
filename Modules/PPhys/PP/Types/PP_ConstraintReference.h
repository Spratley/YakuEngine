#pragma once

#include "YK/Types/Traits/YK_TypeTraits.h"
#include "YK/Utils/YK_AlgorithmUtils.h"
#include "YK/Utils/YK_PointerUtils.h"

namespace JPH
{
    class Constraint;
};

struct PP_ConstraintReference
{
public:
    constexpr PP_ConstraintReference() = default;

    PP_ConstraintReference(JPH::Constraint* p_joltConstraint);
    PP_ConstraintReference(PP_ConstraintReference const& p_other);
    PP_ConstraintReference(PP_ConstraintReference&& p_other) noexcept
        : m_constraint(YK_Exchange(p_other.m_constraint, nullptr))
    {}

    ~PP_ConstraintReference();

    PP_ConstraintReference& operator=(JPH::Constraint* p_joltConstraint);
    PP_ConstraintReference& operator=(PP_ConstraintReference const& p_other);
    PP_ConstraintReference& operator=(PP_ConstraintReference&& p_other) noexcept
    {
        m_constraint = YK_Exchange(p_other.m_constraint, nullptr);
        return *this;
    }

    constexpr bool IsValid() { return m_constraint != nullptr; }

    void Release();

    template <typename ConstraintType, typename Self>
    YK_DeducedReturn_T<ConstraintType, Self&&>* GetAs(this Self&& p_self)
    { return YK_SafetyCast<YK_DeducedReturn_T<ConstraintType, Self&&>*>(p_self.m_constraint); }

private:
    JPH::Constraint* m_constraint = nullptr;
};