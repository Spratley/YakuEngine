#include "PCH/CG_PCH.h"
#include "CG_Texture.h"

#include "YK/IO/Asset/YK_Asset.h"
#include "YK/Types/Math/YK_Integer.h"
#include "YK/Utils/YK_AlgorithmUtils.h"
#include "YK/Utils/YK_MemoryUtils.h"

#include "CG/GPU/CG_GPUDataPolicy.h"
#include "CG/GPU/CG_GPUResource.h"
#include "CG/OpenGL/CG_GLTextureBuffer.h"

#include <string.h>
#include <utility>

CG_Texture::CG_Texture()
    : CG_GPUResource<CG_Texture>(CG_GPUDataPolicy::INVALID)
    , YK_Asset()
    , m_glData(nullptr)
    , m_buffer(nullptr)
    , m_width(0)
    , m_height(0)
    , m_nrChannels(0)
{}

CG_Texture::CG_Texture(CG_GPUDataPolicy::GPUDataPolicy p_dataPolicy)
    : CG_GPUResource<CG_Texture>(p_dataPolicy)
    , YK_Asset()
    , m_glData(nullptr)
    , m_buffer(nullptr)
    , m_width(0)
    , m_height(0)
    , m_nrChannels(0)
{}

CG_Texture::CG_Texture(CG_Texture&& p_otherTexture) noexcept
    : CG_GPUResource<CG_Texture>(std::move(p_otherTexture))
    , YK_Asset(std::move(p_otherTexture))
    , m_glData(p_otherTexture.m_glData)
    , m_buffer(p_otherTexture.m_buffer)
    , m_width(p_otherTexture.m_width)
    , m_height(p_otherTexture.m_height)
    , m_nrChannels(p_otherTexture.m_nrChannels)
{
    p_otherTexture.m_glData = nullptr;
    p_otherTexture.m_buffer = nullptr;

    p_otherTexture.m_width = 0;
    p_otherTexture.m_height = 0;
    p_otherTexture.m_nrChannels = 0;
}

CG_Texture& CG_Texture::operator=(CG_Texture&& p_otherTexture) noexcept
{
    if (this == &p_otherTexture)
    {
        return *this;
    }

    FlushResources();

    CG_GPUResource::operator=(std::move(p_otherTexture));
    YK_Asset::operator=(std::move(p_otherTexture));

    m_glData = YK_Exchange(p_otherTexture.m_glData, nullptr);
    m_buffer = YK_Exchange(p_otherTexture.m_buffer, nullptr);
    m_width = YK_Exchange(p_otherTexture.m_width, 0);
    m_height = YK_Exchange(p_otherTexture.m_height, 0);
    m_nrChannels = YK_Exchange(p_otherTexture.m_nrChannels, 0);

    return *this;
}

void CG_Texture::SetData(YK_U8* p_data, YK_U16 p_width, YK_U16 p_height, YK_U16 p_nrChannels)
{
    const int dataCount = p_width * p_height * p_nrChannels;
    m_buffer = new YK_U8[dataCount];
    memcpy(m_buffer, p_data, sizeof(YK_U8) * dataCount);

    m_width = p_width;
    m_height = p_height;
    m_nrChannels = p_nrChannels;

    OnDataSet();
}

void CG_Texture::UploadGPUDataImpl() { m_glData = new CG_GLTextureBuffer(*this); }

void CG_Texture::FlushGPUDataImpl() { YK_SAFE_DELETE(m_glData); }

void CG_Texture::FlushRAMDataImpl() { YK_SAFE_DELETE_ARRAY(m_buffer); }