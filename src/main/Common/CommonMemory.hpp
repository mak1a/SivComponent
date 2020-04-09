#pragma once

class CommonMemory
{
    static int Difficulty;
    static int GameStartTime;

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
        return Difficulty;
    }

    static void SetDifficulty(int difficult)
    {
        Difficulty = difficult;
    }
};
