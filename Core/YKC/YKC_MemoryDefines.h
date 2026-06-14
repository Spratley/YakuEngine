#pragma once

// TODO: Maybe rename this file since it's not all defines anymore?
template <typename... Args>
constexpr void YK_Unused(Args&&...)
{}

#define YK_SAFE_DELETE(ptr) do { delete ptr; ptr = nullptr; } while(false)
#define YK_SAFE_DELETE_ARRAY(X) { delete[] X; X = nullptr; }

#define YK_STEAL_MEMBER(MEMBER, SOURCE, CLEAR_VALUE) { MEMBER = SOURCE.MEMBER; SOURCE.MEMBER = CLEAR_VALUE; }