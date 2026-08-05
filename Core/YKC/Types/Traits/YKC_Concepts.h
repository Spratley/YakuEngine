#pragma once

#include <type_traits>

template <typename T>
concept YK_NumericType = std::is_arithmetic_v<T>;