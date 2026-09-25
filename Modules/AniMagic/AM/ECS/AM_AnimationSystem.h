#pragma once

#include "YK/Libraries/Zen/System/Zen_System.h"

#include "CG/ECS/CG_Components.h"

#include "AM/ECS/AM_AnimationComponent.h"

class AM_AnimationSystem : public Zen::SystemBase<AM_AnimationSystem, CG_PoseComponent, AM_AnimationComponent>
{
public:
    static void Tick(ComponentView p_components);
};