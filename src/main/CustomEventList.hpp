﻿#pragma once

namespace CustomEvent
{
    typedef unsigned char nByte;
    constexpr nByte MatchingSync = 1;

    constexpr nByte PlayerInit = 10;
    constexpr nByte PlayerSync = 11;
    constexpr nByte PlayerShot = 12;
    constexpr nByte PlayerBulletHit = 13;
    constexpr nByte PlayerSpecial = 14;

    constexpr nByte EnemyGenerate = 20;
    constexpr nByte EnemySync = 21;

    constexpr nByte PlayerCoreSync = 30;

    constexpr nByte GameResult = 99;
}  // namespace CustomEvent
