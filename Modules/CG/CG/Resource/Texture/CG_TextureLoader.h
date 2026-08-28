#pragma once

#include "CG/Resource/Texture/CG_Texture.h"

#include "YK/IO/File/YK_FilePath.h"

class CG_TextureLoader
{
public:
    static void Init();
    static CG_Texture Load(YK_FilePath const& p_texturePath);
};