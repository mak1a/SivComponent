#pragma once

namespace CustomEvent
{
    typedef unsigned char nByte;
    constexpr nByte GameStart = 1;
    constexpr nByte PlayerInit = 10;
    constexpr nByte PlayerSync = 11;
    constexpr nByte PlayerShot = 12;
    constexpr nByte PlayerBulletHit = 13;

    constexpr nByte EnemyGenerate = 20;
    constexpr nByte EnemySync = 21;

    constexpr nByte GameEnd = 99;
}  // namespace CustomEvent
