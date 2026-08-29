#pragma once

#include "CG/GPU/CG_GPUDataPolicy.h"

#include "YK/Utils/YK_AlgorithmUtils.h"
#include "YK/Utils/YK_TemplateUtils.h"

// TODO: Evaluate if this is the right approach and how well it synergizes with the rest of the engine
// This was an old implementaiton that's rolling over from Yakuman
// Maybe not the right choice here

template <class ResourceType>
class CG_GPUResource
{
public:
    CG_GPUResource() = delete;
    CG_GPUResource(CG_GPUResource<ResourceType> const&) = delete;
    CG_GPUResource(CG_GPUResource<ResourceType>&& p_otherResource) noexcept;

    CG_GPUResource& operator=(CG_GPUResource<ResourceType> const&) = delete;
    CG_GPUResource& operator=(CG_GPUResource<ResourceType>&& p_otherResource) noexcept;

    ~CG_GPUResource() { FlushResources(); }

    inline bool HasData() const { return YK_CRTPCast<ResourceType const>(this)->HasDataImpl(); }
    inline bool HasGPUData() const { return YK_CRTPCast<ResourceType const>(this)->HasGPUDataImpl(); }

    void UploadGPUData();
    void OnDataSet();

protected:
    CG_GPUResource(CG_GPUDataPolicy::GPUDataPolicy p_dataPolicy)
        : m_dataPolicy(p_dataPolicy)
    {}

    inline void FlushResources();
    inline void FlushGPUData() { YK_CRTPCast<ResourceType>(this)->FlushGPUDataImpl(); }
    inline void FlushRAMData() { YK_CRTPCast<ResourceType>(this)->FlushRAMDataImpl(); }

private:
    CG_GPUDataPolicy::GPUDataPolicy m_dataPolicy;
};

template <class ResourceType>
inline CG_GPUResource<ResourceType>::CG_GPUResource(CG_GPUResource<ResourceType>&& p_otherResource) noexcept
    : m_dataPolicy(p_otherResource.m_dataPolicy)
{
    p_otherResource.m_dataPolicy = CG_GPUDataPolicy::INVALID;
}

template <class ResourceType>
inline CG_GPUResource<ResourceType>& CG_GPUResource<ResourceType>::operator=(
  CG_GPUResource<ResourceType>&& p_otherResource) noexcept
{
    m_dataPolicy = YK_Exchange(p_otherResource.m_dataPolicy, CG_GPUDataPolicy::INVALID);
    return *this;
}

template <class ResourceType>
void CG_GPUResource<ResourceType>::UploadGPUData()
{
#if !YAKU_RETAIL
    if (HasGPUData())
    {
        YK_LOG_ERROR("Attempting to re-upload GPU data!");
        return;
    }
#endif // !YAKU_RETAIL

    // TODO: Is it safe to allow uploading nothing? Should this check be in the YAKU_RETAIL above?
    if (!HasData())
    {
        YK_LOG_ERROR("Attempting to upload nothing to the GPU!");
        return;
    }

    YK_CRTPCast<ResourceType>(this)->UploadGPUDataImpl();

    if (CG_GPUDataPolicy::HasDataPolicyFlag(m_dataPolicy, CG_GPUDataPolicy::FREE_RAM_POLICY))
    {
        FlushRAMData();
    }
}

template <class ResourceType>
void CG_GPUResource<ResourceType>::OnDataSet()
{
    if (CG_GPUDataPolicy::HasDataPolicyFlag(m_dataPolicy, CG_GPUDataPolicy::UPLOAD_GPU_DATA_POLICY))
    {
        UploadGPUData();
    }
}

template <class ResourceType>
inline void CG_GPUResource<ResourceType>::FlushResources()
{
    FlushGPUData();
    FlushRAMData();
}