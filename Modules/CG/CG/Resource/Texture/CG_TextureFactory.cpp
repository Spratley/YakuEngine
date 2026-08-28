#include "PCH/CG_PCH.h"
#include "CG_TextureFactory.h"

#include "CG/GPU/CG_GPUDataPolicy.h"
#include "CG_Texture.h"

#include "YK/Types/Math/YK_Integer.h"

CG_Texture CG_TextureFactory::FromData(YK_U8* p_data, YK_U16 p_width, YK_U16 p_height, YK_U16 p_nrChannels)
{
    CG_Texture texture(CG_GPUDataPolicy::FREE_RAM_DO_GPU_UPLOAD);
    texture.SetData(p_data, p_width, p_height, p_nrChannels);
    return texture;
}