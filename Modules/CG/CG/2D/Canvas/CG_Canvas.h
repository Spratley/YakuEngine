#pragma once

#include "CG_CanvasTextureItem.h"

class CG_Canvas
{
public:
    void Scroll(YK_Vector2f const& p_scroll);
    void Spin(float p_angleDegrees);

    void SetPosition(YK_Vector2f const& p_position);
    void SetRotation(float p_angleDegrees);

    std::vector<CG_CanvasTextureItem> const& GetItems() const { return m_textureItems; }

private:
    YK_Matrix44 m_transform;

    // TODO: Come back to this, is there a better way to store canvas items?
    std::vector<CG_CanvasTextureItem> m_textureItems;
};