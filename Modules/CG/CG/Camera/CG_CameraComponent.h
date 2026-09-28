#pragma once

#include "YK/ECS/Components/YK_TransformComponent.h"
#include "YK/Math/YK_MatrixMath.h"
#include "YK/Types/Math/YK_Matrix.h"

#include "CG/Matrix/CG_MatrixExtras.h"

struct CG_Camera
{
    constexpr YK_Matrix44 CalculateCameraMatrix(float p_aspectRatio) const
    {
        return YK_Matrix::Perspective<float>(m_fov, p_aspectRatio, m_nearPlane, m_farPlane)
               * YK_Matrix::Inverse(YK_Matrix::Construct(m_transform));
    }

    YK_Transform m_transform;
    float m_fov = 60.0f;
    float m_nearPlane = 0.1f;
    float m_farPlane = 100.0f;
};

using CG_CameraComponent = CG_Camera;