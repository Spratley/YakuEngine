#pragma once

// I build YakuEn projects with all warnings as errors, this is to suppress unused object warnings
template <typename... Args>
constexpr void YK_Unused(Args&&...)
{}

// Is this really needed?
#define YK_SAFE_DELETE(ptr) do { delete ptr; ptr = nullptr; } while(false)
#define YK_SAFE_DELETE_ARRAY(X) { delete[] X; X = nullptr; }

// This just seems lazy, past Jacob
#define YK_STEAL_MEMBER(MEMBER, SOURCE, CLEAR_VALUE) { MEMBER = SOURCE.MEMBER; SOURCE.MEMBER = CLEAR_VALUE; }