#pragma once

#include "CG/Resource/Shader/CG_Shader.h"

#include "YK/IO/File/YK_FilePath.h"

class CG_ShaderLoader
{
public:
    static CG_Shader Load(YK_FilePath const& p_shaderPath);
};