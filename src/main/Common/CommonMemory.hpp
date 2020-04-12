#pragma once
#include <stddef.h>

enum class Difficult
{
    EASY,
    NORMAL,
    HARD,
    VERYHARD,
    EXTREME,
};  // namespace Difficult

enum class PlayerType
{
    Standard,
    Attack,
    Defence,
    Speed,
};

class CommonMemory
{
    static int difficulty;
    static int GameStartTime;
    static int playerType;

public:
    static int GetGameStartTime()
    {
        return GameStartTime;
    }

    static void SetGameStartTime(int starttime)
    {
        GameStartTime = starttime;
    }

    static int GetDifficulty()
    {
        return difficulty;
    }

    static void SetDifficulty(int _difficulty)
    {
        difficulty = _difficulty;
    }

    static void SetDifficulty(Difficult _difficulty)
    {
        difficulty = static_cast<int>(_difficulty);
    }

    static int GetPlayerType()
    {
        return playerType;
    }

    static void SetPlayerType(int type)
    {
        playerType = type;
    }

    static void SetPlayerType(PlayerType type)
    {
        playerType = static_cast<int>(type);
    }
};
