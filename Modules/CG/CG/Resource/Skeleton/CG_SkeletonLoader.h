#pragma once

#include "YK/IO/File/YK_FilePath.h"

#include "CG/Resource/Skeleton/CG_Skeleton.h"

class CG_SkeletonLoader
{
public:
    static CG_Skeleton Load(YK_FilePath const& p_skeletonPath);
};