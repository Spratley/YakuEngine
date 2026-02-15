#pragma once

// Defines what platform is actively being targeted by this build
// This should be more robust and probably not defaulting to windows
#if __EMSCRIPTEN__
#define YK_WEB_ASSEMBLY 1
#define YK_PLATFORM WASM
#else
#define YK_WINDOWS 1
#define YK_PLATFORM Windows
#endif // __EMSCRIPTEN__

// Not the biggest fan of this, but it's needed to stop heap allocation in platform objects
#define YKC_PLATFORM_INCLUDE(Folder, File) YKC_STRINGIFY(Folder\\YK_PLATFORM\\File)