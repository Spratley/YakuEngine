#pragma once

#define YKC_SAFE_DELETE(ptr) do { delete ptr; ptr = nullptr; } while(false)