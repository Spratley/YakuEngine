#pragma once

#include "YK/Types/Math/YK_Vector.h"
#include "YK/Types/Other/YK_Callback.h"

// DisplaySurface is the endpoint of the render pipeline where we swap the buffer to
// Just don't want to call it a window because not everything uses windows
template <typename PlatformSpecificData>
class YK_DisplaySurfaceBase
{
public:
    YK_DisplaySurfaceBase() = default;
    YK_DisplaySurfaceBase(YK_DisplaySurfaceBase const&) = delete;
    YK_DisplaySurfaceBase(YK_DisplaySurfaceBase&&) = delete;
    ~YK_DisplaySurfaceBase();

    YK_DisplaySurfaceBase& operator=(YK_DisplaySurfaceBase const&) = delete;
    YK_DisplaySurfaceBase& operator=(YK_DisplaySurfaceBase&&) = delete;

    bool Init(YK_Vector2i p_dimensions)
    {
        YK_ASSERT(!IsValid(), "Attempting to re-initialize main display surface!");
        return InitImpl(p_dimensions);
    }

    bool IsValid() const;
    bool ShouldClose() const;

    void SwapBuffers() const;

    void* GetNativeHandle() const;

    YK_Callback<void, YK_Vector2i>& GetResizedCallback() { return m_onResizedCallback; }
    void OnSurfaceResized(YK_Vector2i p_dimensions) const
    {
        m_onResizedCallback.Run(p_dimensions);
    }

private:
    bool InitImpl(YK_Vector2i p_dimensions);

private:
    YK_Callback<void, YK_Vector2i> m_onResizedCallback;
    PlatformSpecificData m_platformSpecificData;
};