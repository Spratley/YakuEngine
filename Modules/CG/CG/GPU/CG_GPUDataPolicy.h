#pragma once

#include "YK/Types/Math/YK_Integer.h"

namespace CG_GPUDataPolicy
{
    enum GPUDataPolicyType : YK_U8
    {
        FREE_RAM_POLICY = 0b01,
        UPLOAD_GPU_DATA_POLICY = 0b10
    };

    enum GPUDataPolicy : YK_U8
    {
        KEEP_RAM_NO_GPU_UPLOAD = 0b00,
        FREE_RAM_NO_GPU_UPLOAD = FREE_RAM_POLICY,
        KEEP_RAM_DO_GPU_UPLOAD = UPLOAD_GPU_DATA_POLICY,
        FREE_RAM_DO_GPU_UPLOAD = FREE_RAM_POLICY | UPLOAD_GPU_DATA_POLICY,

        INVALID = 0xFF
    };

    constexpr inline bool HasDataPolicyFlag(GPUDataPolicy policy, GPUDataPolicyType flag)
    {
        return static_cast<YK_U8>(policy) & static_cast<YK_U8>(flag);
    }
} // namespace CG_GPUDataPolicy