

#pragma once

#include "../../ComponentEngine/IScene.hpp"
#include "../../PhotonComponent/PhotonComponent.hpp"

enum class Difficult
{
    EASY,
    NORMAL,
    HARD,
    VERYHARD,
    EXTREME,
};  // namespace Difficult

class Matching : public ComponentEngine::IScene
{
    std::shared_ptr<ComponentEngine::Photon::NetworkSystem> system;

    void Setup();

    static int Difficulty;

public:
    static void SyncRoomInfo(bool gameStart = false);
    static int GameStartTime;

    static void SetDifficulty(int difficult)
    {
        Difficulty = difficult;
    }

    static int GetDifficulty()
    {
        return Difficulty;
    }
};
