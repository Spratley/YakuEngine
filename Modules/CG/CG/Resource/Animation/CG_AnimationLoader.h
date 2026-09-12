#pragma once

#include "YK/IO/File/YK_FilePath.h"

#include "CG/Resource/Animation/CG_Animation.h"

class CG_AnimationLoader
{
public:
    static CG_Animation Load(YK_FilePath const& p_animationPath);
};