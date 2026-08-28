#pragma once

#include "CG/Resource/Texture/CG_Texture.h"

#include "YK/Types/Math/YK_Integer.h"

class CG_TextureFactory
{
public:
    static CG_Texture FromData(YK_U8* p_data, YK_U16 p_width, YK_U16 p_height, YK_U16 p_nrChannels);
};