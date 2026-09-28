#pragma once

class YK_NotCopyable
{
public:
    constexpr YK_NotCopyable() = default;
    YK_NotCopyable(YK_NotCopyable const&) = delete;
    YK_NotCopyable& operator=(YK_NotCopyable const&) = delete;
};

class YK_NotMovable
{
public:
    constexpr YK_NotMovable() = default;
    YK_NotMovable(YK_NotMovable&&) = delete;
    YK_NotMovable& operator=(YK_NotMovable&&) = delete;
};

class YK_NotCopyableNotMovable
    : public YK_NotCopyable
    , public YK_NotMovable
{};