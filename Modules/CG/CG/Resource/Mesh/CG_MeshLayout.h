#pragma once

#include "YK/Debugging/YK_Assert.h"
#include "YK/Types/Math/YK_Integer.h"
#include "YK/Utils/YK_AlgorithmUtils.h"

#include <initializer_list>

enum class CG_MeshAttribute : YK_U8
{
    POSITION,
    NORMAL,
    UV,
    JOINT,
    WEIGHT,

    COUNT
};

struct CG_MeshLayout
{
    static constexpr YK_U8 ToBitflag(CG_MeshAttribute p_attribute) { return (1 << static_cast<YK_U8>(p_attribute)); }
    static constexpr YK_U8 AttributeSize(CG_MeshAttribute p_attribute)
    {
        switch (p_attribute)
        {
            case CG_MeshAttribute::POSITION: return 3u;
            case CG_MeshAttribute::NORMAL:   return 3u;
            case CG_MeshAttribute::UV:       return 2u;
            case CG_MeshAttribute::JOINT:    return 4u;
            case CG_MeshAttribute::WEIGHT:   return 4u;
            default:                         YK_ASSERT(false, "AttributeSize() called for invalid attribute!"); return 0u;
        }
    }

public:
    constexpr CG_MeshLayout() = default;
    constexpr CG_MeshLayout(std::initializer_list<CG_MeshAttribute> p_attributes)
    {
        for (CG_MeshAttribute attribute : p_attributes)
        {
            SetEnabled(attribute);
        }
    }

    constexpr bool IsEnabled(CG_MeshAttribute p_attribute) const
    {
        return m_enabledAttributes & ToBitflag(p_attribute);
    }

    constexpr void SetEnabled(CG_MeshAttribute p_attribute)
    {
        m_enabledAttributes = m_enabledAttributes | ToBitflag(p_attribute);
    }

    constexpr YK_U8 CalculateStride() const
    {
        YK_U8 stride = 0;
        for (auto i : YK_CountTo(CG_MeshAttribute::COUNT))
        {
            CG_MeshAttribute const attribute = static_cast<CG_MeshAttribute>(i);
            stride += IsEnabled(attribute) ? AttributeSize(attribute) : 0;
        }
        // sizeof(float) is used here as a stand-in for any 32 bit data
        return stride * sizeof(float);
    }

private:
    YK_U8 m_enabledAttributes = 0;
};