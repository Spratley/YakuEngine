#pragma once
#include "YK_PlatformDefines.h"

#if YK_WINDOWS
#include "Windows/YK_WindowsCore.h"
#else
#error No Platform Selected!
#endif