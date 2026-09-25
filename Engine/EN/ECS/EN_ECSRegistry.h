#pragma once

#include "AM/ECS/AM_ECSRegistry.h"
#include "CG/ECS/CG_ECSRegistry.h"
#include "PP/ECS/PP_ECSRegistry.h"
#include "YK/ECS/YK_ECSRegistry.h"

namespace EN_ECSRegistry
{
    using ComponentTypes = Zen::TypeList<>;
    using SystemTypes = Zen::TypeList<>;
} // namespace EN_ECSRegistry

using EN_ComponentTypes =
  Zen::TypeListUtils::Concat_T<YK_ComponentTypes, CG_ComponentTypes, PP_ComponentTypes, AM_ComponentTypes, EN_ECSRegistry::ComponentTypes>;
using EN_SystemTypes =
  Zen::TypeListUtils::Concat_T<YK_SystemTypes, CG_SystemTypes, PP_SystemTypes, AM_SystemTypes, EN_ECSRegistry::SystemTypes>;