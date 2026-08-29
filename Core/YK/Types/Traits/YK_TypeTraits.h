#pragma once

#include <type_traits>

template <typename Type_T>
struct YK_TypeType
{
    using Type = Type_T;
};
template <typename Type_T>
using YK_TypeType_T = typename YK_TypeType<Type_T>::Type;

template <typename SourceType, typename DestinationType>
struct YK_CopyCV
{
    using Type = std::conditional_t<std::is_const_v<SourceType>, DestinationType const, DestinationType>;
};
template <typename SourceType, typename DestinationType>
using YK_CopyCV_T = typename YK_CopyCV<SourceType, DestinationType>::Type;