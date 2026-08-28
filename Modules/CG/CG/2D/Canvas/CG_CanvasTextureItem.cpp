#include "PCH/CG_PCH.h"
#include "CG_CanvasTextureItem.h"

#include "CG/Resource/Texture/CG_Texture.h"

#include "YK/Core/YK_Core.h"
#include "YK/IO/File/YK_FilePath.h"

CG_CanvasTextureItem::CG_CanvasTextureItem(YK_FilePath const& p_texturePath)
    : m_texture(YK_Core::GetEngine().GetAssetManager().GetAsset<CG_Texture>(p_texturePath))
{}