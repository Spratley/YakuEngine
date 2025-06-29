#pragma once

#include "CG/GPU/CG_GPUResource.h"

// TODO: Decouple textures and OpenGL
struct CG_GLTextureBuffer;

// Represents a texture to be used with the GPU
// Do we need a CPU side texture for CPU sampling?
class CG_Texture : public CG_GPUResource<CG_Texture>
{
	friend class CG_TextureFactory;
	friend class CG_GPUResource<CG_Texture>;
public:
	CG_Texture();
	// TODO: Destructor?

	CG_Texture& operator=(CG_Texture&) = delete;
	CG_Texture& operator=(CG_Texture&& p_otherTexture) noexcept;

	YK_U8* GetBuffer() const { return m_buffer; }
	YK_U32 GetBufferSize() const { return m_width * m_height * m_nrChannels; } // Should this be cached?

	YK_U16 GetWidth() const { return m_width; }
	YK_U16 GetHeight() const { return m_height; }
	YK_U16 GetNrChannels() const { return m_nrChannels; }

	CG_GLTextureBuffer const& GetGLData() const { return *m_glData; }

private:
	CG_Texture(CG_GPUDataPolicy::GPUDataPolicy p_dataPolicy);
	CG_Texture(CG_Texture&& p_otherTexture) noexcept;

	void SetData(YK_U8* p_data, YK_U16 p_width, YK_U16 p_height, YK_U16 p_nrChannels);

	// CG_GPUResource implementations
	bool HasDataImpl() const { return m_buffer; }
	bool HasGPUDataImpl() const { return m_glData; }

	void UploadGPUDataImpl();
	void FlushGPUDataImpl();
	void FlushRAMDataImpl();

private:
	CG_GLTextureBuffer* m_glData;

	YK_U8* m_buffer;

	YK_U16 m_width;
	YK_U16 m_height;
	YK_U16 m_nrChannels;
};