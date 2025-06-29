#include "PCH/CG_PCH.h"
#include "CG_Texture.h"

#include "CG/OpenGL/CG_GLTextureBuffer.h"

CG_Texture::CG_Texture()
	: CG_GPUResource<CG_Texture>(CG_GPUDataPolicy::INVALID)
	, m_glData(nullptr)
	, m_buffer(nullptr)
	, m_width(0)
	, m_height(0)
	, m_nrChannels(0)
{}

CG_Texture::CG_Texture(CG_GPUDataPolicy::GPUDataPolicy p_dataPolicy)
	: CG_GPUResource<CG_Texture>(p_dataPolicy)
	, m_glData(nullptr)
	, m_buffer(nullptr)
	, m_width(0)
	, m_height(0)
	, m_nrChannels(0)
{}

CG_Texture::CG_Texture(CG_Texture&& p_otherTexture) noexcept
	: CG_GPUResource<CG_Texture>(std::move(p_otherTexture))
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

	YK_STEAL_MEMBER(m_glData, p_otherTexture, nullptr);
	YK_STEAL_MEMBER(m_buffer, p_otherTexture, nullptr);
	YK_STEAL_MEMBER(m_width, p_otherTexture, 0);
	YK_STEAL_MEMBER(m_height, p_otherTexture, 0);
	YK_STEAL_MEMBER(m_nrChannels, p_otherTexture, 0);

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

void CG_Texture::UploadGPUDataImpl()
{
	m_glData = new CG_GLTextureBuffer(*this);
}

void CG_Texture::FlushGPUDataImpl()
{
	YK_SAFE_DELETE(m_glData);
}

void CG_Texture::FlushRAMDataImpl()
{
	YK_SAFE_DELETE_ARRAY(m_buffer);
}