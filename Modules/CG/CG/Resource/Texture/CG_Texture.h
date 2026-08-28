#pragma once

#include "CG/GPU/CG_GPUDataPolicy.h"
#include "CG/GPU/CG_GPUResource.h"

#include "YK/Types/Math/YK_Integer.h"

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
    CG_Texture(CG_Texture&& p_otherTexture) noexcept;
    ~CG_Texture() = default;

    CG_Texture& operator=(CG_Texture&& p_otherTexture) noexcept;

    // Prevent accidental copies
    CG_Texture(CG_Texture const&) = delete;
    CG_Texture& operator=(CG_Texture const&) = delete;

    YK_U8* GetBuffer() const { return m_buffer; }
    YK_U32 GetBufferSize() const { return m_width * m_height * m_nrChannels; } // Should this be cached?

    YK_U16 GetWidth() const { return m_width; }
    YK_U16 GetHeight() const { return m_height; }
    YK_U16 GetNrChannels() const { return m_nrChannels; }

    constexpr float GetAspectRatio() const { return static_cast<float>(m_width) / static_cast<float>(m_height); }

    CG_GLTextureBuffer const& GetGLData() const { return *m_glData; }

private:
    CG_Texture(CG_GPUDataPolicy::GPUDataPolicy p_dataPolicy);

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