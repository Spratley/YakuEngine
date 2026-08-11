#pragma once

#include "YK/Types/Math/YK_Integer.h"

namespace YK_Hash
{
    constexpr inline YK_U32 FNV32_Base = 0x811C9DC5;
    constexpr inline YK_U32 FNV32_Prime = 0x1000193;

    constexpr inline YK_U64 FNV64_Base = 0xCBF29CE484222325;
    constexpr inline YK_U64 FNV64_Prime = 0x100000001B3;

    constexpr YK_U32 FNV_1A_32(char const* p_str)
    {
        YK_U32 hash = FNV32_Base;
        while (*p_str)
        {
            hash ^= static_cast<YK_U32>(*p_str++);
            hash *= FNV32_Prime;
        }
        return hash;
    }

    constexpr YK_U64 FNV_1A_64(char const* p_str)
    {
        YK_U64 hash = FNV64_Base;
        while (*p_str)
        {
            hash ^= static_cast<YK_U64>(*p_str++);
            hash *= FNV64_Prime;
        }
        return hash;
    }
}; // namespace YK_Hash