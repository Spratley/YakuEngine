#pragma once

#include "CG/ECS/CG_ECSRegistry.h"
#include "YK/ECS/YK_ECSRegistry.h"

namespace EN_ECSRegistry
{
    using ComponentTypes = Zen::TypeList<>;
    using SystemTypes = Zen::TypeList<>;
} // namespace YK_ECSRegistry

using EN_ComponentTypes = Zen::TypeListUtils::Concat_T<YK_ComponentTypes, CG_ComponentTypes, EN_ECSRegistry::ComponentTypes>;
using EN_SystemTypes = Zen::TypeListUtils::Concat_T<YK_SystemTypes, CG_SystemTypes, EN_ECSRegistry::SystemTypes>;