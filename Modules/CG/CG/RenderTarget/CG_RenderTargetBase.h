#pragma once

#include "YK/Types/Math/YK_Integer.h"
#include "YK/Types/Math/YK_Vector.h"

template <typename ExtraData>
class CG_RenderTargetBase
{
public:
    CG_RenderTargetBase();
    ~CG_RenderTargetBase();

    bool Initialize();
    bool IsInitialized() const;

    void Bind() const;
    void BindAsInputTexture(YK_U32 p_slotIndex) const;

    void SetSize(YK_Vector2i p_size)
    {
        m_size = p_size;
        m_aspectRatio = static_cast<float>(m_size.x) / static_cast<float>(m_size.y);
    }

    YK_Vector2i GetSize() const { return m_size; }
    float GetAspectRatio() const { return m_aspectRatio; }

private:
    YK_Vector2i m_size = YK_Vector2i::Zero();
    float m_aspectRatio = 1.0f;
    ExtraData m_extraData;
};