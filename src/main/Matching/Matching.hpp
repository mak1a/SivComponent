

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

public:
    static void SyncRoomInfo(bool gameStart = false);
    static int GameStartTime;
    static int Difficulty;

    // 1 ~ 5
    static int GetDifficulty()
    {
        return Difficulty;
    }
};
