#pragma once

#include "CG/ECS/CG_ECSRegistry.h"
#include "YKC/ECS/YKC_ECSRegistry.h"

#include "YK_TEST_BobbingComponent.h"

namespace YK_ECSRegistry
{
    using ComponentTypes = Zen::TypeList<BobbingComponent>;
    using SystemTypes = Zen::TypeList<BobbingSystem>;
} // namespace YK_ECSRegistry

using YK_ComponentTypes = Zen::TypeListUtils::Concat_T<YKC_ComponentTypes, CG_ComponentTypes, YK_ECSRegistry::ComponentTypes>;
using YK_SystemTypes = Zen::TypeListUtils::Concat_T<YKC_SystemTypes, CG_SystemTypes, YK_ECSRegistry::SystemTypes>;