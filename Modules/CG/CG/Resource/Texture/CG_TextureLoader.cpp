#include "PCH/CG_PCH.h"
#include "CG_TextureLoader.h"

#include "CG/Libraries/STB Image/stb_image.h"
#include "CG/Resource/Texture/CG_Texture.h"
#include "CG/Resource/Texture/CG_TextureFactory.h"

#include "YK/IO/File/YK_FilePath.h"
#include "YK/IO/Logging/YK_Logger.h"
#include "YK/Types/Math/YK_Integer.h"

void CG_TextureLoader::Init() { stbi_set_flip_vertically_on_load(true); }

CG_Texture CG_TextureLoader::Load(YK_FilePath const& p_path)
{
    int width;
    int height;
    int nrChannels;
    YK_U8* imageData = stbi_load(p_path.CString(), &width, &height, &nrChannels, 0);

#if !YAKU_RETAIL
    if (!imageData)
    {
        YK_LOG_ERROR("Failed to load image:");
        YK_LOG_ERROR(p_path.CString());
        return CG_Texture();
    }
#endif // !YAKU_RETAIL

    CG_Texture texture = CG_TextureFactory::FromData(imageData,
                                                     static_cast<YK_U16>(width),
                                                     static_cast<YK_U16>(height),
                                                     static_cast<YK_U16>(nrChannels));
    stbi_image_free(imageData);
    return texture;
}