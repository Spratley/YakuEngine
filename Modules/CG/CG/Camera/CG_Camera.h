#pragma once

#include "YKC/ECS/YKC_TEMP_TransformComponent.h"
#include "YKC/Libraries/Zen/System/Zen_System.h"
#include "YKC/Types/YKC_Matrix.h"

struct CG_CameraComponent
{
    YK_Matrix44 m_viewMatrix;
};

struct CG_CameraUpdateSystem : public Zen::SystemBase<CG_CameraUpdateSystem, CG_CameraComponent, TransformComponent>
{
    static void Tick(ComponentView const& p_components)
    {
        for (auto [camera, transform] : p_components) 
        {
            camera.m_viewMatrix = YK_Matrix::Inverse(YK_Matrix::Construct(transform.m_position, transform.m_orientation, transform.m_scale));
        }
    }
};