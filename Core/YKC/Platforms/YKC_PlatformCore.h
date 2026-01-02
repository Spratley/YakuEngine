#pragma once
#include "YKC_PlatformDefines.h"

// TODO: Migrate platform wrapper to YKC, it should be lower in the architecture

#if YK_WINDOWS
#include "YKC/Platforms/Windows/YKC_WindowsCore.h"
#else
#error No Platform Selected!
#endif