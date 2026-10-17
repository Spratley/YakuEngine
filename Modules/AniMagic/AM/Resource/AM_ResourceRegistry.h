#pragma once

#include "YK/IO/Asset/YK_AssetHandle.h"
#include "YK/IO/Asset/YK_AssetManager.h"

#include "AM/Resource/AM_BoneMask.h"
#include "AM/Resource/AM_BoneMaskLoader.h"

namespace AM_ResourceRegistry
{
    void RegisterAssetTypes(YK_AssetManager& p_assetManager)
    { p_assetManager.RegisterType<AM_BoneMask, AM_BoneMaskLoader>(); }
} // namespace AM_ResourceRegistry