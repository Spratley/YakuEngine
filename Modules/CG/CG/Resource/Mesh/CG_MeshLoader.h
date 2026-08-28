#pragma once

#include "CG/Resource/Mesh/CG_Mesh.h"

#include "YK/IO/File/YK_FilePath.h"

struct CG_MeshLoader
{
    static CG_Mesh Load(YK_FilePath const& p_path);
};