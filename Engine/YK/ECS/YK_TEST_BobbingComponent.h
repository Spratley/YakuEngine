#pragma once

#include "YKC/ECS/YKC_TEMP_TransformComponent.h"
#include "YKC/Libraries/Zen/System/Zen_System.h"
#include "YKC/Time/YKC_Time.h"

#include <cmath>

struct BobbingComponent
{
    float m_phase;
};

struct BobbingSystem : public Zen::SystemBase<BobbingSystem, TransformComponent, BobbingComponent>
{
    static void Tick(ComponentView const& p_components)
    {
        float elapsedTime = YKC_Time::ElapsedTime();
        for (auto [transform, bobbing] : p_components)
        {
            YK_Vector3f position = YK_Matrix::GetPosition(transform.m_transform);
            position.y = (std::sin(elapsedTime * 10.0f + bobbing.m_phase) * 0.25f) - 0.5f;
            YK_Matrix::SetPosition(transform.m_transform, position);
        }
    }
};