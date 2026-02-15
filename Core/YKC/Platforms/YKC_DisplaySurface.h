#pragma once

// Platform specific data to store directly in DisplaySurfaec
#include YKC_PLATFORM_INCLUDE(YKC/Platforms, YKC_DisplaySurfaceContents.h)

// YKC_DisplaySurface is the platform agnostic window class
// Just dont' want to call it a window because not everything uses windows
// DisplaySurface is the endpoint of the render pipeline where we swap the buffer to
class YKC_DisplaySurface
{
public:
    YKC_DisplaySurface() = default;
    YKC_DisplaySurface(YKC_DisplaySurface const&) = delete;
    YKC_DisplaySurface(YKC_DisplaySurface&&) = delete;
    ~YKC_DisplaySurface();

    bool Init(YK_Vector2i p_dimensions)
    {
        if (IsValid())
        {
            return false;
        }
        return InitImpl(p_dimensions);
    }

    bool IsValid() const;
    bool ShouldClose() const;

    void* GetNativeHandle() const;

    void SwapBuffers() const;

    void SetResizedCalback(void (*p_callback)(YK_Vector2i)) { m_onResizedCallback = p_callback; }
    void OnWindowResized(YK_Vector2i p_dimensions) const
    {
        if (m_onResizedCallback)
        {
            m_onResizedCallback(p_dimensions);
        }
    }

    // TODO: REMOVE!! THIS DATA SHOULD NOT BE EXPOSED AS PART OF THE PUBLIC API
    // I REPEAT THIS IS PLATFORM SPECIFIC CONTENT, DO NOT TREAT AS PUBLIC API
    YKC_DisplaySurfaceContents& GetContents() { return m_contents; }
    YKC_DisplaySurfaceContents const& GetContents() const { return m_contents; }

private:
    bool InitImpl(YK_Vector2i p_dimensions);

private:
    YKC_DisplaySurfaceContents m_contents;

    // TODO: Come back to this
    void (*m_onResizedCallback)(YK_Vector2i p_dimensions) = nullptr;
};