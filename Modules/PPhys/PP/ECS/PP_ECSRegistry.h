#pragma once

#include "YK/ECS/YK_ECSRegistry.h"

#include "PP/ECS/PP_RigidBodyComponent.h"

namespace PP_ECSRegistry
{
    using ComponentTypes = Zen::TypeList<PP_RigidBodyComponent>;
    using SystemTypes = Zen::TypeList<>;
} // namespace CG_ECSRegistry

using PP_ComponentTypes = Zen::TypeListUtils::Concat_T<YK_ComponentTypes, PP_ECSRegistry::ComponentTypes>;
using PP_SystemTypes = Zen::TypeListUtils::Concat_T<YK_SystemTypes, PP_ECSRegistry::SystemTypes>;