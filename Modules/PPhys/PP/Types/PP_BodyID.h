#pragma once

#include "YK/Math/YK_NumericLimits.h"
#include "YK/Types/Math/YK_Integer.h"

#include <Jolt/Jolt.h>
#include "PP/Libraries/Jolt/Physics/Body/BodyID.h"

struct PP_BodyID
{
    static const PP_BodyID Invalid;

    constexpr PP_BodyID() = default;
    constexpr PP_BodyID(YK_U32 p_value)
        : m_value(p_value)
    {}
    constexpr PP_BodyID(JPH::BodyID const& p_id)
        : m_value(p_id.GetIndexAndSequenceNumber())
    {}

    operator JPH::BodyID() const { return JPH::BodyID{ m_value }; }

    bool IsValid() const { return !JPH::BodyID(*this).IsInvalid(); }

private:
    YK_U32 m_value = YK_NumericLimits<YK_U32>::Max;
};

constexpr inline PP_BodyID PP_BodyID::Invalid{ YK_NumericLimits<YK_U32>::Max };