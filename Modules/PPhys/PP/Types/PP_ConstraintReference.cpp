#include "PCH/PPhys_PCH.h"
#include "PP_ConstraintReference.h"

#include "YK/Utils/YK_PointerUtils.h"

#include <Jolt/Jolt.h>
#include "Jolt/Physics/Constraints/Constraint.h"

PP_ConstraintReference::PP_ConstraintReference(JPH::Constraint* p_joltConstraint)
    : m_constraint(p_joltConstraint)
{ m_constraint->AddRef(); }

PP_ConstraintReference::PP_ConstraintReference(PP_ConstraintReference const& p_other)
    : m_constraint(p_other.m_constraint)
{
    if (IsValid())
    {
        m_constraint->AddRef();
    }
}

PP_ConstraintReference::~PP_ConstraintReference() { Release(); }

PP_ConstraintReference& PP_ConstraintReference::operator=(JPH::Constraint* p_joltConstraint)
{
    m_constraint = p_joltConstraint;
    if (IsValid())
    {
        m_constraint->AddRef();
    }
    return *this;
}

PP_ConstraintReference& PP_ConstraintReference::operator=(PP_ConstraintReference const& p_other)
{
    m_constraint = p_other.m_constraint;
    if (IsValid())
    {
        m_constraint->AddRef();
    }
    return *this;
}

void PP_ConstraintReference::Release()
{
    if (!m_constraint)
    {
        return;
    }

    m_constraint->Release();
    m_constraint = nullptr;
}
