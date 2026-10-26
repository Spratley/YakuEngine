#pragma once

template <typename ToType, typename FromType>
constexpr ToType YK_SafetyCast(FromType p_from)
{
#if YAKU_RETAIL
    return static_cast<ToType>(p_from);
#else
    ToType result = dynamic_cast<ToType>(p_from);
    YK_ASSERT(result, "Cast failed! Provided To type is incompatible with object!");
    return result;
#endif // YAKU_RETAIL
}