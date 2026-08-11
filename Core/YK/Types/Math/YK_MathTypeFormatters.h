#pragma once

#include "YK/Types/Math/YK_Integer.h"
#include "YK/Types/Math/YK_Vector.h"
#include "YK/Utils/YK_AlgorithmUtils.h"

#include <format>

template <typename DataType, YK_U32 DimensionCount>
struct std::formatter<YK_Vector_N<DataType, DimensionCount>>
{
    constexpr auto parse(std::format_parse_context& p_context) { return p_context.begin(); }
    std::format_context::iterator format(YK_Vector_N<DataType, DimensionCount> const& p_vector,
                                         std::format_context& p_context) const
    {
        auto out = p_context.out();
        out = std::format_to(out, "(");
        for (auto i : YK_CountTo(DimensionCount))
        {
            if (i > 0)
            {
                out = std::format_to(out, ", ");
            }
            out = std::format_to(out, "{}", p_vector[i]);
        }
        out = std::format_to(out, ")");

        return out;
    }
};
