#pragma once

#include "YKC/ECS/YKC_ECSRegistry.h"

#include "CG/Camera/CG_Camera.h"

namespace CG_ECSRegistry
{
    using ComponentTypes = Zen::TypeList<CG_CameraComponent>;
    using SystemTypes = Zen::TypeList<CG_CameraUpdateSystem>;
} // namespace CG_ECSRegistry

using CG_ComponentTypes = Zen::TypeListUtils::Concat_T<YKC_ComponentTypes, CG_ECSRegistry::ComponentTypes>;
using CG_SystemTypes = Zen::TypeListUtils::Concat_T<YKC_SystemTypes, CG_ECSRegistry::SystemTypes>;