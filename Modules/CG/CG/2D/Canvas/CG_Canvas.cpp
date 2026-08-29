#include "PCH/CG_PCH.h"
#include "CG_Canvas.h"

#include <cmath>

YK_Matrix44 CG_Canvas::CalculateMatrix() const
{
    float const cosAngle = std::cos(m_spin);
    float const sinAngle = std::sin(m_spin);
    YK_Matrix44 result = {};
    result[0][0] = cosAngle;
    result[0][1] = -sinAngle;
    result[1][0] = sinAngle;
    result[1][1] = cosAngle;
    result[3][0] = m_scroll.x;
    result[3][1] = m_scroll.y;
    return result;
}