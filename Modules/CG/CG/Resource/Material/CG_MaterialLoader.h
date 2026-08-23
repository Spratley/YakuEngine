#pragma once

#include "CG/Resource/Material/CG_Material.h"

#include "YK/IO/File/YK_FilePath.h"

class CG_MaterialLoader
{
public:
    static CG_Material Load(YK_FilePath const& p_materialPath);
};