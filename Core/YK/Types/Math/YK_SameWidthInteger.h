#pragma once

#include "YK/Types/Math/YK_Integer.h"

template <typename T>
struct YK_SameWidthInt;

template <typename T>
requires(sizeof(T) == 1)
struct YK_SameWidthInt<T>
{
    using Type = YK_Int8;
};

template <typename T>
requires(sizeof(T) == 2)
struct YK_SameWidthInt<T>
{
    using Type = YK_Int16;
};

template <typename T>
requires(sizeof(T) == 4)
struct YK_SameWidthInt<T>
{
    using Type = YK_Int32;
};

template <typename T>
requires(sizeof(T) == 8)
struct YK_SameWidthInt<T>
{
    using Type = YK_Int64;
};

template <typename T>
using YK_SameWidthInt_T = typename YK_SameWidthInt<T>::Type;