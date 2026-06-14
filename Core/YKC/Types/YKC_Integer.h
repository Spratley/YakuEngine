#pragma once

using YK_Int8 = signed char;
using YK_Int16 = signed short;
using YK_Int32 = signed int;
using YK_Int64 = signed long long;

using YK_U8 = unsigned char;
using YK_U16 = unsigned short;
using YK_U32 = unsigned int;
using YK_U64 = unsigned long long;

// TODO: Validate that this is correct
// Needs support for non-64 bit platforms?
using YK_SizeT = YK_U64;

constexpr inline YK_Int8 YK_Int8_Max = 0x7F;
constexpr inline YK_Int16 YK_Int16_Max = 0x7FFF;
constexpr inline YK_Int32 YK_Int32_Max = 0x7FFFFFFF;
constexpr inline YK_Int64 YK_Int64_Max = 0x7FFFFFFFFFFFFFFF;

constexpr inline YK_Int8 YK_Int8_Min = static_cast<YK_Int8>(0x80);
constexpr inline YK_Int16 YK_Int16_Min = static_cast<YK_Int16>(0x8000);
constexpr inline YK_Int32 YK_Int32_Min = static_cast<YK_Int32>(0x80000000);
constexpr inline YK_Int64 YK_Int64_Min = static_cast<YK_Int64>(0x8000000000000000);

constexpr inline YK_U8 YK_U8_Max = 0xFF;
constexpr inline YK_U16 YK_U16_Max = 0xFFFF;
constexpr inline YK_U32 YK_U32_Max = 0xFFFFFFFF;
constexpr inline YK_U64 YK_U64_Max = 0xFFFFFFFFFFFFFFFF;

#define TEST_INT_SIZE(Base) static_assert(sizeof(YK_Int##Base) == (Base / 8), "Incorrectly sized integer detected!: YK_Int" #Base); \
static_assert(sizeof(YK_U##Base) == (Base / 8), "Incorrectly sized unsigned integer detected!: YK_U" #Base);
TEST_INT_SIZE(8);
TEST_INT_SIZE(16);
TEST_INT_SIZE(32);
TEST_INT_SIZE(64);
#undef TEST_INT_SIZE