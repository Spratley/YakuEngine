#pragma once

#include "CG/Camera/CG_CameraComponent.h"

#include "YK/ECS/Components/YK_TransformComponent.h"
#include "YK/Libraries/Zen/System/Zen_System.h"
#include "YK/Math/YK_MatrixMath.h"

struct CG_CameraUpdateSystem : public Zen::SystemBase<CG_CameraUpdateSystem, CG_CameraComponent, YK_TransformComponent>
{
    static void Tick(ComponentView const& p_components)
    {
        for (auto [camera, transform] : p_components)
        {
            camera.m_viewMatrix = YK_Matrix::Inverse(
              YK_Matrix::Construct(transform.m_position, transform.m_orientation, transform.m_scale));
        }
    }
};