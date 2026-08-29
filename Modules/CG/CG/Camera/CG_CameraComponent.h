#pragma once

#include "CG/Matrix/CG_MatrixExtras.h"

#include "YK/Types/Math/YK_Matrix.h"

struct CG_CameraComponent
{
    YK_Matrix44 m_viewMatrix;
    float m_fov;
    float m_nearPlane;
    float m_farPlane;

public:
    // Yeah I know this aint true ECS anymore, do I care though?
    constexpr YK_Matrix44 CalculateCameraMatrix(float p_aspectRatio) const
    {
        return YK_Matrix::Perspective<float>(m_fov, p_aspectRatio, m_nearPlane, m_farPlane) * m_viewMatrix;
    }
};
