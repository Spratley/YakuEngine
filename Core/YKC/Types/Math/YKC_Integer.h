#pragma once

using YK_Int8 = signed char;
using YK_Int16 = signed short;
using YK_Int32 = signed int;
using YK_Int64 = signed long long;

using YK_U8 = unsigned char;
using YK_U16 = unsigned short;
using YK_U32 = unsigned int;
using YK_U64 = unsigned long long;

using YK_SizeT = decltype(sizeof(0));

#define TEST_INT_SIZE(Base) static_assert(sizeof(YK_Int##Base) == (Base / 8), "Incorrectly sized integer detected!: YK_Int" #Base); \
static_assert(sizeof(YK_U##Base) == (Base / 8), "Incorrectly sized unsigned integer detected!: YK_U" #Base);
TEST_INT_SIZE(8);
TEST_INT_SIZE(16);
TEST_INT_SIZE(32);
TEST_INT_SIZE(64);
#undef TEST_INT_SIZE