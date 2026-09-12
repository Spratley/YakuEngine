#include "PCH/CG_PCH.h"
#include "CG_Canvas.h"

#include "YK/Math/YK_AngleMath.h"
#include "YK/Types/Math/YK_Matrix.h"

YK_Matrix44 CG_Canvas::CalculateMatrix() const
{
    float const cosAngle = YK_Cosine(m_spin);
    float const sinAngle = YK_Sine(m_spin);
    YK_Matrix44 result = {};
    result[0][0] = cosAngle;
    result[0][1] = -sinAngle;
    result[1][0] = sinAngle;
    result[1][1] = cosAngle;
    result[3][0] = m_scroll.x;
    result[3][1] = m_scroll.y;
    return result;
}