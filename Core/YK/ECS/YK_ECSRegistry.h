#pragma once

#include "YK/Libraries/Zen/Utils/Zen_TypeListUtils.h"
#include "YK/Libraries/Zen/Zen_Types.h"

#include "YK/ECS/Components/YK_TransformComponent.h"

using YK_ComponentTypes = Zen::TypeList<YK_TransformComponent>;
using YK_SystemTypes = Zen::TypeList<>;