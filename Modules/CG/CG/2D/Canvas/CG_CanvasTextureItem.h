#pragma once

#include "CG/Resource/Texture/CG_Texture.h"

#include "YK/IO/File/YK_FilePath.h"

struct CG_CanvasTextureItem
{
public:
    CG_CanvasTextureItem(YK_FilePath const& p_texturePath);
    ~CG_CanvasTextureItem() = default;

    CG_Texture const& GetTexture() const { return m_texture; }
    float GetAspectRatio() const { return m_texture.GetAspectRatio(); }

private:
    CG_Texture const& m_texture;
};