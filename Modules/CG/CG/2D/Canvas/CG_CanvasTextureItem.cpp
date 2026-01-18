#include "PCH/CG_PCH.h"
#include "CG_CanvasTextureItem.h"

#include "CG/Resource/Texture/CG_TextureFactory.h"

CG_CanvasTextureItem::CG_CanvasTextureItem(const char* p_texturePath)
{
    m_texture = CG_TextureFactory::LoadPNG(p_texturePath);
    m_aspectRatio = static_cast<float>(m_texture->GetWidth()) / static_cast<float>(m_texture->GetHeight());
}

CG_CanvasTextureItem::~CG_CanvasTextureItem()
{
    // This will eventually be removed because resources will be managed externally
    delete m_texture;
}