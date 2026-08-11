#pragma once

#include "CG/Resource/Texture/CG_Texture.h"

class CG_TextureFactory
{
public:
	static void Init();

	static CG_Texture* FromData(YK_U8* p_data, YK_U16 p_width, YK_U16 p_height, YK_U16 p_nrChannels);

	// TODO: Should this be elsewhere?
	// File Loaders
	static CG_Texture* LoadPNG(char const* p_path);
};