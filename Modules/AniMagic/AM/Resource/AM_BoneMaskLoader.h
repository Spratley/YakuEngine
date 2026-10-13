#pragma once

#include "YK/IO/File/YK_FilePath.h"

#include "AM/Resource/AM_BoneMask.h"

class AM_BoneMaskLoader
{
public:
    static AM_BoneMask Load(YK_FilePath const& p_poseMaskPath);
};