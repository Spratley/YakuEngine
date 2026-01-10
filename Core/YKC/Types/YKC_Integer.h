#pragma once

typedef signed char YK_Int8;
typedef signed short YK_Int16;
typedef signed int YK_Int32;
typedef signed long long YK_Int64;

typedef unsigned char YK_U8;
typedef unsigned short YK_U16;
typedef unsigned int YK_U32;
typedef unsigned long long YK_U64;

constexpr YK_Int8 YK_Int8_Max = 0x7F;
constexpr YK_Int16 YK_Int16_Max = 0x7FFF;
constexpr YK_Int32 YK_Int32_Max = 0x7FFFFFFF;
constexpr YK_Int64 YK_Int64_Max = 0x7FFFFFFFFFFFFFFF;

constexpr YK_Int8 YK_Int8_Min = static_cast<YK_Int8>(0x80);
constexpr YK_Int16 YK_Int16_Min = static_cast<YK_Int16>(0x8000);
constexpr YK_Int32 YK_Int32_Min = static_cast<YK_Int32>(0x80000000);
constexpr YK_Int64 YK_Int64_Min = static_cast<YK_Int64>(0x8000000000000000);

constexpr YK_U8 YK_U8_Max = 0xFF;
constexpr YK_U16 YK_U16_Max = 0xFFFF;
constexpr YK_U32 YK_U32_Max = 0xFFFFFFFF;
constexpr YK_U64 YK_U64_Max = 0xFFFFFFFFFFFFFFFF;