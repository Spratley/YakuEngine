#pragma once

#include "YK/IO/Asset/YK_Asset.h"
#include "YK/IO/Asset/YK_AssetHandle.h"

#include "CG/Resource/Shader/CG_Shader.h"
#include "CG/Resource/Texture/CG_Texture.h"

class CG_Material : public YK_Asset
{
public:
    YK_AssetHandle<CG_Shader> m_shader;
    YK_AssetHandle<CG_Texture> m_texture;
};