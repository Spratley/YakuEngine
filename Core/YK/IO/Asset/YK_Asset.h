#pragma once

#include "YK/IO/Asset/YK_AssetConcepts.h"
#include "YK/IO/Asset/YK_AssetHandle.h"
#include "YK/Types/Math/YK_Integer.h"
#include "YK/Types/Traits/YK_TypeTraits.h"

class YK_Asset
{
public:
    template <typename Self>
    requires(YK_AssetType<YK_StripCVR_T<Self>>)
    YK_AssetHandle<YK_StripCVR_T<Self&&>> GetHandle(this Self&& p_self)
    {
        // TODO: Make thread safe
        using HandleType = YK_AssetHandle<YK_StripCVR_T<Self&&>>;
        ++(p_self.m_referenceCount);
        return HandleType{ &p_self };
    }

    template <typename Self>
    requires(YK_AssetType<YK_StripCVR_T<Self>>)
    void ReturnHandle(this Self&& p_self, YK_AssetHandle<YK_StripCVR_T<Self&&>>& p_handle)
    {
        // TODO: Make thread safe
        using HandleType = YK_AssetHandle<YK_StripCVR_T<Self&&>>;
        --(p_self.m_referenceCount);
        p_handle.m_asset = nullptr;

        if (p_self.m_referenceCount == 0)
        {
            YK_LOG("Reference count for asset just hit 0!");
        }
    }

    YK_SizeT ReferenceCount() const { return m_referenceCount; }

private:
    mutable YK_U32 m_referenceCount = 0;
    mutable YK_U32 m_generation = 0;
};