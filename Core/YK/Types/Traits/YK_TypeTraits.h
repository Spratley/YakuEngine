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

template <typename Type_T>
struct YK_StripCVR
{
    using Type = std::remove_cvref_t<Type_T>;
};
template <typename Type_T>
using YK_StripCVR_T = typename YK_StripCVR<Type_T>::Type;

template <typename Type_T>
struct YK_StripReference
{
    using Type = std::remove_reference_t<Type_T>;
};
template <typename Type_T>
using YK_StripReference_T = typename YK_StripReference<Type_T>::Type;

template <typename Type_T, typename SelfType>
struct YK_DeducedReturn
{
    // Copies the underlying const-ness of SelfType onto Type_T, ignoring if the type is a reference
    // Used with deducing this to return a const value when the method is called from a const object
    using Type = typename YK_CopyCV<YK_StripReference_T<SelfType>, Type_T>::Type;
};
template <typename Type_T, typename SelfType>
using YK_DeducedReturn_T = typename YK_DeducedReturn<Type_T, SelfType>::Type;