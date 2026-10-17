#pragma once

#include "YK/Types/Traits/YK_Concepts.h"

#include <type_traits>

class YK_Asset;

template <typename AssetType>
concept YK_AssetType = YK_NonCVRType<AssetType> && std::is_base_of_v<YK_Asset, AssetType>;
