#pragma once
#include "YK/Platforms/YK_PlatformDefines.h"

#define CG_DEBUG_DRAW_ENABLED !YAKU_RETAIL && YK_PLATFORM == YK_WINDOWS

#if CG_DEBUG_DRAW_ENABLED
#define CG_IF_DEBUG_DRAW_ENABLED(X) X
#else
#define CG_IF_DEBUG_DRAW_ENABLED(X)
#endif // CG_DEBUG_DRAW_ENABLED