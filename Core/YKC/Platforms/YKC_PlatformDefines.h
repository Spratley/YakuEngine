#pragma once

// Defines what platform is actively being targeted by this build
// This should be more robust and probably not defaulting to windows
#if __EMSCRIPTEN__
#define YK_WEB_ASSEMBLY 1
#else
#define YK_WINDOWS 1
#endif // __EMSCRIPTEN__