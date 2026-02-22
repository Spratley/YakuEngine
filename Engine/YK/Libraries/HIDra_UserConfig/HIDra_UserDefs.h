#include "YKC/Platforms/YKC_PlatformDefines.h" // Needed for YK_WINDOWS, etc.

// Platform
#if YK_WINDOWS
#define HIDra_Platform HIDra_Platform_Windows
#elif YK_WEB_ASSEMBLY
#define HIDra_Platform HIDra_Platform_WebAssembly
#endif

// Features
#define HIDra_Gamepad HIDra_Enable
#define HIDra_GP_Generic HIDra_Enable
#define HIDra_GP_Multiple HIDra_Disable

#define HIDra_Keyboard HIDra_Enable
#define HIDra_Mouse HIDra_Enable

// Debug
#if YAKU_RETAIL
#define HIDra_Debug HIDra_Disable
#else
#define HIDra_Debug HIDra_Enable
#endif // YAKU_RETAIL