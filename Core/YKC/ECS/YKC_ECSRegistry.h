#pragma once

#include "../Libraries/Zen/Utils/Zen_TypeListUtils.h"
#include "../Libraries/Zen/Zen_Types.h"

#include "YKC_TEMP_TransformComponent.h"

using YKC_ComponentTypes = Zen::TypeList<TransformComponent, RenderableComponent>;
using YKC_SystemTypes = Zen::TypeList<>;