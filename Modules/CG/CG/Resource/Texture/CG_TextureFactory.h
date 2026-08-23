#pragma once

#include "CG/Resource/Texture/CG_Texture.h"

#include "YK/IO/File/YK_FilePath.h"

class CG_TextureFactory
{
public:
    static void Init();

    static CG_Texture* FromData(YK_U8* p_data, YK_U16 p_width, YK_U16 p_height, YK_U16 p_nrChannels);

    // TODO: Should this be elsewhere?
    // File Loaders
    static CG_Texture* LoadPNG(char const* p_path);
};

class CG_TextureLoader
{
public:
    static CG_Texture Load(YK_FilePath const& p_texturePath)
    {
        CG_Texture* loaded = CG_TextureFactory::LoadPNG(p_texturePath.CString());
        CG_Texture result = std::move(*loaded);
        delete loaded;
        return result;
    }
};