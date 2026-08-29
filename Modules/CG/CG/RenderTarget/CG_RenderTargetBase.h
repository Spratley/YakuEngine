#pragma once

#include "YK/Types/Math/YK_Integer.h"
#include "YK/Types/Math/YK_Vector.h"

template <typename ExtraData>
class CG_RenderTargetBase
{
public:
    CG_RenderTargetBase(YK_Vector2i p_size);
    ~CG_RenderTargetBase();

    bool Initialize();
    bool IsInitialized() const;

    void SetSize(YK_Vector2i p_size) { m_size = p_size; }

    void Bind() const;
    void BindAsInputTexture(YK_U32 p_slotIndex) const;

private:
    YK_Vector2i m_size;
    ExtraData m_extraData;
};