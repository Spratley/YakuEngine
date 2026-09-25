#pragma once

#include "YK/ECS/YK_ECSRegistry.h"

#include "CG/ECS/CG_ECSRegistry.h"

#include "AM/ECS/AM_AnimationComponent.h"
#include "AM/ECS/AM_AnimationSystem.h"

namespace AM_ECSRegistry
{
    using ComponentTypes = Zen::TypeList<AM_AnimationComponent>;
    using SystemTypes = Zen::TypeList<AM_AnimationSystem>;
} // namespace AM_ECSRegistry

using AM_ComponentTypes =
  Zen::TypeListUtils::Concat_T<YK_ComponentTypes, CG_ComponentTypes, AM_ECSRegistry::ComponentTypes>;
using AM_SystemTypes = Zen::TypeListUtils::Concat_T<YK_SystemTypes, CG_SystemTypes, AM_ECSRegistry::SystemTypes>;