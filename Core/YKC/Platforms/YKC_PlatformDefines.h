#pragma once

// Defines what platform is actively being targeted by this build
#define YK_WASM 0x00
#define YK_WINDOWS 0x01

#if defined(__EMSCRIPTEN__)
#define YK_PLATFORM YK_WASM
#elif defined(_WIN32)
#define YK_PLATFORM YK_WINDOWS
#else
// No platform selected? What are you trying to build then?
#endif