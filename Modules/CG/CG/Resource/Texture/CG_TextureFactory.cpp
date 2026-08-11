#include "PCH/CG_PCH.h"
#include "CG_TextureFactory.h"

#include "CG/Libraries/STB Image/stb_image.h"

void CG_TextureFactory::Init()
{
	stbi_set_flip_vertically_on_load(true);
}

CG_Texture* CG_TextureFactory::FromData(YK_U8* p_data, YK_U16 p_width, YK_U16 p_height, YK_U16 p_nrChannels)
{
	CG_Texture* texture = new CG_Texture(CG_GPUDataPolicy::FREE_RAM_DO_GPU_UPLOAD);
	texture->SetData(p_data, p_width, p_height, p_nrChannels);
	return texture;
}

CG_Texture* CG_TextureFactory::LoadPNG(char const* p_path)
{
	int width;
	int height;
	int nrChannels;
	YK_U8* imageData = stbi_load(p_path, &width, &height, &nrChannels, 0);

#if !YAKU_RETAIL
	if (!imageData)
	{
		YK_LOG_ERROR("Failed to load image:");
		YK_LOG_ERROR(p_path);
		return nullptr;
	}
#endif // !YAKU_RETAIL

	CG_Texture* texture = FromData(imageData, static_cast<YK_U16>(width), static_cast<YK_U16>(height), static_cast<YK_U16>(nrChannels));
	stbi_image_free(imageData);
	return texture;
}