#pragma once

class CG_Texture;

struct CG_CanvasTextureItem
{
public:
    CG_CanvasTextureItem(char const* p_texturePath);
    ~CG_CanvasTextureItem();

    CG_Texture const* const GetTexture() const { return m_texture; }
    float GetAspectRatio() const { return m_aspectRatio; }

private:
    // TODO: Switch this for asset references
    CG_Texture* m_texture;
    float m_aspectRatio;
};