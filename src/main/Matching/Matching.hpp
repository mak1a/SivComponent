

#pragma once

#include "../../ComponentEngine/IScene.hpp"
#include "../../PhotonComponent/PhotonComponent.hpp"

class Matching : public ComponentEngine::IScene
{
    std::shared_ptr<ComponentEngine::Photon::NetworkSystem> system;

    void Setup();

public:
    static void SyncRoomInfo(bool gameStart = false);
    static int GameStartTime;
    static int Difficulty;

    static int GetDifficulty()
    {
        return Difficulty;
    }
};
