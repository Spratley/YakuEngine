#pragma once

#include "CG/ECS/CG_ECSRegistry.h"
#include "YKC/ECS/YKC_ECSRegistry.h"

#include "EN_TEST_BobbingComponent.h"

namespace EN_ECSRegistry
{
    using ComponentTypes = Zen::TypeList<BobbingComponent>;
    using SystemTypes = Zen::TypeList<BobbingSystem>;
} // namespace YK_ECSRegistry

using EN_ComponentTypes = Zen::TypeListUtils::Concat_T<YKC_ComponentTypes, CG_ComponentTypes, EN_ECSRegistry::ComponentTypes>;
using EN_SystemTypes = Zen::TypeListUtils::Concat_T<YKC_SystemTypes, CG_SystemTypes, EN_ECSRegistry::SystemTypes>;