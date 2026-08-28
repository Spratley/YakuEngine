#pragma once

#include "CG_CanvasTextureItem.h"

#include "YK/IO/File/YK_FilePath.h"
#include "YK/Math/YK_AngleMath.h"
#include "YK/Types/Math/YK_Matrix.h"
#include "YK/Types/Math/YK_Vector.h"

#include <cmath>
#include <vector>

class CG_Canvas
{
public:
    void Scroll(YK_Vector2f const& p_scroll) { m_scroll += p_scroll; }
    void Spin(float p_angleDegrees) { m_spin += YK_ToRadians(p_angleDegrees); }

    void SetPosition(YK_Vector2f const& p_position) { m_scroll = p_position; }
    void SetRotation(float p_angleDegrees) { m_spin = YK_ToRadians(p_angleDegrees); }

    YK_Matrix44 CalculateMatrix() const
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

    // Temp
    std::vector<CG_CanvasTextureItem> const& GetItems() const { return m_textureItems; }
    void AddItem(YK_FilePath const& p_texturePath) { m_textureItems.emplace_back(p_texturePath); }

private:
    YK_Vector2f m_scroll = YK_Vector2f::Zero();
    float m_spin = 0.0f;

    std::vector<CG_CanvasTextureItem> m_textureItems;
};