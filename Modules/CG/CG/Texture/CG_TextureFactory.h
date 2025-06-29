#pragma once

#include "CG/Texture/CG_Texture.h"

// TODO: Factory base class for data policy
class CG_TextureFactory
{
public:
	static void Init();

	static CG_Texture* FromData(YK_U8* p_data, YK_U16 p_width, YK_U16 p_height, YK_U16 p_nrChannels);

	// TODO: Should this be elsewhere?
	// File Loaders
	static CG_Texture* LoadPNG(const char* p_path);
};