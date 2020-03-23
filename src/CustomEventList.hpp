#pragma once

namespace CustomEvent
{
    typedef unsigned char nByte;
    constexpr nByte GameStart = 1;
    constexpr nByte PlayerInit = 10;
    constexpr nByte PlayerSync = 11;
    constexpr nByte PlayerShot = 12;
}  // namespace CustomEvent
