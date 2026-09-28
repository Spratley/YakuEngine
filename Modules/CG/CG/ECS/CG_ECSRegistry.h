#pragma once

#include "YK/ECS/YK_ECSRegistry.h"

#include "CG/Camera/CG_CameraComponent.h"
#include "CG/ECS/CG_Components.h"

namespace CG_ECSRegistry
{
    using ComponentTypes = Zen::TypeList<CG_CameraComponent, CG_MeshComponent, CG_RendererComponent>;
    using SystemTypes = Zen::TypeList<>;
} // namespace CG_ECSRegistry

using CG_ComponentTypes = Zen::TypeListUtils::Concat_T<YK_ComponentTypes, CG_ECSRegistry::ComponentTypes>;
using CG_SystemTypes = Zen::TypeListUtils::Concat_T<YK_SystemTypes, CG_ECSRegistry::SystemTypes>;