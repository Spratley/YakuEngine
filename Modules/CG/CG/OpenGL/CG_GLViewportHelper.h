#pragma once

// Should this be a stack instead of just a single static value?
// This definitely isn't the right way to go around this
// Gotta figure this out
class CG_GLViewportHelper
{
public:
    static void SetViewportSize(YK_Vector2i p_viewportSize);
    static YK_Vector2i const& GetViewportSize() { return s_viewportSize; }

private:
    static inline YK_Vector2i s_viewportSize = YK_Vector2i(0, 0);
};